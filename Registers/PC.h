#ifndef PROCESSOR_DESIGN_PC_H
#define PROCESSOR_DESIGN_PC_H

#include <stdio.h>
#include <stdint.h>
#define PC_SIZE 16

typedef struct {
    uint16_t address ;
}PC;

/**
 * Store Address of Next Instruction in PC
 * @param pc : refrence to Program Counter Register
 * @param instAddress  : The address of the instruction to be stored in PC
 */
void storeNextInstruction(PC *pc ,uint16_t instAddress){
    pc-> address = instAddress ;
}

/**
 * This function used in BEQZ Instruction
 * @param pc :refrence to Program Counter Register
 * @param value : value that PC will jump with Imm
 */
void jumpPC(PC *pc , uint8_t value){
    pc->address = pc->address +1+value ;
}






#endif //PROCESSOR_DESIGN_PC_H
