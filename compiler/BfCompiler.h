#ifndef _BFINTERPRETER
#define BFINTERPRETER

#include <stdint.h>
#include "../Instructions.h"
#include "../Errors.h"

#define MEM_SIZE 30000

int     findClosing     (const char *, size_t, int);
void    calc_memIndex   (char, int*);
void    chk_memIndex    (int);
int     chk_exit        (const char*, size_t);
int     compile         (const char*, compiled_code_t*);
void    purify          (char *);
#endif
