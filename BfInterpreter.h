#ifndef _BFINTERPRETER
#define _BFINTERPRETER

#include "Instructions.h"
#include "Errors.h"

#define MEM_SIZE 30000

int  chk_exit    (const char*, size_t);
void printMem    ();
int  debug       (const char*, size_t);
int  findClosing (const char*, size_t, int);
int  execute     (const char*);
void purify      (char *);
#endif
