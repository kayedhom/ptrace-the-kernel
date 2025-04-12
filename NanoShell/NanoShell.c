
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_N 16384
#define PROMPT "Nano shell prompt >"
#define VAR_CAPACITY 256
#define STRING_CAPACITY 256

char* find_program_in_path(const char* program_name);
int execute(char* program_path, char* arguments);
char* get_var(char* variable);
void add_variable(char* var);
void echo_var(char* input);
char* expand_variables(const char* str);
void export_var(char* input);
char* path_env ;
char** local_vars = NULL;
int var_count = 0;


int main(int argc, char* argv[]) {

	path_env = getenv("PATH");
	if (!path_env) return 0;

	    char input[MAX_INPUT_N];
	    printf("%s", PROMPT);
	    fflush(stdout);

	local_vars = (char**) malloc(VAR_CAPACITY * sizeof(char*));
    while(1) {
        char* ret = fgets(input, MAX_INPUT_N, stdin) ;
        // Get input
        if (ret != NULL)
        {
	    if(strchr(input,'=') != NULL){
	    	add_variable(input);
	    }
            else if (strcmp(input, "exit\n") == 0) {
                printf("Good Bye\n");
                exit(0);
                break;
            }
            else if (strncmp(input, "echo ", 5) == 0) {
            	char* dol = strchr(input, '$' ) ;
                if(dol == NULL || *(dol+1) == ' '){
                char *word = strtok(input + 5, " \n");
                while (word) {
                    printf("%s", word);
                    word = strtok(NULL, " \n");
                    if (word) printf(" ");
                }
                }
                else{
                	echo_var(input);
                }
                printf("\n");
            }
            else if (strncmp(input, "cd ", 3) == 0) {
            	char* tmp_input = strtok(input+3,"\n");
                if(chdir(tmp_input) < 0)
                    {
                        printf("cd: /invalid_directory: No such file or directory\n");
                        fflush(stdout);
                    }
            }
            else if (strncmp(input, "export ", 7) == 0) {
                export_var(input);
            }
	else if (strlen(input) > 0 && strcmp(input, "\n") != 0) {
	                   // Expand variables before executing the command
                char* expanded_input = expand_variables(input);
                
                char* tmp_input = strtok(expanded_input, "\n");
                
                // Create a copy to extract the command name (first word)
                char* cmd_copy = strdup(tmp_input);
                char* first_word = strtok(cmd_copy, " ");
                
                // Find the program in PATH
                char* prog_path = find_program_in_path(first_word);
                free(cmd_copy);
                
                if (prog_path == NULL)
                    printf("invalid_command: command not found\n");
                else
                    execute(prog_path, expanded_input);
                
                free(expanded_input);  // Free the expanded input after use
	}

            printf("%s", PROMPT);
            fflush(stdout);

            
        }else{
            exit(0) ;
        }
    
    }
        
    }


char* get_var(char* variable) {
    for (int i = 0; i < var_count; i++) {
        char* equal_pos = strchr(local_vars[i], '=');
        if (equal_pos == NULL) continue;
        
        int var_len = equal_pos - local_vars[i];
        char var_name[256];
        
        if (var_len >= 256) continue; // Avoid buffer overflow
        
        strncpy(var_name, local_vars[i], var_len);
        var_name[var_len] = '\0';
        
        if (strcmp(variable, var_name) == 0) {
            return equal_pos + 1;
        }
    }
    return NULL; // Return NULL instead of '\0'
}

void echo_var(char* input) {
    char* dollar_pos = strchr(input, '$');
    if (!dollar_pos || *(dollar_pos+1) == ' ' || *(dollar_pos+1) == '\0' || *(dollar_pos+1) == '\n') {
        return;
    }
    
    // Extract the variable name
    char var_name[256];
    char* start = dollar_pos + 1;
    int i = 0;
    
    // Copy until space or newline
    while (start[i] != ' ' && start[i] != '\n' && start[i] != '\0' && i < 255) {
        var_name[i] = start[i];
        i++;
    }
    var_name[i] = '\0';
    
    // Get the variable value
    char* value = get_var(var_name);
    
    // Print the value if it exists
    if (value != NULL && value[0] != '\0') {
        printf("%s", value);
    }
}

void add_variable(char* var){
 	char * eq_loc = strchr(var,'=');
	if(eq_loc != var && *(eq_loc+1) != '\0' ){
		if(*(eq_loc+1) != ' ' && *(eq_loc-1) != ' ')
		{
			//printf("%s\n",var);
			local_vars[var_count] = (char*)malloc(STRING_CAPACITY*sizeof(char));
			strcpy(local_vars[var_count],var);
			//printf(local_vars[var_count]);
			var_count++;
		}
		else{
			printf("Invalid command\n");
			fflush(stdout);
		}
	}
	else{
		printf("Invalid command\n");
		fflush(stdout);
	}
}

void export_var(char* input) {
    // Skip "export " part
    char* var_name = input + 7;
    
    // Remove trailing newline if present
    char* newline = strchr(var_name, '\n');
    if (newline) *newline = '\0';
    
    // Trim any leading/trailing whitespace
    while (*var_name == ' ') var_name++;
    
    char* value = get_var(var_name);
    if (value != NULL) {
        // Create a string in format "NAME=VALUE" for setenv
        char env_string[512];
        snprintf(env_string, sizeof(env_string), "%s=%s", var_name, value);
        
        // Use putenv or setenv to set the environment variable
        if (putenv(strdup(env_string)) != 0) {
            printf("Failed to export variable %s\n", var_name);
        }
    } else {
        printf("Variable %s not found\n", var_name);
    }
}

// Function to expand variables in a string
char* expand_variables(const char* str) {
    char* result = malloc(MAX_INPUT_N * sizeof(char));
    int res_index = 0;
    
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '$' && str[i+1] != ' ' && str[i+1] != '\0' && str[i+1] != '\n') {
            // Found a variable reference
            char var_name[256];
            int name_len = 0;
            
            // Extract variable name
            i++; // Skip '$'
            while (str[i] != ' ' && str[i] != '\0' && str[i] != '\n' && name_len < 255) {
                var_name[name_len++] = str[i++];
            }
            var_name[name_len] = '\0';
            i--; // Step back as loop will increment i
            
            // Get variable value and append to result
            char* value = get_var(var_name);
            if (value != NULL) {
                strcat(result + res_index, value);
                res_index += strlen(value);
            }
        } else {
            // Regular character
            result[res_index++] = str[i];
        }
    }
    
    result[res_index] = '\0';
    return result;
}


int execute(char* program_path, char* arguments) {
    pid_t child_pid = fork();
    if (child_pid == -1) {
        printf("Error Fork");
    }
    else if (child_pid == 0) {
        if (strlen(arguments) > 0) {
            // Expand variables in the arguments
            char* expanded_args = expand_variables(arguments);
            
            char* args[MAX_INPUT_N];
            char* token = strtok(expanded_args, " ");
            int counter = 0;
            
            while (token != NULL) {
                args[counter++] = token;
                token = strtok(NULL, " ");
            } 
            args[counter] = NULL; 
            
            execv(program_path, args);
            
            // If execv fails
            free(expanded_args);
            perror("execv failed");
            exit(1);
        }
        else {
            execv(program_path, NULL);
        }
    }

    wait(&child_pid);
    return 1;
}


char* find_program_in_path(const char* program_name) {
    char* path_copy = strdup(path_env);
    char* dir = strtok(path_copy, ":");

    while (dir) {
        char* full_path = (char*)malloc(strlen(dir) + strlen(program_name) + 2);
        sprintf(full_path, "%s/%s", dir, program_name);
 
        if (access(full_path, X_OK) == 0) {free(path_copy); return full_path; }
        free(full_path); dir = strtok(NULL, ":");
    }
    free(path_copy); return NULL;
}


