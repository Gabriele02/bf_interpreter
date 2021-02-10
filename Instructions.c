#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instructions.h"
#include "Errors.h"

void InstructionsInit(compiled_code_t *compiledCode){
    compiledCode->length = 0;
    compiledCode->instructions = calloc(MAX_INST_SIZE, sizeof(instruction_t));
    int i;
    for(i = 0; i < MAX_INST_SIZE; i++){
        compiledCode->instructions[i].instruction = INSTRUCTION_UNDEFINED;
    }
}

uint8_t charToInst(char inst){
    switch (inst){
        case '+': return INSTRUCTION_INC_CELL;   break;
        case '-': return INSTRUCTION_DEC_CELL;   break;
        case '>': return INSTRUCTION_INC_MEM;    break;
        case '<': return INSTRUCTION_DEC_MEM;    break;
        case '[': return INSTRUCTION_START_LOOP; break;
        case ']': return INSTRUCTION_END_LOOP;   break;
        case ',': return INSTRUCTION_INPUT;      break;
        case '.': return INSTRUCTION_OUTPUT;     break;
    }
}

inline void copyToCompiled(compiled_code_t* compiled,  uint16_t repetitions, char inst){
    instruction_t instruction;
    instruction.times = (uint8_t)(repetitions - 1);
    instruction.instruction = charToInst(inst);
    addInstruction(compiled, instruction);
}

inline void addInstruction(compiled_code_t *compiledCode, instruction_t inst){
    compiledCode->instructions[compiledCode->length++] = inst;
}

void resetCode(compiled_code_t *compiledCode){
    uint32_t i;
    for(i = 0; i < compiledCode->length; i++){
        while(compiledCode->instructions[i].instruction != INSTRUCTION_UNDEFINED){
            compiledCode->instructions[i].times = 0;
            compiledCode->instructions[i].instruction = INSTRUCTION_NOP;
        }
        compiledCode->length = 0;
    }
}

void freeCode(compiled_code_t* compiledCode){
    uint32_t i;
    for(i = 0; i < compiledCode->length; i++){
        compiledCode->length = 0;
    }
    free(compiledCode->instructions);
}

char* codeToString(const compiled_code_t *compiledCode){
    char *str = calloc(compiledCode->length * 15, sizeof(char));//Worst case
    uint32_t i;
    uint32_t str_i = 0;
    
    for(i = 0; i < compiledCode->length; i++){
        if(compiledCode->instructions[i].instruction != INSTRUCTION_UNDEFINED){
            uint32_t start_str_i = str_i;
            sprintf(str + str_i, "t: %d, i: %d\n", compiledCode->instructions[i].times, compiledCode->instructions[i].instruction);
            str_i += 15; //Worst case
            while(start_str_i < str_i){
                if(str[start_str_i] == '\0'){
                    str[start_str_i] = ' ';                    
                }
                start_str_i++;
            }
        }
    }
    str[str_i] = 0;
    return str;
}

uint8_t writeCodeToFile(const compiled_code_t *compiledCode, const char *fileName){
    if(fileName == NULL || strlen(fileName) <= 0){
        printf("FileName can not be empty or NULL!\n");
        return GENERAL_ERROR;
    }

    uint32_t code_len = compiledCode->length;
    if(code_len <= 0){return 0;} //Nothing to write

    FILE *ptr;
    ptr = fopen(fileName, "wb");
    if(!ptr){
        printf("Unable to open file: %s\n", fileName);
        return FILE_NOT_FOUND;
    }

    int wrote = fwrite((compiledCode->instructions), sizeof(instruction_t), code_len, ptr);    
    fclose(ptr);   
    if(code_len - wrote){//Should always be 0
        printf("Error while writing file: %s\n", fileName);
        return GENERAL_ERROR;
    }
    return 0;
}


uint8_t readCodeFromFile(compiled_code_t *compiledCode, const char *fileName){
    if(fileName == NULL || strlen(fileName) <= 0){
        printf("FileName can not be empty or NULL!\n");
        return GENERAL_ERROR;
    }

    FILE *ptr;
    ptr = fopen(fileName, "rb");
    if(!ptr){
        printf("Unable to open file: %s\n", fileName);
        return FILE_NOT_FOUND;
    }

    InstructionsInit(compiledCode);

    while(fread(compiledCode->instructions + compiledCode->length, sizeof(instruction_t), 1, ptr)){compiledCode->length++;};
    fclose(ptr);

    int i = 0;
    for(i = 0; i < compiledCode->length; i++){
        compiledCode->instructions[i].times++;
    }
    
    printf("length: %d, loaded code: %s", compiledCode->length, codeToString(compiledCode));
    return 0;
}
