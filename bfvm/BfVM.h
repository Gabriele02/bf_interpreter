#ifndef _BFINTERPRETER
#define BFINTERPRETER

#include "BracesCache.h"
#include "../Instructions.h"
#include "../Errors.h"

#define MEM_SIZE 30000
#define DEBUGGING_MODE 1

void printMem    ();
/*int  debug       (const char*, size_t);*/
int  findClosing (const compiled_code_t*, int, braces_cache_t*);
int  execute     (const compiled_code_t*);
#endif
