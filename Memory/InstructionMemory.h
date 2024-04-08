#ifndef PROCESSOR_DESIGN_INSTRUCTIONMEMORY_H
#define PROCESSOR_DESIGN_INSTRUCTIONMEMORY_H

#include <stdio.h>
#define WORD_SIZE 16
#define INSTRUCTION_MEM_SIZE 1024

struct Word {
    uint16_t size ;
};

// Array of words
struct InstructionMemory {
    Word Imemory[INSTRUCTION_MEM_SIZE];
};

/**
 * Initializing IM with values
 * @param mem : refrence to the IM
 */
void IMInit(InstructionMemory *mem){
    for (int i = 0; i < INSTRUCTION_MEM_SIZE ; ++i) {
        mem->Imemory[i] = 0 ;
    }
}

/**
 * Reading from IM
 * @param mem : refrence to the IM
 * @param address : address is 16 bit because INSTRUCTION_MEM_SIZE =1024 So
 *                  log(1024) =10 --> 16
 */
uint16_t IMRead(InstructionMemory *mem, uint16_t address){
    if(address >= INSTRUCTION_MEM_SIZE){
        printf("This Address not in Instruction Memory range of Addresses") ;
        return -1 ;
    }
    return mem->Imemory[address];
}
/**
 * Writing To IM
 * @param mem : refrence to the IM
 * @param address : address is 16 bit because INSTRUCTION_MEM_SIZE =1024 So
 *                  log(1024) =10 --> 16
 * @param data : data is 16 bit because WORD_SIZE = 16
 */
void IMWrite(InstructionMemory *mem, uint16_t address, uint16_t data){
    if(address >= INSTRUCTION_MEM_SIZE){
        printf("This Address not in Instruction Memory range of Addresses") ;
        return  ;
    }
    mem->Imemory[address] = data ;
}


#endif //PROCESSOR_DESIGN_INSTRUCTIONMEMORY_H