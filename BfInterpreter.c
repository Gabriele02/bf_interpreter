#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "DynamicArray.h"
#include "BfInterpreter.h"

int consoleMode = true;
unsigned char mem[MEM_SIZE] = {'\0'}; /*0 to 255*/
long int memIndex = 0;
char last_exec = '\0';

int main(int argc, char **argv){
    char *toExec;
    if(argc <= 1){
        int returnCode = 0;
        printf("BrainFuck Interpreter\n");
        toExec = calloc(MAX_INST_SIZE, sizeof(char));
        do{
            printf(last_exec == '.' || returnCode < 0 ? "\n# " : "# ");
            do{
                fgets(toExec, MAX_INST_SIZE , stdin);
                purify(toExec);
           }while(!toExec[0]);
            returnCode = execute(toExec);
            /*
            TODO:
                Add command to clear memory
            */
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
            toExec = calloc(length, sizeof(char));
            if(toExec){
                fread(toExec, 1, length, f);
            }
            fclose(f);
        }else{
            printf("Unable to find file: \"%s\"", argv[1]);
            return FILE_NOT_FOUND;
        }

        if(toExec){
            purify(toExec);
            /*if(argc >= 3){
                FILE *fp = fopen("intext.txt", "w");
                if (fp != NULL){
                    fputs(toExec, fp);
                    fclose(fp);
                }
            }*/
            return execute(toExec);
        }
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
int execute(const char *completeInst){
    size_t instLen = strlen(completeInst);
    if(consoleMode && chk_exit(completeInst, instLen)){return EXECUTION_END;}
    char inst;
    long long instIndex = 0;
    DynamicArray openB;
    DynamicArrayInit(&openB, -1);
    long depth = 0;
    while(instIndex < instLen){
        inst = completeInst[instIndex];
        switch(inst){
            default:
                if(consoleMode){
                    if(debug(completeInst, instLen)){return EXECUTION_OK;}
                    printf("Unknown comman: \"%s\"", completeInst);
                    return UNKNOWN_COMMAND;
                }
                break;
            case '+':
                mem[memIndex]++;
                break;
            case '-':
                mem[memIndex]--;
                break;
            case '<':
                memIndex--;
                break;
            case '>':
                memIndex++;
                break;
            case '.':
                putchar((char)mem[memIndex]);
                break;
            case ',':
                scanf(" %c", (char *)&mem[memIndex]);
                break;
            case '[':
                if(mem[memIndex] == 0){
                    instIndex = findClosing(completeInst, instLen, instIndex);
                     if(instIndex < 0 || instIndex >= instLen){
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
            case ']':
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
            case ' ':
            case '\0':
            case '\n':
                break;
        }
        if(consoleMode)
            last_exec = inst;
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
*/
int chk_exit(const char *completeInst, size_t instLen){
    return instLen >= 4 && !strncmp(completeInst, "exit", 4);
}

/*
Compares the input sting against all the debug commands
I: completeInst string to compare
I: instLen completeInst length
Return: 1 if the input string is one of the debug commands, 0 otherwise
*/
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
}

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
