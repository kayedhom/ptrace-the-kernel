
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT_N 16384
#define PROMPT "Femto shell prompt >"

char* find_program_in_path(const char* program_name);
int execute(char* program_path);

char* path_env ;

int main(int argc, char *argv[]) {

	path_env = getenv("PATH");
	if (!path_env) return 0;

	    char input[MAX_INPUT_N];
	    int count = 0;
	    printf("%s", PROMPT);

    while(1) {
        char* ret = fgets(input, MAX_INPUT_N, stdin) ;
        // Get input
        if (ret != NULL)
        {

            if (strcmp(input, "exit\n") == 0) {
                printf("Good Bye\n");
                exit(0);
                break;
            }
            else if (strncmp(input, "echo ", 5) == 0) {
                printf("%s", input + 5);
            }
            else if (strlen(input) > 0 && strcmp(input, "\n") != 0) {
            	char* tmp_input = strtok(input,"\n");
            	char* prog_path = find_program_in_path(tmp_input);
            	if(prog_path == NULL)
                	printf("Invalid command\n");
                else
                	execute(prog_path);
            }

            printf("%s", PROMPT);

            
        }else{
            exit(0) ;
        }
    
    }
        
    }


char* find_program_in_path(const char* program_name) {
    char* path_copy = strdup(path_env);
    char* dir = strtok(path_copy, ":");

    while (dir) {
        char* full_path = malloc(strlen(dir) + strlen(program_name) + 2);
        sprintf(full_path, "%s/%s", dir, program_name);
 
        if (access(full_path, X_OK) == 0) {free(path_copy); return full_path; }
        free(full_path); dir = strtok(NULL, ":");
    }
    free(path_copy); return NULL;
}



int execute(char* program_path){

	pid_t child_pid = fork();
	if(child_pid == -1){
		printf("Error Fork");
	}
	else if (child_pid == 0 )
	{
	execv(program_path,NULL);
	}
	else{
	wait(child_pid);
		return 1;
	}

}
