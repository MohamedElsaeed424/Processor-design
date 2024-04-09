//
// Created by Mahmoud on 4/9/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Instructions/Instruction.h"
#define MAX_LINE_LENGTH 100
#define INSTRUCTION_SIZE_IN_BYTES 18

InstructionsArr* ReadAssemblyTextFile (){
    char fileName []= "E:\\Processor-design\\AssemblyProgramm.txt" ;
    FILE *file = fopen( fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    int instructionsArrIdx = 0 ;
    //allocating the memory size to be max size of instruction which is 18 byte
    //and the maximum number of instructions which is 10 bits approx 2 bytes
    InstructionsArr* IArr = calloc(1024,INSTRUCTION_SIZE_IN_BYTES);

    char line[MAX_LINE_LENGTH];

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
                    Instruction I  ;
                    //assigning the value of the instruction
                    strcpy(I.operation, instruction);
                    strcpy(I.firstOp, operand1);
                    strcpy(I.secondOp, operand2);
//                    printf("%s %s %s \n" ,I.operation, I.firstOp ,I.secondOp ) ;
                    InstructionArrWrite(IArr,I,instructionsArrIdx);
                    ++instructionsArrIdx ;
                    // Print or process the parsed instruction and operands
//                    printf("Instruction: %s, Operand1: %s, Operand2: %s\n", instruction, operand1, operand2);
                } else {
                    printf( "Error: Invalid format in line\n");
                }
            } else {
                printf("Error: Invalid format in line\n");
            }
        } else {
            printf( "Error: Empty line\n");
        }
    }
    int numberOfInstructions = instructionsArrIdx + 1 ;
    //getting number of bytes needed to store all instructions
    int numberOfBytesNeeded = (int) ceil(log2(numberOfInstructions)/8) ;
//    printf("%i" ,numberOfBytesNeeded) ;
//    InstructionsArr * tmp = calloc()
    IArr = realloc(IArr , INSTRUCTION_SIZE_IN_BYTES*numberOfBytesNeeded ) ;
    fclose(file);
    return IArr ;
}

uint16_t decodeInstruction (){

}

