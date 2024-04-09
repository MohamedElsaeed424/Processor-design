
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

#include "Instructions/Instruction.h"
#include "Memory/InstructionMemory.h"

#define MAX_LINE_LENGTH 100
#define INSTRUCTION_SIZE_IN_BYTES 18


/**
 * Reading from assembly text file and store all instructions
 * and operands to array of instruction
 * @return
 */
InstructionsArr *ReadAssemblyTextFile() {
    char fileName[] = "E:\\Processor-design\\AssemblyProgramm.txt";
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    int instructionsArrIdx = 0;
    //allocating the memory size to be max size of instruction which is 18 byte
    //and the maximum number of instructions which is 10 bits approx 2 bytes
    InstructionsArr *IArr = calloc(1024, INSTRUCTION_SIZE_IN_BYTES);

    char line[MAX_LINE_LENGTH];
    if (IArr == NULL) {
        printf("Memory allocation failure");
        return NULL;
    } else {
        // Read lines from the file until EOF is reached
        while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
            // Tokenize the line using strtok
            char *token = strtok(line, " \n");

            // Assuming each line has three tokens: instruction, operand1, operand2
            if (token != NULL) {
                char instruction[4];
                char operand1[6];
                char operand2[6];

                // Copy the first token (instruction)
                strcpy(instruction, token);

                // Get the next token (operand1)
                token = strtok(NULL, " \n");
                if (token != NULL) {
                    strcpy(operand1, token);

                    // Get the third token (operand2)
                    token = strtok(NULL, " \n");
                    if (token != NULL) {
                        strcpy(operand2, token);
                        Instruction I;
                        //assigning the value of the instruction
                        strcpy(I.operation, instruction);
                        strcpy(I.firstOp, operand1);
                        strcpy(I.secondOp, operand2);
//                    printf("%s %s %s \n" ,I.operation, I.firstOp ,I.secondOp ) ;
                        InstructionArrWrite(IArr, I, instructionsArrIdx);
                        ++instructionsArrIdx;
                        // Print or process the parsed instruction and operands
//                    printf("Instruction: %s, Operand1: %s, Operand2: %s\n", instruction, operand1, operand2);
                    } else {
                        printf("Error: Invalid format in line\n");
                    }
                } else {
                    printf("Error: Invalid format in line\n");
                }
            } else {
                printf("Error: Empty line\n");
            }
        }
        int numberOfInstructions = instructionsArrIdx + 1;
        //getting number of bytes needed to store all instructions
        int numberOfBytesNeeded = (int) ceil(log2(numberOfInstructions) / 8);
//    printf("%i" ,numberOfBytesNeeded) ;
//    InstructionsArr * tmp = calloc()
        IArr = realloc(IArr, INSTRUCTION_SIZE_IN_BYTES * numberOfBytesNeeded);
        fclose(file);
        return IArr;
    }
}


// Function to decode operation opcode to 4-bit value
unsigned char decodeOperation(const char *opcode) {
    if (strcmp(opcode, "ADD") == 0) {
        return 0b0000;
    } else if (strcmp(opcode, "SUB") == 0) {
        return 0b0001;
    } else if (strcmp(opcode, "MUL") == 0) {
        return 0b0010;
    } else if (strcmp(opcode, "LDI") == 0) {
        return 0b0011;
    } else if (strcmp(opcode, "BEQZ") == 0) {
        return 0b0100;
    } else if (strcmp(opcode, "AND") == 0) {
        return 0b0101;
    } else if (strcmp(opcode, "OR") == 0) {
        return 0b0110;
    } else if (strcmp(opcode, "JR") == 0) {
        return 0b0111;
    } else if (strcmp(opcode, "SLC") == 0) {
        return 0b1000;
    } else if (strcmp(opcode, "SRC") == 0) {
        return 0b1001;
    } else if (strcmp(opcode, "LB") == 0) {
        return 0b1010;
    } else if (strcmp(opcode, "SB") == 0) {
        return 0b1011;
    } else {
        printf("This Opcode not exist in out ISA");
        return 0xFF; // 0xFF as an error value
    }
}

decode

uint16_t decodeOneInstruction(Instruction i){
    unsigned char opcode =decodeOperation(i.operation); // 4 bits
    decodeFirstOperand(i.firstOp); // 6 bits

    decodeSecoundOperand(i.secondOp); // 6 bits
}


// get instructions from instructions array  -> in instruction.h
// based on the fields will decode its binary values  ex: ADD 0000 , R0 , R1
// note : each instruction should be decoded to 16 bit
// save each 16 bit instruction (one word in IM)in the Instruction Memory
uint16_t decodeInstruction() {
    InstructionsArr *Instructions = ReadAssemblyTextFile() ;

}

char InstructionFormat (char opcode[]){

InstructionMemory* DecodeAllInstructions(InstructionsArr instArray , InstructionMemory * mem){
    int length = sizeof(instArray.Instructions) / sizeof(instArray.Instructions[0]);
    for (int i = 0; i < length; ++i) {
        IMWrite(mem, i,decodeOneInstruction(instArray.Instructions[i]))  ;
    }
}





