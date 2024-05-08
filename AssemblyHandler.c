
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
InstructionMemory * Imem ;
PC *pc;
GPRs *gprs;
DataMemory *Dmem;
SREG *sreg;

int clock = 1;
int numOfInstructions = 0  ;
uint16_t *fetched = NULL ;
DecodedInstruction* decoded;

// function prototypes
int add(uint8_t operand1, uint8_t operand2);
int sub(uint8_t operand1, uint8_t operand2);
int mul(uint8_t operand1, uint8_t operand2);
int ldi(uint8_t operand1, uint8_t imm);
int beqz(uint8_t operand1, uint8_t imm);
int and(uint8_t operand1, uint8_t operand2);
int or(uint8_t operand1, uint8_t operand2);
int jr(uint8_t operand1, uint8_t operand2);

int slc(uint8_t operand1, uint8_t imm);
int src(uint8_t operand1, uint8_t imm);
int lb(uint8_t operand1, uint8_t address);
int sb(uint8_t operand1, uint8_t address);

int (*opFuncs[])(uint8_t, uint8_t) = { add,  sub,  mul, ldi, beqz, and, or,
                                        jr, slc, src, lb, sb};
/**
 * Reading from assembly text file and store all instructions
 * and secondOperands to array of instruction
 * @return
 */
void ReadAssemblyTextFile() {
    char fileName[] = "AssemblyProgramm.txt";
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        IArr = NULL;
    }
    int instructionsArrIdx = 0;
    //allocating the memory size to be max size of instruction which is 18 byte
    //and the maximum number of instructions which is 10 bits approx 2 bytes
    IArr = calloc(1, sizeof(InstructionsArr) );

    char line[MAX_LINE_LENGTH];
    if (IArr == NULL) {
        printf("Memory allocation failure");
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
        numOfInstructions = numberOfInstructions ;// TODO: numofintruction habal
        fclose(file);
    }
}


// Function to decode operation opcode to 4-bit value
unsigned char decodeOperation(char *opcode) {

    if (strcmp(opcode, "ADD") == 0) {
        return 0b0000;
    } else if (strcmp(opcode, "SUB") == 0) {
        return 0b0001;
    } else if (strcmp(opcode,"MUL") == 0) {
        return 0b0010;
    } else if (strcmp(opcode,"LDI") == 0) {
        return 0b0011;
    } else if (strcmp(opcode,"BEQZ") == 0) {
        return 0b0100;
    } else if (strcmp(opcode,"AND") == 0) {
        return 0b0101;
    } else if (strcmp(opcode,"OR") == 0) {
        return 0b0110;
    } else if (strcmp(opcode,"JR") == 0) {
        return 0b0111;
    } else if (strcmp(opcode,"SLC") == 0) {
        return 0b1000;
    } else if (strcmp(opcode,"SRC") == 0) {
        return 0b1001;
    } else if (strcmp(opcode,"LB") == 0) {
        return 0b1010;
    } else if (strcmp(opcode,"SB") == 0) {
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
    unsigned char opcode = decodeOperation(i.operation); // 4 bits
    unsigned char firstOpr = decodeFirstOperand(i.firstOp); // 6 bits
    unsigned char secondOpr = decodeSecondOperand(i.secondOp); // 6 bits
    printf("%i\n",opcode) ;
    printf("%s\n",i.operation);
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
int fetch(){
    fetched = &Imem->Imemory[pc->address++];
    printf("fetched 0x%x\n", *fetched);
    if(pc->address >= INSTRUCTION_MEM_SIZE)
        *fetched = -1;
    if(*fetched == 0)
        return 2;
    else
        return 2044;
}
void decode(){
    if(fetched){
        decoded = decodeInstruction(*fetched);
        printf("decoded into opcode: %d, operand1: %d, operand2/immediate: %d\n",
               decoded->opcode, decoded->operand1, decoded->operand2);
    }
}
int execute(){
    if(decoded){
        if(decoded->opcode > 11)
            return -1;
        return opFuncs[decoded->opcode] (decoded->operand1, decoded->operand2);
    }
}


void init(){
    ReadAssemblyTextFile() ;
    IMInit(&Imem) ;
    DecodeAllInstructions(IArr,Imem) ;
    PCInit(&pc);
    GPRsInit(&gprs);
    DMInit(&Dmem);
    SregInit(&sreg);
}
void terminate(){
    free(IArr) ;
    free(Imem) ;
    free(pc) ;
    free(gprs) ;
    free(Dmem) ;
    free(sreg);
    free(decoded);
}
int main(){

    init();
    int next = 1044;

    while(1){
        printf("Clock Cycle %d\n", clock);
        printf("PC: %d\n", pc->address);
        printf("inst: %x\n", Imem->Imemory[pc->address]);
        int status = execute();
        // status 1 means there was a jump
        if(status == 1){
            clock++;
            continue;
        } else if(status == -1){
            // status -1 means pc is out of range or terminate
            break;
        }

        decode();
        next = fetch();
        if(clock > 24)
            break;
        clock++;
        printf("\n");
    }

    IMPrint(Imem);
    GPRsPrint(gprs);
    printf("PC = %d\n", pc->address);
    printStatus(sreg);
    terminate();
}
/**
 * updates N, S and Z flags of the status registers
 */
void updateNZ(int res){
    sreg->N = checkBit(res, 7);
    sreg->Z = res == 0;
    printStatus(sreg);
}
/**
 * Performs the addition instruction (ie. ADD R1 R2).
 * Adds register R2 to register R1
 * @param operand1 the first register in the instruction
 * @param operand2 the second register in the instruction
 */
int add(uint8_t operand1, uint8_t operand2){
    printf("adding R%d to R%d\n", operand2, operand1);
    int result = gprs->GPRegisters[operand1] + gprs->GPRegisters[operand2];
    printf("Result is %d\n", result);
    int posOp1 = checkBit(gprs->GPRegisters[operand1], 7);
    int posOp2 = checkBit(gprs->GPRegisters[operand2], 7);
    int posRes = checkBit(result, 7);

    sreg->C = checkBit(result, 8);
    sreg->V = posOp1 == posOp2 && posRes != posOp2;
    sreg->S = sreg->N ^ sreg->V;
    updateNZ(result);

    GPRsWrite(gprs, operand1, result);
    return 0;
}
/**
 * Performs the subtraction instruction (ie. SUB R1 R2).
 * subtracts register R2 from register R1
 * @param operand1 the first register in the instruction
 * @param operand2 the second register in the instruction
 */
int sub(uint8_t operand1, uint8_t operand2){
    printf("subtracting R%d from R%d\n", operand2, operand1);
    int result = gprs->GPRegisters[operand1] - gprs->GPRegisters[operand2];
    printf("Result is %d\n", result);
    int posOp1 = checkBit(gprs->GPRegisters[operand1], 7);
    int posOp2 = checkBit(gprs->GPRegisters[operand2], 7);
    int posRes = checkBit(result, 7);

    sreg->V = posOp1 != posOp2 && posRes == posOp2 ;
    sreg->S = sreg->N ^ sreg->V;
    updateNZ(result);

    GPRsWrite(gprs, operand1, result);
    return 0;

}
/**
 * Performs the multiplication instruction (ie. MUL R1 R2).
 * Multiplies register R2 to register R1
 * @param operand1 the first register in the instruction
 * @param operand2 the second register in the instruction
 */
int mul(uint8_t operand1, uint8_t operand2){
    printf("multiplying R%d to R%d\n", operand1, operand2);
    int result = gprs->GPRegisters[operand1] * gprs->GPRegisters[operand2];
    printf("Result is %d\n", result);
    
    updateNZ(result);

    GPRsWrite(gprs, operand1, result);
    return 0;

}
/**
 * Performs the LDI instruction
 * loads an immediate value into the specified register
 * @param operand1 the register to be loaded
 * @param imm the value to load
 */
int ldi(uint8_t operand1, uint8_t imm){
    printf("loading value %d into R%d\n", imm, operand1);

    GPRsWrite(gprs, operand1, imm);
    return 0;

}
/**
 * Performs the BEQZ instruction
 * if the register contains 0 then jumps relative the the current pc
 * @param operand1 the register to be checked
 * @param imm the value to jump
 */
// TODO: pc value correct?
int beqz(uint8_t operand1, uint8_t imm){
    printf("checking if R%d = 0\n", operand1);
    if(gprs->GPRegisters[operand1] == 0){
        pc->address += imm-1;
        if(pc->address >= INSTRUCTION_MEM_SIZE){
            printf("PC out of instruction memory range\n");
            return -1;
        }
        printf("branching to %d \n", pc->address);
        // reset fetched and decoded because they will not be executed
        fetched = NULL;
        free(decoded);
        decoded = NULL;
        return 1;
    }
    printf("no branch\n");
    return 0;
}
/**
 * Performs the AND instruction
 * Does a bitwise and on two registers and stores
 * @param operand1
 * @param operand2
 */
int and(uint8_t operand1, uint8_t operand2){
    printf("and-ing  R%d and R%d\n", operand1, operand2);
    int result = gprs->GPRegisters[operand1] & gprs->GPRegisters[operand2];
    printf("Result is %d\n", result);
    
    updateNZ(result);
    GPRsWrite(gprs, operand1, result);
    return 0;

}
int or(uint8_t operand1, uint8_t operand2){
    printf("or-ing  R%d and R%d\n", operand1, operand2);
    int result = gprs->GPRegisters[operand1] | gprs->GPRegisters[operand2];
    printf("Result is %d", result);

    updateNZ(result);
    GPRsWrite(gprs, operand1, result);
    return 0;

}

int jr(uint8_t operand1, uint8_t operand2){
    pc->address = (gprs->GPRegisters[operand1] << 8) | gprs->GPRegisters[operand2];
    if(pc->address >= INSTRUCTION_MEM_SIZE){
        printf("PC out of instruction memory range after jump\n");
        return -1;
    }

    printf("jumping to %d\n", pc->address);
    // reset fetched and decoded because they will not be executed
    fetched = NULL;
    free(decoded);
    decoded = NULL;
    return 1;
}
int slc(uint8_t operand1, uint8_t imm){
    printf("Circular shift left R%d by %d\n", operand1, imm);
    // TODO: unsigned shift to be tested
    int result = (gprs->GPRegisters[operand1] << imm) |
            ((gprs->GPRegisters[operand1] >> (8-imm))/* & ((1<<imm) -1)*/ );
    printf("Result is %d\n", result);

    updateNZ(result);
    GPRsWrite(gprs, operand1, result);
    return 0;
}

int src(uint8_t operand1, uint8_t imm){
    printf("Circular shift right R%d by %d\n", operand1, imm);
    int result = (gprs->GPRegisters[operand1] >> imm) |
                                  (gprs->GPRegisters[operand1] << (8-imm));
    printf("Result is %d\n", result);

    updateNZ(result);
    GPRsWrite(gprs, operand1, result);
    return 0;
}
int lb(uint8_t operand1, uint8_t address){
    printf("loading byte %d from memory address %d into R%d\n", Dmem->Dmemory[address], address, operand1);
    GPRsWrite(gprs, operand1, Dmem->Dmemory[address]);
    return 0;
}
int sb(uint8_t operand1, uint8_t address){
    printf("storing byte %d from R%d into memory address %d\n", gprs->GPRegisters[operand1], address, operand1);
    Dmem->Dmemory[address] = gprs->GPRegisters[operand1];
    return 0;
}



