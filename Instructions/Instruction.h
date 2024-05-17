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
    char operation[5] ;
    char firstOp[4] ; // Reg
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
typedef struct {
    unsigned char opcode;
    unsigned char operand1;
    unsigned char operand2;
    char immSigned;
    uint8_t reg1;
    uint8_t reg2;
} DecodedInstruction;

int checkBit(int x, int i){
    return (x & (1<<i)) != 0;
    // x = 11001001
    //     01000000
}
/**
 * decodes the instruction given into the four fields
 * opcode, operand1, operand2, immSigned
 * @param instruction the instruction to be decoded
 * @return the decoded instruction in a struct
 */
DecodedInstruction *decodeInstruction(uint16_t instruction){
    DecodedInstruction *decoded = malloc(sizeof(DecodedInstruction ));
    decoded->opcode = (instruction >> 12) & 0x000F;
    decoded->operand1 = (instruction >> 6) & 0x003F;
    decoded->operand2 = instruction & 0x003F;
    decoded->immSigned = (char) decoded->operand2;


    if(checkBit(decoded->operand2, 5) != 0)
        decoded->immSigned = (char) (decoded->operand2 | 0b11000000);
    return decoded;
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
