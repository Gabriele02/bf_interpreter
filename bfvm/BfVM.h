#ifndef _BFINTERPRETER
#define BFINTERPRETER

#include "../Instructions.h"
#include "../Errors.h"

#define MEM_SIZE 30000

/*void printMem    ();*/
/*int  debug       (const char*, size_t);*/
int  findClosing (const compiled_code_t*, int);
int  execute     (const compiled_code_t*);
#endif
