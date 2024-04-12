//
// Created by Mahmoud on 4/8/2024.
//
#include <stdio.h>
#include <stdint.h>
#ifndef PROCESSOR_DESIGN_INSTRUCTION_H
#define PROCESSOR_DESIGN_INSTRUCTION_H
#endif //PROCESSOR_DESIGN_INSTRUCTION_H
#define INSTRUCTION_MEM_SIZE 1024

typedef struct {
    char operation[4] ;
    char firstOp[3] ; // Reg
    char secondOp[6] ; // Reg or IMM or Address
}Instruction;


typedef struct {
    Instruction Instructions[INSTRUCTION_MEM_SIZE] ;
}InstructionsArr;



void InstructionArrWrite(InstructionsArr* Arr , Instruction I , uint16_t addres ) {
    Arr->Instructions[addres] = I ;
};

void InstructionArrPrint (InstructionsArr* Arr){
    for (int i = 0; i < INSTRUCTION_MEM_SIZE  ; ++i) {
        Instruction  I = Arr->Instructions[i] ;
        printf("operation: %s ,first operand: %s,second operand %s \n" ,I.operation,I.firstOp,I.secondOp) ;
    }
}

//void decode(Instruction* i , ){
//    switch (i->operation) {
//        case "ADD":
//
//            break;
//        case "SUB":
//            break;
//        case "MUL":
//            break;
//        case "LDI":
//            break;
//        case "BEQZ":
//            break;
//        case "AND":
//            break;
//        case "OR":
//            break;
//        case "JR":
//            break;
//        case "SLC":
//            break;
//        case "SRC":
//            break;
//        case "LB":
//            break;
//        case "SB":
//            break;
//    }
//}
