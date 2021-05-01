#include"Utility.h"

//---------------------------------------------------Utility Funtions-------------------------------------------------------------------------

void ChangeDirectory(char * command){
	if(command[2] == '\0'){
		char cwd[256];
		char home[] = "/home";
		getcwd(cwd, sizeof(cwd));
		while(strcmp(cwd,home) != 0){
			chdir("..");
			getcwd(cwd, sizeof(cwd));
		}
	}
	else{
		char newdir[256];
		int i = 0;
		for(; command[i] != '\0' ; i++){
			newdir[i] = command[3+i];
		}
		newdir[i] = '\0';
		if(chdir(newdir) != 0)
			errormessage("Error :: No Such File or Directory");
	}
}

void lsImplementation(){
    char curr_dir[256];
    DIR *dp = NULL;

    getcwd(curr_dir, sizeof(curr_dir));								//get current directory
    if(curr_dir == NULL){
        errormessage("--- ERROR :: Could not get the working directory ---");
        return;
    }

    dp = opendir((const char*)curr_dir);
    if(dp == NULL)
    {
        printf("--- ERROR : Could not open the working directory ---");
        return;
    }

	struct dirent *temp = readdir(dp);						
    for(int i = 0; temp != NULL ; i++)
    {
        printf("%s  ",temp->d_name);
		temp = readdir(dp);
    }
	printf("\n");
    return ;
}

void Environment(char * command){
	char env1[] = "environ";		//all possible formats provided by user
	char env2[] = "environ ";
	char env3[] = "environ\t";
		
	if(strcmp(command, env1) == 0 || strcmp(command, env2) == 0 || strcmp(command, env3) == 0){		//so that even if user adds a space or tab etc it's still correct input and result should be displayed
		extern char **environ;
		int i = 1;
		char *s = *environ;

		for (; s; i++) {				//loop to list entire environment variables
    		printf("%s\n", s);
    		s = environ[i];
  		}

  		return ;
	}
}

void setorunsetEnv(char* command){
	char cmd[150][25];
	int num;
	Tokenize(command, cmd, &num);

	if(strcmp(cmd[0],"setenv") == 0){
		if(num >= 1){
			char * x = getenv(cmd[1]);
			if(x != NULL){
				errormessage("---ERROR :: ENVIRONMENT VARIABLE ALREADY DEFINED - CANNOT REDEFINE ---");
				return;
			}
			if(num >= 2)
				setenv(cmd[1],cmd[2],1);
			else	 
				setenv(cmd[1],"",1);
		}
	}

	else if(strcmp(cmd[0],"unsetenv") == 0){
		if(num == 1){
			char* x = getenv(cmd[1]);
			if(x == NULL)	errormessage(" --- ERROR :: ENVIRONMENT VARIABLE ALREADY UNDEFINED ---");
			else unsetenv(cmd[1]);
		}
		else errormessage("--- ERROR :: INCORRECT COMMAND ---");
	}
}

void ExecuteProgram(char* command, char cmd[][25], int numoftokens){
	int find1 = findintokens("<", cmd, numoftokens);
	int find2 = findintokens(">", cmd, numoftokens);
	char* arg[2];
	arg[0] = NULL;
	arg[1] = NULL;	
	if(find1 != -1){													//set all commands between actual command and </> as arguments to pass on exec call
		if(find1 < find2 || find2 == -1){
			for(int i = 1 ; i < find1 ; i++){
				arg[i-1] = cmd[i];
			}
		}
	}
	else if(find2 != -1){
		if(find2 < find1 || find1 == -1){
			for(int i = 1 ; i < find2 ; i++){
				arg[i-1] = cmd[i];
			}
		}
	}
	else if(numoftokens >= 1){											//even if no </> check if arguments sent by user
		for(int i = 1 ; i <= numoftokens && i < 2; i++){
			arg[i-1] = cmd[i];
		}
	}

	pid_t p = fork();
	if(p == 0){
		char cwd[256];
    	char executable[] = "/gbsh";
		getcwd(cwd, sizeof(cwd));							//will get current working directory
    	strcat(cwd, executable);
		setenv("PARENT", cwd , 1);							//setting parent of child function
		execlp(command, command, arg[0],arg[1],NULL);
		errormessage("--- ERROR :: ENVIRON EXEC FAILED --- ");
		exit(0);	
	}
	wait(NULL);
}

void Built_In_GBSH(char* command, char cmd[][25], int numoftokens){
	int find1 = findintokens("<", cmd, numoftokens);
	int find2 = findintokens(">", cmd, numoftokens);
	char* arg[2];
	arg[0] = NULL;
	arg[1] = NULL;	
	if(find1 != -1){													//set all commands between actual command and </> as arguments to pass on exec call
		if(find1 < find2 || find2 == -1){
			for(int i = 1 ; i < find1 ; i++){
				arg[i-1] = cmd[i];
			}
		}
	}
	else if(find2 != -1){
		if(find2 < find1 || find1 == -1){
			for(int i = 1 ; i < find2 ; i++){
				arg[i-1] = cmd[i];
			}
		}
	}
	else if(numoftokens >= 1){											//even if no </> check if arguments sent by user
		for(int i = 1 ; i <= numoftokens && i < 2; i++){
			arg[i-1] = cmd[i];
		}
	}

	if(strcmp(command, "pwd") == 0){
		char cwd[256];
		getcwd(cwd, sizeof(cwd));											//will print current working directory
		printf("%s\n ",cwd);
	}
	else if(strcmp(command,"clear") == 0){
		printf("\e[1;1H\e[2J");												//clear screen
	}
	else if(strcmp(command, "ls") == 0){									//will perform ls and ls -l
		lsImplementation();
	}
	else if(command[0] == 'c' && command[1] == 'd'){						//cd commands 
		ChangeDirectory(command);
	}
	else if(strstr(command, "environ") != NULL){
		Environment(command);												//environ - to display all environment variables
	}
	else if(strstr(command, "setenv") != NULL){
		setorunsetEnv(command);												//setenv - to set environment variables according to users preference
	}
	else if(strcmp(command, "man csh") == 0){							
		system(command);													//NOTE : need to fix this
	}
	else{
		pid_t p = fork();
		if(p==0){
			execlp(command, command,arg[0], arg[1], NULL);
			errormessage("--- ERROR :: EXEC FAILED --- ");
			exit(0);
		}
		wait(NULL);
	}
}

void Execute_External_GBSH(char cmd[][25], int n){};