#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

#define MAX_LINE 80
#define BUFFER_SIZE 50

char buffer[BUFFER_SIZE];
char history[10][BUFFER_SIZE]; //store the history command
int NumOfCommand = 0;   //the num of history command, the maximum value is 10
int start_index = 0;

void setup(char inputBuffer[], char *args[], int *background)
{
	int len;
	int start = -1;
	int j = 0;
	int i;
	int flag = 0; 
	
	len = read(STDIN_FILENO, inputBuffer, MAX_LINE); //read the command
	inputBuffer[len] = '\0';
	
	if(strcmp(inputBuffer, "quit\n") == 0 || strcmp(inputBuffer, "exit\n") == 0)
		exit(0);
	
	//first deal with the r x, find the corresponding command from the history, and put it to the inputBuffer
	if(inputBuffer[0] == 'r' && NumOfCommand > 0)
	{
		if(inputBuffer[1] == '\n')
		{
			strcpy(inputBuffer, history[NumOfCommand - 1]);
		}
		else if(inputBuffer[1] == ' ' && inputBuffer[2] != '\0' && inputBuffer[3] == '\n')
		{
			for(i = NumOfCommand - 1; i >= 0; --i)
			{
				if(inputBuffer[2] == history[i][0])
				{
					strcpy(inputBuffer, history[i]);
					flag = 1;
					break;
				}
			}
			if(!flag)    //if there is no command match to the r x, then return and no action to the history
			{
				args[0] = NULL;
				return;
			}

		}
		len = strlen(inputBuffer);
	}

	if(len < 0)  //ctrl c
	{
		args[0] = NULL;
		return;
	}

	if(len == 0)  //ctrl z
	{
		exit(0);
	}
	
	strcpy(history[(start_index + NumOfCommand) % 10], inputBuffer);
	if(NumOfCommand == 10) //if the num of history command is 10,move the olddest command out
	{
		start_index = (start_index + 1 + 10) % 10;
	}
	else
	{
		++NumOfCommand;   	
	}

	//seperating the command to the distinct tokens
	for(i = 0; i < len; ++i)
	{
		if(inputBuffer[i] == ' ' || inputBuffer[i] == '\t') //find the delimiters
		{
			if(start != -1)
			{
				args[j++] = &inputBuffer[start];
				start = -1;
			}
			inputBuffer[i] = '\0';   
		}
		else if(inputBuffer[i] == '\n')
		{
			if(start != -1)
			{
				//inputBuffer[i] = '\0';
				args[j++] = &inputBuffer[start];
			}
			args[j] = NULL;
			inputBuffer[i] = '\0';
		}
		else
		{
			if(inputBuffer[i] == '&')
			{
				*background = 1;
				inputBuffer[i] = '\0';
			}
			if(start == -1)
				start = i;
		}
	}

	args[j] = NULL;

}

void handle_SIGINT()      //get the Ctrl-c signal, to display the history command
{
	//printf("\nCaught Ctrl C\n");
	write(STDOUT_FILENO, buffer, strlen(buffer));
	printf("The Command History:\n");
	int i;
	i = start_index;
	int j = 0;
	fflush(stdout);
	for(; i < NumOfCommand + start_index; ++i)
	{
		printf("%d. %s", ++j, history[i % 10]);
		//fflush(stdout);
	}
	//printf("COMMAND-> ");
	//fflush(stdout);
	return;
	//exit(0);
}


int main()
{
	char input[MAX_LINE];
	int background;
	char *args[MAX_LINE / 2 + 1];
	
	//set up the signal handler
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	sigaction(SIGINT, &handler, NULL);
	
	strcpy(buffer, "\nCaught Control C\n");

	while(1)
	{
		background = 0;
		fflush(stdin);
		printf("COMMAND-> ");
		fflush(stdout);   //print the stdout buffer
		background = 0;	
		setup(input, args, &background); //read the command
		
		//deal with cd command
		if(strcmp(input, "cd") == 0)
		{	
			if(*args[1] == '~')
			{
				strcpy(args[1], "/home/gao");
			}
			if(chdir(args[1]) == 0)
			{	
				continue;
			}
		}
	    if(args[0] != NULL)
		{
			
			pid_t pid;
			
			pid = fork();

			if(pid < 0)
			{
				printf("fork failed\n");
				exit(1);
			}
			else if(pid == 0)
			{
				execvp(args[0], args);
				background = 0;
				printf("command error\n");
				exit(0);
			}

			else
			{
				if(background == 0)
				{
					waitpid(pid, NULL, 0);
				}
			} 
		}
	}
	return 0;
}

	




