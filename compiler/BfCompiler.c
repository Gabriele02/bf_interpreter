#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "BfCompiler.h"
#include "../DynamicArray.h"

int consoleMode = true;
char last_exec = '\0';

int main(int argc, char **argv){
    char *toCompile;
    compiled_code_t *compiledCode;
    if(argc <= 1){
        int returnCode = 0;
        printf("BrainFuck Compiler\n");
        toCompile = calloc(MAX_INST_SIZE, sizeof(char));
        compiledCode = calloc(1, sizeof(compiled_code_t));
        InstructionsInit(compiledCode);
        do{
            printf(last_exec == '.' || returnCode < 0 ? "\n# " : "# ");
            do{
                fgets(toCompile, MAX_INST_SIZE , stdin);
                purify(toCompile);
            }while(!toCompile[0]);
            returnCode = compile(toCompile, compiledCode);
            
            if(returnCode >= 0){
                char *compiled = codeToString(compiledCode);
                printf("%s\n", compiled);
                free(compiled);
            }
            //Always clear
            resetCode(compiledCode);
        }while(returnCode);/*continue until an exit command is detected*/
        return EXECUTION_END;
    }else{
        consoleMode = false;
        FILE *f = fopen(argv[1], "r");
        long length;
        if(f){
            fseek(f, 0, SEEK_END);
            length = ftell(f);
            fseek(f, 0, SEEK_SET);
            toCompile = calloc(length, sizeof(char));
            compiledCode = calloc(1, sizeof(compiled_code_t));
            InstructionsInit(compiledCode);
            if(toCompile && compiledCode){
                fread(toCompile, 1, length, f);
            }
            fclose(f);
        }else{
            printf("Unable to find file: \"%s\"", argv[1]);
            return FILE_NOT_FOUND;
        }

        if(toCompile){
            purify(toCompile);
            /*if(argc >= 3){
                FILE *fp = fopen("intext.txt", "w");
                if (fp != NULL){
                    fputs(toExec, fp);
                    fclose(fp);
                }
            }*/
            int ret = compile(toCompile, compiledCode);
            if(ret >= 0){
                writeCodeToFile(compiledCode, "compiled_code.bfc");
            }          
            return ret;
        }
    }
    free(toCompile);
    freeCode(compiledCode);
    return EXECUTION_END;
}

/*
Executes the instructions given as input
I: completeInst instructions to compile
Return:
    EXECUTION_OK ----------- if everything went well
    EXECUTION_END ---------- if an exit command is detected
    UNKNOWN_COMMAND -------- if an unknown command is detected
    MISSING_OPEN_BRACKET --- if an opening bracket is not found
    MISSING_CLOSE_BRACKET -- if a closing bracket is not found
*/
int compile(const char *completeInst, compiled_code_t *compiledCode){
    int memIndex = 0;
    size_t instLen = strlen(completeInst);
    if(consoleMode && chk_exit(completeInst, instLen)){return EXECUTION_END;}
    DynamicArray openB;
    DynamicArrayInit(&openB, -1);
    long depth = 0;
    char inst;
    long long instIndex = 0;
    long long compiledIndex = 0;
    while(instIndex < instLen){
        inst = completeInst[instIndex];
        switch(inst){
            default:
                if(consoleMode){
                    printf("Unknown comman: \"%s\"", completeInst);
                    return UNKNOWN_COMMAND;
                }
                break;
            case '+':
            case '-':
            case '>':
            case '<':{
                uint16_t reps = 1;
                //calc_memIndex(inst, &memIndex);
                while(instIndex < instLen - 1 && completeInst[instIndex + 1] == inst && reps < 256){
                    reps++;
                    instIndex++;
                    //calc_memIndex(inst, &memIndex);
                }
                //chk_memIndex(memIndex, instIndex);
                copyToCompiled(compiledCode, reps, inst);
                break;
            }
            case '.':
            case ',':
                copyToCompiled(compiledCode, 1, inst);
                break;
            case '[':{
                long long closingIndex = findClosing(completeInst, instLen, instIndex);
                    if(closingIndex < 0 || closingIndex >= instLen){
                    printf("] expected!!");
                    return MISSING_CLOSE_BRACKET;
                }
                if(closingIndex < 0){
                    printf("] expected!!");
                    return MISSING_CLOSE_BRACKET;
                }
                DynamicArrayAdd(&openB, depth++, instIndex);
                copyToCompiled(compiledCode, 1, inst);
                break;
            }
            case ']':{
                long long closingIndex;
                if(!DynamicArrayGet(&openB, depth - 1, &closingIndex)){
                    printf("[ expected!!");
                    return MISSING_OPEN_BRACKET;
                }
                depth--;
                if(depth < 0){
                    printf("[ expected!!");
                    return MISSING_OPEN_BRACKET;
                }
                copyToCompiled(compiledCode, 1, inst);
                break;
            }
            case ' ':
            case '\0':
            case '\n':
                break;
        }
        if(consoleMode)
            last_exec = inst;
        instIndex++;
    }
    //compiledCode[compiledIndex] = 0; //Add null terminator
    freeDynamicArray(&openB);
    return EXECUTION_OK;
}

/*
compares the input string against "exit"
I: completeInst string to compare
I: instLen completeInst length
Return: 1 if the input string matches "exit", 0 otherwise
*/
int chk_exit(const char *completeInst, size_t instLen){
    return instLen >= 4 && !strncmp(completeInst, "exit", 4);
}

/*
Removes from the string every character which is not in the BrainFuck instruction-set
O: cmd string to purify
*/
void purify(char *cmd){
    int i = 0;
    int len = strlen(cmd);
    while(i < len){
        if(consoleMode && (cmd[i] == '\n' || cmd[i] == ' ')){
            memmove(&cmd[i], &cmd[i + 1], len - i);
            len--;
        }else if(!consoleMode &&(
                 cmd[i] != '+' && cmd[i] != '-' &&
                 cmd[i] != '>' && cmd[i] != '<' &&
                 cmd[i] != '.' && cmd[i] != ',' &&
                 cmd[i] != '[' && cmd[i] != ']'))
        {
            memmove(&cmd[i], &cmd[i + 1], len - i);
            len--;
        }else{
            i++;
        }
    }
    cmd[len] = '\0';
}

/*
Finds the corresponding closing bracket index
I: text text in which  to search for the bracket
I: textLen length of text
I: openPos position of the opening bracket
return: Index of the closing bracket, if found, MISSING_CLOSE_BRACKET otherwise
*/
int findClosing(const char *text, size_t textLen, int openPos) {
    int closePos = openPos;
    int counter = 1;
    while (counter > 0) {
        if(++closePos >= textLen)
            return MISSING_CLOSE_BRACKET;
        char c = text[closePos];
        if (c == '[') {
            counter++;
        }else if (c == ']') {
            counter--;
        }
    }
    return closePos;
}

inline void calc_memIndex(char inst, int *memIndex){
     switch(inst){
        case '<':
            (*memIndex)--;
            break;
        case '>':
            (*memIndex)++;
            break;
    }
}
/*
inline void chk_memIndex(int memIndex, long long instIndex){
    if(memIndex < 0){
        printf("Waring: memory index (%d) is < than 0. Behaviour is undefined. Character %d\n", memIndex, instIndex);
    }else if(memIndex >= MEM_SIZE){
        printf("Waring: memory index (%d) is >= MEM_SIZE (%d). Behaviour is undefined. Character: %d\n", memIndex, MEM_SIZE, instIndex);
    }
}*/