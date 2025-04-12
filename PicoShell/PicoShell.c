
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_N 16384
#define PROMPT "Pico shell prompt >"

char* find_program_in_path(const char* program_name);
int execute(char* program_path, char* arguments);

char* path_env ;

int picoshell_main(int argc, char* argv[]) {

	path_env = getenv("PATH");
	if (!path_env) return 0;

	    char input[MAX_INPUT_N];
	    printf("%s", PROMPT);
	    fflush(stdout);

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
                char *word = strtok(input + 5, " \n");
                while (word) {
                    printf("%s", word);
                    word = strtok(NULL, " \n");
                    if (word) printf(" ");
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

            else if (strlen(input) > 0 && strcmp(input, "\n") != 0) {
            	char* tmp_input = strtok(input,"\n");
            	char* prog_path = find_program_in_path(strtok(strdup(tmp_input)," "));
            	if(prog_path == NULL)
                	printf("invalid_command: command not found\n");
                else
                	execute(prog_path,tmp_input);
            }

            printf("%s", PROMPT);
            fflush(stdout);

            
        }else{
            exit(0) ;
        }
    
    }
        
    }




int execute(char* program_path, char* arguments){

	pid_t child_pid = fork();
	if(child_pid == -1){
		printf("Error Fork");
	}
	else if (child_pid == 0 )
	{
            if(strlen(arguments) > 0)
        	{
        	    char* args[MAX_INPUT_N];
        		char* token = strtok(arguments," ");
        		int counter = 0;
        		
        		while(token != NULL){
        		
        			args[counter++] = token;
        			token = strtok(NULL," ");
        		} 
        		args[counter] = NULL; 
        
        
        	execv(program_path,args);
        	}
        	else{
        	    execv(program_path,NULL);
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


