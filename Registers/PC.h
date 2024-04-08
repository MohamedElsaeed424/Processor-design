#ifndef PROCESSOR_DESIGN_PC_H
#define PROCESSOR_DESIGN_PC_H

#include <stdio.h>
#define PC_SIZE 16

typedef struct PC{
    uint16_t address ;
};

/**
 * Store Address of Next Instruction in PC
 * @param pc : refrence to Program Counter Register
 * @param instAddress  : The address of the instruction to be stored in PC
 */
void storeNextInstruction(PC *pc ,unit16_t instAddress){
    pc-> address = instAddress ;
}

/**
 * This function used in BEQZ Instruction
 * @param pc :refrence to Program Counter Register
 * @param value : value that PC will jump with Imm
 */
void jumpPC(PC *pc , unit8_t value){
    pc->address = pc->address +1+value ;
}






#endif //PROCESSOR_DESIGN_PC_H
