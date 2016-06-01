#include <stdio.h>
#include <unistd.h>
#include "p1_Unix_Shell.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
/*Maximum legnth of input allowed to the shell.*/
#define INPUT_LENTH 100 
/*Maximum number of argument allowed to be passed to the shell. */
#define ARGUMENT_BUFFER 100        
#define CHAR_BUFFER 100

/*
 *This function should parse the user input into an array. 
 */
int parse_input(char * cmd, char * argv[])
{
	int i = 1;
    argv[0] = strtok(cmd, " \n");   
	while(argv[i-1] != NULL && i < (ARGUMENT_BUFFER - 1))
	{	
	argv[i] = strtok(NULL, " \n");
    i++;
	}
	return i-1;
}
	
/*
 *This function parses the arguments passed from the command line to the shell..
 */
char * parse_argument(int argc, char ** argv)
{
	int arg;
	opterr = 0;
	char * prompt = NULL;
	while((arg = getopt(argc, argv, "p:")) != -1)
	{
		if (arg == 'p'){
		prompt = optarg;
		}
		else{
		abort();
		}
		
	}
	
	if(prompt == NULL)
	{
		prompt = "308sh";
	}

	return prompt;

}

/*
 *This is the main function with the while loop that executes the shell.
* It take the user imput from the command line and parses it to the shell program.
 */
int main(int argc, char ** argv)
{
	char * prompt = parse_argument(argc, argv);
	char input[INPUT_LENTH]; 
	while(1)
	{
		char * argv[ARGUMENT_BUFFER];
		printf(">>>%s> ", prompt);
		fgets(input, INPUT_LENTH, stdin);
		int lastIndex = parse_input(input, argv);
		if(lastIndex != 0)
		{
			if(!strcmp(argv[0], "exit"))
			{
				break;
			}
			else if(!strcmp(argv[0], "cd"))
			{
				cd(argv);
			}	
			else if(!strcmp(argv[0], "cwd"))
			{
			printf(" Current Working Directory : %s\n", getcwd(NULL,CHAR_BUFFER));
			}	
			
			else if(!strcmp(argv[0], "pid"))
			{
				printf("PID = %i\n", getpid());
			}	
			else if(!strcmp(argv[0], "ppid"))
			{
				printf("PPID = %i\n", getppid());
			}	
			else if(!strcmp(argv[lastIndex-1], "&"))
			{
				argv[lastIndex-1] = NULL; 
				execute(argv);
			}
			else
			{
				execute_block(argv);
			}
		}
		int status;
		pid_t process_child = waitpid(-1, &status, WNOHANG);
		if(process_child > 0)
		{
			printf("process %i exited\n", process_child);
			processStatus(process_child, status);
		}
	}
	return 0;
}

/*
 *Fucntion when cd is called. 
 */
void cd(char * argv[])
{
	if(argv[1] == NULL)
	{
		printf("No paramaters supplied to cd\n");
	}
	else
	{
		if(chdir(argv[1]) == -1)
		{
			printf("Cd failed - %s\n", strerror(errno));
		}
	}
}

 
/*
 * This function prints the status so the user can see it.
 */
void processStatus(pid_t childPid, int status)
{
	if(WIFSIGNALED(status))
	{
		printf("Child %i exited with signal %d\n", childPid, WTERMSIG(status));
	}
	else if(WIFEXITED(status))
	{
		printf("Child %i exited with return code %d\n", childPid, WEXITSTATUS(status));
	}
	
}

/*
 *execute_block waits for the process to finish.
 */
void execute_block(char * argv[])
{
	pid_t pid = fork();
	if(pid == 0)
	{
		printf("pid: %i\n", getpid());
		execvp(*argv, argv);
		printf("Unknown command.\n");
		exit(0);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		processStatus(pid, status);
	}
		
}

/*
 *Exceute does not block the main process and waits for the child process to finish. 
 */
void execute(char * argv[])
{
	pid_t pid = fork();
	if(pid == 0)
	{
		printf("\n PID: %i\n", getpid());
		execvp(*argv, argv);
		printf("PID: %i failed - %s\n", getpid(), strerror(errno));
	}
}


