#include"build.h"

//------------------------------------CONSISTS OF BASIC BUILD FUNCTIONS THAT BRING THE SHELL TOGETHER---------------------------------------------------
void StartingMessage(){

	printf("\033[0;35m");
	printf("\n-------------------------------------MY SHELL------------------------------------\n\n");
	printf("\033[0m");
}

void EndingMessage(){
	printf("\033[0;35m");
	fprintf(stdout, "\n-----------------------------SHELL HAS BEEN EXITTED------------------------------\n");
 	printf("\033[0m");
}

void PromptDisplay(){
	char* lgn = NULL;
	char hostbuffer[256];
	char cwd[256];
	lgn = getlogin();
	int check = gethostname(hostbuffer, sizeof(hostbuffer));
	getcwd(cwd, sizeof(cwd));
	
	printf("\033[1;31m");
	if(check != -1 && lgn != NULL) printf("%s@%s ",lgn, hostbuffer);
	printf("\033[1;33m");
	printf("%s ",cwd);
    printf("\033[0m"); //Resets the text to default color
}

void SetShellEnvironment(){
	char cwd[256];
    char executable[] = "/gbsh";
	getcwd(cwd, sizeof(cwd));							//will get current working directory
    strcat(cwd, executable);

	setenv("SHELL", cwd,1);
	char* value;
	value = getenv("SHELL");
	if(value){
      //printf("VALUE of new SHELL environment variable is: %s\n",value);   
    }
    else{
        printf(" -- ERROR :: SHELL ENVIRONMENT NOT ALTERED TO PATH OF GBSH EXECUTABLE --\n");
    }    
}

void Tokenize(char * str, char tokens[150][25], int* n){
	int c = 0;
	int j = 0;
	for(int i = 0; str[i] != '\0' ; i++){
		if(str[i]!=' ' && str[i] != '\t' && str[i] != '\n' ){
			tokens[c][j] = str[i];
			j++;
		}
		else if(!( (str[i] == ' ' || str[i] == '\t') && str[i+1] == '\0')){
			tokens[c][j] = '\0';
			j = 0;
			c++;
		}
	}
	tokens[c][j] = '\0';
	*n = c;
}

void pipeTokenize(char * str, char tokens[50][100], int* n){
	int c = 0;											//separate tokenization when it comes to pipes!!
	int j = 0;
	for(int i = 0; str[i] != '\0' ; i++){
		if(str[i]!='|' && str[i+1] != '|'){
			tokens[c][j] = str[i];
			j++;
		}
		else if(str[i] == '|'){
			tokens[c][j] = '\0';
			j = 0;
			c+=1;										//since there will be a space after | (i.e pipe) 
			i++;
		}
	}
	tokens[c][j] = '\0';
	*n = c;
}

void errormessage(char* error){
    printf("\033[0;36m");
    printf("\n%s\n",error);
    printf("\033[0m"); //Resets the text to default color
}

int findintokens(char * find, char cmd[][25], int numoftokens){
	for(int i = 0 ; i <= numoftokens; i++){
		if(strcmp(find, cmd[i]) == 0) return i;
	}
	return -1;
}

void StdOutOverWrite(char * command, char cmd[][25], int* numoftokens){
	char* check = strchr(command, '>');										//check if we need to do output redirection
	if(check != NULL){
		int indfilename;

		indfilename = findintokens(">", cmd, *numoftokens);
		indfilename += 1;
		
		int fileopencheck = 1;
		int outputfile = open(cmd[indfilename], O_WRONLY | O_CREAT, 0644 );	//open in write only mode, create if not created
		if(outputfile < 0) {
 		    errormessage("------ERROR OPENING OUTPUT FILE-------\n");
			fileopencheck = 0;
    	}
		if(fileopencheck){
			dup2(outputfile, 1);											//overwrite stdout
		}
	}
}

void StdInOverWrite(char * command, char cmd[][25], int* numoftokens){
	char *check = strchr(command, '<');											//we need to do input redirection
	if(check != NULL){
		int indfilename;
		indfilename = findintokens("<", cmd, *numoftokens);
		indfilename += 1;

		int fileopencheck = 1;
		int inputfile = open(cmd[indfilename], O_RDONLY);
		if(inputfile < 0) {
     		errormessage("------ERROR OPENING INPUT FILE-------\n");
        	fileopencheck = 0;
    	}
		if(fileopencheck){
			dup2(inputfile, 0);
		}
	}
}

void SIGINT_Handler(int sig_num) {
    signal(SIGINT, SIGINT_Handler); 
    printf("\n BONUS PART : SIGINT is ignored - can not use CTRL+C to end program enter exit to terminate gbsh shell\n"); 
    fflush(stdout); 
}
