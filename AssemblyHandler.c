
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

#include "Instructions/Instruction.h"
#include "Memory/InstructionMemory.h"
#include "Memory/DataMemory.h"

#define MAX_LINE_LENGTH 100
#define INSTRUCTION_SIZE_IN_BYTES 18


/**
 * Reading from assembly text file and store all instructions
 * and secondOperands to array of instruction
 * @return
 */
InstructionsArr* ReadAssemblyTextFile() {
    char fileName[] = "AssemblyProgramm.txt";
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

            // Assuming each line has three tokens: instruction, secondOperand1, secondOperand2
            if (token != NULL) {
                char instruction[4];
                char secondOperand1[6];
                char secondOperand2[6];

                // Copy the first token (instruction)
                strcpy(instruction, token);

                // Get the next token (secondOperand1)
                token = strtok(NULL, " \n");
                if (token != NULL) {
                    strcpy(secondOperand1, token);

                    // Get the third token (secondOperand2)
                    token = strtok(NULL, " \n");
                    if (token != NULL) {
                        strcpy(secondOperand2, token);
                        Instruction I;
                        //assigning the value of the instruction
                        strcpy(I.operation, instruction);
                        strcpy(I.firstOp, secondOperand1);
                        strcpy(I.secondOp, secondOperand2);
//                    printf("%s %s %s \n" ,I.operation, I.firstOp ,I.secondOp ) ;
                        InstructionArrWrite(IArr, I, instructionsArrIdx);
                        ++instructionsArrIdx;
                        // Print or process the parsed instruction and secondOperands
//                    printf("Instruction: %s, Operand1: %s, Operand2: %s\n", instruction, secondOperand1, secondOperand2);
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
//        IArr = realloc(IArr, INSTRUCTION_SIZE_IN_BYTES * numberOfBytesNeeded);
        fclose(file);
        return IArr;
    }
}


// Function to decode operation opcode to 4-bit value
unsigned char decodeOperation(char *opcode) {

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

unsigned char decodeFirstOperand(const char *firstOperand) {
    int number = atoi(firstOperand + 1);
    if (number > 63) {
        fprintf(stderr, "Error:Register R%d Doesn't exist.\n" , number);
        exit(EXIT_FAILURE);
    }
    return (unsigned char)(number & 0x3F); // Mask the number to fit within 6 bits
}

unsigned char decodeImmediate(const char *value) {
    int number = atoi(value + 1); // Skip the '#' character
    if (number > 63) {
        fprintf(stderr, "Error: Immediate value out of range.\n");
        exit(EXIT_FAILURE);
    }
    return (unsigned char)(number & 0x3F);
}
unsigned char decodeSecondOperand(const char *secondOperand) {
    if(secondOperand[0] == 'R'){ // Register
        return decodeFirstOperand(secondOperand) ;
    } else if (secondOperand[0] == '#') { // Immediate value
        return decodeImmediate(secondOperand);
    }else if (secondOperand[0] == '(') {
        char addressOperand[20];
        sscanf(secondOperand, "(%[^)])", addressOperand);
        int addressValue = atoi(addressOperand);
        if (addressValue > 63) {
            fprintf(stderr, "Error: Address value out of range.\n");
            exit(EXIT_FAILURE);
        }
        return (unsigned char)(addressValue & 0x3F);
    }else {
        fprintf(stderr, "Error: Invalid secondOperand format.\n");
        exit(EXIT_FAILURE);
    }
}

uint16_t decodeOneInstruction(Instruction i){
    unsigned char opcode =decodeOperation(i.operation); // 4 bits
    printf("%i\n",opcode) ;
    printf("%s\n",i.operation) ;
    unsigned char firstOpr = decodeFirstOperand(i.firstOp); // 6 bits
    unsigned char secondOpr = decodeSecondOperand(i.secondOp); // 6 bits
    uint16_t instruction = 0;
    instruction |= (opcode & 0x0F) << 12;     // Shift opcode to the most significant bits
    instruction |= (firstOpr & 0x3F) << 6;    // Shift first operand into position
    instruction |= (secondOpr & 0x3F);         // Include second operand as it is
    return instruction;
}


InstructionMemory* DecodeAllInstructions(InstructionsArr* instArray , InstructionMemory * mem){
    int length = sizeof(instArray->Instructions) / sizeof(instArray->Instructions[0]);
    for (int i = 0; i < length; ++i) {
        IMWrite(mem, i,decodeOneInstruction(instArray->Instructions[i]))  ;
    }
}


int main(){
    InstructionsArr* IArr ;
    InstructionMemory * Imem ;
    DataMemory* Dmem ;
    IMInit(Imem) ;
    DMInit(Dmem);
    IArr = ReadAssemblyTextFile() ;
    DecodeAllInstructions(IArr,Imem) ;
//    IMPrint(Imem) ;
    InstructionArrPrint(IArr) ;
    free(IArr) ;
}





