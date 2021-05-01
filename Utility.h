#include"build.h"

//--------------------------------------------PROTOTYPES OF UTILITY FUNCTIONS-----------------------------------------------------
void ChangeDirectory(char * command);
void lsImplementation();
void Environment(char* );
void setorunsetEnv(char*);
void ExecuteProgram(char*, char[][25], int);
void Built_In_GBSH(char*, char [][25], int );
void Execute_External_GBSH(char cmd[][25], int);