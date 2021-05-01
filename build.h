#include<unistd.h>
#include<limits.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<dirent.h>
#include <fcntl.h>

//------------------------------------------------PROTOTYPES OF BUILD FUNCTIONS------------------------------------------------------
void StartingMessage();
void EndingMessage();
void PromptDisplay();
void Tokenize(char * str, char tokens[150][25], int* n);
void SetShellEnvironment();
void errormessage(char* error);
void StdOutOverWrite(char * command, char cmd[][25], int* numoftokens);
void StdInOverWrite(char * command, char cmd[][25], int* numoftokens);
int findintokens(char * find, char cmd[][25], int);
void pipeTokenize(char * str, char tokens[50][100], int* n);
void SIGINT_Handler(int) ;