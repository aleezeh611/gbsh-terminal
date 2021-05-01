#include"Utility.h"

int main(int argc, char *argv[]) {
	signal(SIGINT, SIGINT_Handler);
	int stdout = dup(1);														//store stdout actual value
	int stdin = dup(0);															//store stdin actual value
	char cmd[150][25];
	int* numoftokens  = malloc(sizeof(int));
	int* usetokens = malloc(sizeof(int));										//will tell whether to use tokens or not
	int* pipetokens = malloc(sizeof(int));
	char pipedcommands[50][100];												//store separate commands if pipes are used
	int fd[2];
	int checkpipe = pipe(fd);
	if(checkpipe < 0){
		errormessage(" \n--- ERROR WHILE CREATING PIPE ---\n");
	}
	if (fcntl(fd[0], F_SETFL, O_NONBLOCK) < 0) 	exit(2); 						//so we don't have to worry about opening and closing pipes 

	StartingMessage();
	//setting SHELL to the path of gbsh executable as asked by assignment pdf
	SetShellEnvironment();
	
	//main shell loop to run again and again
	while(1){
		char* command = malloc(256);
		PromptDisplay();
		scanf("%[^\n]%*c", command);											//ask user for command	
		*usetokens = 0;
		*numoftokens = 0;
		*pipetokens = 0;

		char* check = strchr(command, '|');
		if(check!=NULL){
			pipeTokenize(command,pipedcommands,pipetokens);

		}		

		for(int i = 0 ; i <= *pipetokens ; i++){
			if(check!=NULL){
				strcpy(command, pipedcommands[i]);									//if we command consists of pipes then make due changes
				if(i != *pipetokens)
					dup2(fd[1], 1);														//redirect stdoutput onto pipe but not for last program
			}
			printf("\n%s\n", command);
			Tokenize(command, cmd, numoftokens);
			StdOutOverWrite(command,cmd,numoftokens);	
			StdInOverWrite(command,cmd,numoftokens);

			if(strcmp(command, "exit") == 0){ 
				free(command);														//free up memory
				goto exitnow;														//exit our shell
			}
			else if(command[0] == '.' && command[1] == '/'){						//to run programs 
				ExecuteProgram(cmd[0], cmd, *numoftokens);
			}
			else{																	//to run built in shell commands
				Built_In_GBSH(cmd[0], cmd, *numoftokens);							//division into tokens so will be needed to take into consideration
			}

			dup2(fd[0], 0);															//changed , change it back
			dup2(stdout, 1);
		}
		free(command);
		dup2(stdout, 1);														//if stdout or stdin was
		dup2(stdin, 0);															//changed , change it back
	}
	
	exitnow:
		free(usetokens);
		free(numoftokens);
		EndingMessage();
		exit(0); // exit normally	
}
