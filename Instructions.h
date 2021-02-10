#include <stdint.h>
#ifndef _INSTRUCTIONS
#define _INSTRUCTIONS

#define MAX_INST_SIZE 30000

#define INSTRUCTION_INC_CELL    0
#define INSTRUCTION_DEC_CELL    1
#define INSTRUCTION_INC_MEM     2
#define INSTRUCTION_DEC_MEM     3
#define INSTRUCTION_START_LOOP  4
#define INSTRUCTION_END_LOOP    5
#define INSTRUCTION_INPUT       6
#define INSTRUCTION_OUTPUT      7
#define INSTRUCTION_NOP         8
#define INSTRUCTION_UNDEFINED   9
#define INSTRUCTION_BREAK      10

typedef struct {
    uint8_t times; //Numer of times the instruction should be repeated max 256 (0 is considered as 1)
    uint8_t instruction;
} instruction_t;

typedef struct {
    uint32_t length;
    instruction_t *instructions;
} compiled_code_t;

uint8_t charToInst       (char);
void    copyToCompiled   (compiled_code_t*, uint16_t, char);
void    InstructionsInit (compiled_code_t*);
void    addInstruction   (compiled_code_t*, instruction_t);
void    resetCode        (compiled_code_t*);
void    freeCode         (compiled_code_t*);
char *  codeToString     (const compiled_code_t*);
uint8_t writeCodeToFile  (const compiled_code_t*, const char*);
uint8_t readCodeFromFile (compiled_code_t*, const char*);
#endif
