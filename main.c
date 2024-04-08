#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Instructions/Instruction.h"

#define MAX_LINE_LENGTH 100

struct InstructionsArr* fetch (){
    FILE *file = fopen("E:\\Processor-design\\AssemblyProgramm.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    int instructionsArrIdx = 0 ;
    InstructionsArr IArr ;

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

                    IArr.Instructions[instructionsArrIdx] = I ;
                    // Print or process the parsed instruction and operands
                    printf("Instruction: %s, Operand1: %s, Operand2: %s\n", instruction, operand1, operand2);
                } else {
                    fprintf(stderr, "Error: Invalid format in line\n");
                }
            } else {
                fprintf(stderr, "Error: Invalid format in line\n");
            }
        } else {
            fprintf(stderr, "Error: Empty line\n");
        }
    }
    fclose(file);
    return IArr ;
}

int main(){
    fetch() ;
}
