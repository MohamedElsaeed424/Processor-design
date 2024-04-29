
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

#include "Instructions/Instruction.h"
#include "Memory/InstructionMemory.h"
#include "Memory/DataMemory.h"
#include "Registers/GPRs.h"
#include "Registers/PC.h"
#include "Registers/StatusRegister.h"

#define MAX_LINE_LENGTH 100
#define INSTRUCTION_SIZE_IN_BYTES 18
InstructionsArr* IArr ;
InstructionMemory * Imem ; // TODO: initializing Imem with zeroes is incorrect because 0 is a valid instruction
PC *pc;
GPRs *gprs;
DataMemory *Dmem;
SREG *sreg;

int clock = 0;
int numOfInstructions = 0  ;
uint16_t *fetched = NULL ; // TODO: make null until next instruction is fetched
DecodedInstruction* decoded;

// function prototypes
void add(uint8_t operand1, uint8_t operand2);
void sub(uint8_t operand1, uint8_t operand2);
void mul(uint8_t operand1, uint8_t operand2);
void ldi(uint8_t operand1, uint8_t imm);
void beqz(uint8_t operand1, uint8_t imm);
void and(uint8_t operand1, uint8_t operand2);
void or(uint8_t operand1, uint8_t operand2);
void jr(uint8_t operand1, uint8_t operand2);

void slc(uint8_t operand1, uint8_t imm);
void src(uint8_t operand1, uint8_t imm);
void lb(uint8_t operand1, uint8_t address);
void sb(uint8_t operand1, uint8_t address);

void (*opFuncs[])(uint8_t, uint8_t) = { add,  sub,  mul, ldi, beqz, and, or,
                                        jr, slc, src, lb, sb};
/**
 * Reading from assembly text file and store all instructions
 * and secondOperands to array of instruction
 * @return
 */
void ReadAssemblyTextFile(InstructionsArr **IArr) {
    char fileName[] = "AssemblyProgramm.txt";
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        *IArr = NULL;
    }
    int instructionsArrIdx = 0;
    //allocating the memory size to be max size of instruction which is 18 byte
    //and the maximum number of instructions which is 10 bits approx 2 bytes
    *IArr = calloc(1024, INSTRUCTION_SIZE_IN_BYTES);

    char line[MAX_LINE_LENGTH];
    if (*IArr == NULL) {
        printf("Memory allocation failure");
        IArr = NULL;
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
                        InstructionArrWrite(*IArr, I, instructionsArrIdx);
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
        numOfInstructions = numberOfInstructions ;
        fclose(file);
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
        printf("This Opcode not exist in out ISA\n");
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
    unsigned char firstOpr = decodeFirstOperand(i.firstOp); // 6 bits
    unsigned char secondOpr = decodeSecondOperand(i.secondOp); // 6 bits
    printf("%i\n",opcode) ;
    printf("%s\n",i.operation) ;
    uint16_t instruction = 0;
    instruction |= (opcode & 0x0F) << 12;     // Shift opcode to the most significant bits
    instruction |= (firstOpr & 0x3F) << 6;    // Shift first operand into position
    instruction |= (secondOpr & 0x3F);        // Include second operand as it is
    return instruction;
}


void DecodeAllInstructions(InstructionsArr* instArray , InstructionMemory * mem){
    int length = sizeof(instArray->Instructions) / sizeof(instArray->Instructions[0]);
    for (int i = 0; i < numOfInstructions-1; ++i) {
        IMWrite(mem, i,decodeOneInstruction(instArray->Instructions[i]))  ;
    }
}

/** read instruction from Imem
// for each (16 bit)
//      devid these 16 bit to 4 6 6 bits
//      We have to determine the operation cause we will do the following steps based on it
//      this will affect how we will retrieve the operands and where we will store our result .
//      for first 6 bits (first operand) note : will be always a Register
//          Access the gpr with these 6 bits  000001001 -> gpr.GPRegisters[9] and save the first operand in var 1
//      for second 6 bits note : maybe reg or memory
//          based on the 4 bits of the operation will determine
//      for the 4 bits . do the operation based on these 4 bits  0000 -> +
 **/
void fetch(){
    fetched = &Imem->Imemory[pc->address++];
}
void decode(){
    if(fetched)
        decoded = decodeInstruction(*fetched);
}
void execute(){
    if(decoded){
        opFuncs[decoded->opcode] (decoded->operand1, decoded->operand2);
    }
}


void init(){
    ReadAssemblyTextFile(&IArr) ;
    IMInit(&Imem) ;
    DecodeAllInstructions(IArr,Imem) ;
    PCInit(&pc);
    GPRsInit(&gprs);
    DMInit(&Dmem);
    SregInit(&sreg);
}
void end(){
    free(IArr) ;
    free(Imem) ;
    free(pc) ;
    free(gprs) ;
    free(Dmem) ;
    free(sreg);
    free(decoded);
}
//int main(){
//
//    init();
//    printf("no instructions: %i", numOfInstructions);
//    // TODO: figure out time with jump instructions
//    for(clock = 0; clock < 3 + numOfInstructions - 1; clock++){
//        printf("clock cycle: %d\n", clock);
//        execute();
//        decode();
//        fetch();
//    }
//
//    IMPrint(Imem);
//
//    end();
//}
void updateNSZ(int res){
    sreg->N = checkBit(res, 7);
    sreg->S = sreg->N ^ sreg->V;
    sreg->Z = res == 0;
}

void add(uint8_t operand1, uint8_t operand2){
    printf("adding R%d to R%d\n", operand2, operand1);
    int result = gprs->GPRegisters[operand1] + gprs->GPRegisters[operand2];
    int posOp1 = checkBit(gprs->GPRegisters[operand1], 7);
    int posOp2 = checkBit(gprs->GPRegisters[operand2], 7);
    int posRes = checkBit(result, 7);

    sreg->V = posOp1 == posOp2 && posRes != posOp2;
    sreg->C = checkBit(result, 8);
    updateNSZ(result);
    gprs->GPRegisters[operand1] = result;
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
}
void sub(uint8_t operand1, uint8_t operand2){
    printf("subtracting R%d from R%d\n", operand2, operand1);
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
    int result = gprs->GPRegisters[operand1] - gprs->GPRegisters[operand2];
    int posOp1 = checkBit(gprs->GPRegisters[operand1], 7);
    int posOp2 = checkBit(gprs->GPRegisters[operand2], 7);
    int posRes = checkBit(result, 7);
    sreg->V = posOp1 != posOp2 && posRes == posOp2 ;
    updateNSZ(result);
    gprs->GPRegisters[operand1] = result;
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
}
void mul(uint8_t operand1, uint8_t operand2){
    printf("multiplying R%d to R%d\n", operand1, operand2);
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
    int result = gprs->GPRegisters[operand1] * gprs->GPRegisters[operand2];
    updateNSZ(result);
    gprs->GPRegisters[operand1] = result;
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
}
void ldi(uint8_t operand1, uint8_t imm){
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
    printf("loading value %d into R%d\n", imm, operand1);
    gprs->GPRegisters[operand1] = imm;
    printf("R%d after: %d, ", operand1, gprs->GPRegisters[operand1]);
}
// odd one. Don't know what to do during pipeline
// TODO: figure out what happens during pipeline
void beqz(uint8_t operand1, uint8_t imm){
    printf("checking if R%d = 0\n", operand1);
    if(gprs->GPRegisters[operand1] == 0){
        printf("branching to %d \n", pc->address + imm);
        pc->address += imm; // no need to add 1 because pc already incremented
        // reset fetched and decoded because they will not be executed
        fetched = NULL;
        free(decoded);
    }
    printf("no branch\n");
}
void and(uint8_t operand1, uint8_t operand2){
    printf("and-ing  R%d and R%d\n", operand1, operand2);
    int result = gprs->GPRegisters[operand1] & gprs->GPRegisters[operand2];
    updateNSZ(result);
    gprs->GPRegisters[operand1] = result;
}
void or(uint8_t operand1, uint8_t operand2){
    printf("or-ing  R%d and R%d\n", operand1, operand2);
    int result = gprs->GPRegisters[operand1] | gprs->GPRegisters[operand2];
    updateNSZ(result);
    gprs->GPRegisters[operand1] = result;
}
// odd one. Don't know what to do during pipeline
// TODO: figure out what happens during pipeline and if R1 || R2 is bigger than 1024
void jr(uint8_t operand1, uint8_t operand2){
    pc->address = (gprs->GPRegisters[operand1] << 8) | gprs->GPRegisters[operand2];
    printf("jumping to %d\n", pc->address);
    // reset fetched and decoded because they will not be executed
    fetched = NULL;
    free(decoded);
}
void slc(uint8_t operand1, uint8_t imm){
    printf("Circular shift left R%d by %d\n", operand1, imm);
    gprs->GPRegisters[operand1] = (gprs->GPRegisters[imm] << imm) |
            ((gprs->GPRegisters[imm] >> (8-imm))/* & ((1<<imm) -1)*/ );
}
// bit shift on unsigned type is unsigned
void src(uint8_t operand1, uint8_t imm){
    printf("Circular shift right R%d by %d\n", operand1, imm);
    gprs->GPRegisters[operand1] = (gprs->GPRegisters[imm] >> imm) |
                                  (gprs->GPRegisters[imm] << (8-imm));
}
void lb(uint8_t operand1, uint8_t address){
    printf("loading byte from memory address %d into R%d", address, operand1);
    gprs->GPRegisters[operand1] = Dmem->Dmemory[address];
}
void sb(uint8_t operand1, uint8_t address){
    printf("storing byte from R%d into memory address %d", address, operand1);
    Dmem->Dmemory[address] = gprs->GPRegisters[operand1];
}



