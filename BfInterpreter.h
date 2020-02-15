#ifndef _BFINTERPRETER
#define BFINTERPRETER

#define MAX_INST_SIZE 30000
#define MEM_SIZE 30000
#define EXECUTION_OK 1
#define EXECUTION_END 0
#define UNKNOWN_COMMAND -1
#define MISSING_OPEN_BRACKET -2
#define MISSING_CLOSE_BRACKET -3
#define FILE_NOT_FOUND -3

int  chk_exit    (const char*, size_t);
void printMem    ();
int  debug       (const char*, size_t);
int  findClosing (const char*, size_t, int);
int  execute     (const char*);
void purify      (char *);
#endif
