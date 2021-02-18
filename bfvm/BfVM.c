#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "../DynamicArray.h"
#include "BfVM.h"

unsigned char mem[MEM_SIZE] = {'\0'}; /*0 to 255*/
long int memIndex = 0;
char last_exec = '\0';

int main(int argc, char **argv){
    if(argc <= 1){
        printf("Too few arguments");
        return TOO_FEW_ARGS;
    }else{
        compiled_code_t *code = calloc(1, sizeof(compiled_code_t));
        if(!code){
            printf("Unable to allocate memory: tryed to allocate %d bytes", 1 * sizeof(compiled_code_t));
            return GENERAL_ERROR;
        }

        readCodeFromFile(code, argv[1]);
        
        if(code->length <= 0){return EXECUTION_END;}
        #ifdef DEBUGGING_MODE
        clock_t start = clock();
        #endif

        int ret_code = execute(code);

        #ifdef DEBUGGING_MODE
        clock_t end = clock();
        double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Elapsed CPU time: %f", elapsed_time);
        #endif
        /*
        1) 16.6
        2) 16.6
        3) 16.7
        */
        return ret_code;
    }
    return EXECUTION_END;
}

/*
Executes the instructions given as input
I: completeInst instructions to execute
Return:
    EXECUTION_OK ----------- if everything went well
    EXECUTION_END ---------- if an exit command is detected
    UNKNOWN_COMMAND -------- if an unknown command is detected
    MISSING_OPEN_BRACKET --- if an opening bracket is not found
    MISSING_CLOSE_BRACKET -- if a closing bracket is not found
*/
int execute(const compiled_code_t *code){
    instruction_t inst;
    long long instIndex = 0;
    DynamicArray openB;
    DynamicArrayInit(&openB, -1);
    long depth = 0;
    while(instIndex < code->length){
        inst = code->instructions[instIndex];
        switch(inst.instruction){
            default:
                break;
            case INSTRUCTION_INC_CELL:
                mem[memIndex] += inst.times;
                break;
            case INSTRUCTION_DEC_CELL:
                mem[memIndex] -= inst.times;
                break;
            case INSTRUCTION_DEC_MEM:
                memIndex -= inst.times;
                break;
            case INSTRUCTION_INC_MEM:
                memIndex += inst.times;
                break;
            case INSTRUCTION_OUTPUT:
                putchar((char)mem[memIndex]);
                break;
            case INSTRUCTION_INPUT:
                scanf(" %c", (char *)&mem[memIndex]);
                break;
            case INSTRUCTION_START_LOOP:
                if(mem[memIndex] == 0){
                    instIndex = findClosing(code, instIndex);
                     if(instIndex < 0 || instIndex >= code->length){
                        printf("] expected!!");
                        return MISSING_CLOSE_BRACKET;
                    }
                }else{
                    if(instIndex < 0){
                        printf("] expected!!");
                        return MISSING_CLOSE_BRACKET;
                    }
                    DynamicArrayAdd(&openB, depth++, instIndex);
                }
                break;
            case INSTRUCTION_END_LOOP:
                if(mem[memIndex] != 0){
                    if(!DynamicArrayGet(&openB, depth - 1, &instIndex)){
                        printf("[ expected!!");
                        return MISSING_OPEN_BRACKET;
                    }
                }else{
                    depth--;
                    if(depth < 0){
                        printf("[ expected!!");
                        return MISSING_OPEN_BRACKET;
                    }
                }
                break;
        }
        instIndex++;
    }
    freeDynamicArray(&openB);
    return EXECUTION_OK;
}

/*
compares the input string against "exit"
I: completeInst string to compare
I: instLen completeInst length
Return: 1 if the input string matches "exit", 0 otherwise
*//*
int chk_exit(const char *completeInst, size_t instLen){
    return instLen >= 4 && !strncmp(completeInst, "exit", 4);
}*/

/*
Compares the input sting against all the debug commands
I: completeInst string to compare
I: instLen completeInst length
Return: 1 if the input string is one of the debug commands, 0 otherwise
*//*
int debug(const char *debugCommand, size_t instLen){
    if(strcmp(debugCommand, "printChar") == 0){
        printf("%c\n", (char)mem[memIndex]);
        return true;
    }else if(strcmp(debugCommand, "print") == 0){
        printf("%d\n", mem[memIndex]);
        return true;
    }else if(strcmp(debugCommand, "printPtr") == 0){
        printf("%d\n", (int)memIndex);
        return true;
    }else if(strcmp(debugCommand, "printMem") == 0){
        printMem();
        return true;
    }
    return false;
}*/

/*
Prints memory's content at +/- 5 from the memory pointer
*/
void printMem(){
    int s = (int)memIndex - 5;
    int e = (int)memIndex + 5;
    s = s > 0 ? s : 0;
    e = e > 0 ? e : 0;
    int i;
    for(i = s ; i < e; i++){
        printf("(%d) [%d]", i, mem[i]);
        if(i == memIndex)
            printf(" <--");
        printf("\n");
    }
}

/*
Finds the corresponding closing bracket index
I: text text in which  to search for the bracket
I: textLen length of text
I: openPos position of the opening bracket
return: Index of the closing bracket, if found, MISSING_CLOSE_BRACKET otherwise
*/
int findClosing(const compiled_code_t *code, int openPos) {
    uint32_t codeLen = code->length;
    int closePos = openPos;
    int counter = 1;
    while (counter > 0) {
        if(++closePos >= codeLen){
            return MISSING_CLOSE_BRACKET;
        }
        uint8_t inst = code->instructions[closePos].instruction;
        if (inst == INSTRUCTION_START_LOOP) {
            counter++;
        }else if (inst == INSTRUCTION_END_LOOP) {
            counter--;
        }
    }
    return closePos;
}
