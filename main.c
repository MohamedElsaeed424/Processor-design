
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

/// Colors for UNIX Systems
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

InstructionsArr *IArr ;
InstructionMemory *Imem ;
PC *pc;
GPRs *gprs;
DataMemory *Dmem;
SREG *sreg;

int clock = 1;
int numOfInstructions = 0  ;

/// IF/ID
uint16_t *fetched = NULL ;
/// ID/EX
DecodedInstruction* decoded = NULL;
/// Output from EX
int regUpdating = -1;
int result;

// function prototypes of instructions
int add();
int sub();
int mul();
int ldi();
int beqz();
int and();
int or();
int jr();

int slc();
int src();
int lb();
int sb();

int (*opFuncs[])() = { add,  sub,  mul, ldi, beqz, and, or,
                                        jr, slc, src, lb, sb};
/// ----------------------------------- start Assembly Parsing -----------------------------------
/**
 * Reading from assembly text file and store all instructions
 * and secondOperands to array of instruction
 * @return
 */
void ReadAssemblyTextFile(char fileName[]) {
//    char fileName[] = "AssemblyProgramm.txt";
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
                        InstructionArrWrite(IArr, I, instructionsArrIdx);
                        ++instructionsArrIdx;
                        // Print or process the parsed instruction and secondOperands
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
        numOfInstructions = numberOfInstructions-1 ;
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
        fprintf(stderr,"This instruction %s not exist in out ISA\n", opcode);
        exit(EXIT_FAILURE);
//        return 0xFF; // 0xFF as an error value
    }
}

unsigned char decodeFirstOperand(const char *firstOperand) {
    int number = atoi(firstOperand + 1);
    if (number > 63) {
        fprintf(stderr, "Error:Register R%d Doesn't exist.\n" , number);
        exit(EXIT_FAILURE);
    }
    return (unsigned char) number; // Mask the number to fit within 6 bits
}

unsigned char decodeImmediate(const char *value, int isSigned) {
    int number = atoi(value); // Skip the '#' character
    int range1 = isSigned? -32: 0;
    int range2 = isSigned? 31: 63;

    if (number < range1 || number > range2) {
        fprintf(stderr, "Error: Immediate value out of range.\n");
        exit(EXIT_FAILURE);
    }
    return (unsigned char) number;
}
unsigned char decodeSecondOperand(const char *secondOperand, int isSigned) {
    if(secondOperand[0] == 'R'){ // Register
        return decodeFirstOperand(secondOperand) ;
    } else if ((secondOperand[0] >= '0' && secondOperand[0] <= '9') || secondOperand[0] == '-' ) { // Immediate value
        return decodeImmediate(secondOperand, isSigned);
    } else {
        fprintf(stderr, "Error: Invalid secondOperand format.\n");
        exit(EXIT_FAILURE);
    }
}

uint16_t decodeOneInstruction(Instruction i){
    unsigned char opcode = decodeOperation(i.operation); // 4 bits
    int isSigned = opcode <  0b1000;
    unsigned char firstOpr = decodeFirstOperand(i.firstOp); // 6 bits
    unsigned char secondOpr = decodeSecondOperand(i.secondOp, isSigned); // 6 bits
    printf("%i\n",opcode) ;
    printf("%s\n",i.operation);
    uint16_t instruction = 0;
    instruction |= (opcode & 0x0F) << 12;     // Shift opcode to the most significant bits
    instruction |= (firstOpr & 0x3F) << 6;    // Shift first operand into position
    instruction |= (secondOpr & 0x3F);        // Include second operand as it is
    return instruction;
}

/** read instruction from Imem
// for each (16 bit)
//      divide these 16 bit to 4 6 6 bits
//      We have to determine the operation cause we will do the following steps based on it
//      this will affect how we will retrieve the operands and where we will store our result .
//      for first 6 bits (first operand) note : will be always a Register
//          Access the gpr with these 6 bits  000001001 -> gpr.GPRegisters[9] and save the first operand in var 1
//      for second 6 bits note : maybe reg or memory
//          based on the 4 bits of the operation will determine
//      for the 4 bits . do the operation based on these 4 bits  0000 -> +
 **/
void DecodeAllInstructions(InstructionsArr* instArray , InstructionMemory * mem){
    int length = sizeof(instArray->Instructions) / sizeof(instArray->Instructions[0]);
    for (int i = 0; i < numOfInstructions; ++i) {
        IMWrite(mem, i,decodeOneInstruction(instArray->Instructions[i]))  ;
    }
    free(IArr) ;
}
/// -------------------------end Assembly parsing-----------------------------------------

/**
 * function that handles data hazards by forwarding the result
 * @param reg the register being read
 * @return either the result of execution or the register value
 */
uint8_t data_hazard(uint8_t reg){
    if(regUpdating == reg)
        // forward result of execution
        return result;
    return gprs->GPRegisters[reg];
}
/**
 * fetches the instruction pointed to by PC
 * and increments PC
 */
void fetch(){
    fetched = &Imem->Imemory[pc->address++];
    printf( "fetched 0x%x\n", *fetched);
    if(pc->address >= INSTRUCTION_MEM_SIZE)
        *fetched = -1;
}
/**
 * decodes the instruction fetched in the previous stage
 * into four fields {opcode, operand1, operand2 and signed immediate)
 * and reads the required registers
 */
void decode(){
    if(fetched){
        decoded = decodeInstruction(*fetched);

        // read register values
        decoded->reg1 = data_hazard(decoded->operand1);
        decoded->reg2 = data_hazard(decoded->operand2);

        printf("decoded into opcode: %d, operand1: %d, operand2/immediate: %d signed imm: %d\n",
               decoded->opcode, decoded->operand1, decoded->operand2, decoded->immSigned);
    }
}
/**
 * executes the appropriate function according to the instruction
 * @return returns a status value. returns 1 if a branch occurred,
 * -1 if pc out of instruction range or when trying to execute an invalid instruction
 * and 0 is returned otherwise
 */
int execute(){
    if(decoded){
        if(decoded->opcode < 0 || decoded->opcode > 11)
            return -1;
        return opFuncs[decoded->opcode] ();
    }
    return 0;
}

/**
 * initializes the program by reading the assembly text file and allocating memory
 */
void init(char path[]){
    ReadAssemblyTextFile(path) ;
    IMInit(&Imem) ;
    DecodeAllInstructions(IArr,Imem) ;
    PCInit(&pc);
    GPRsInit(&gprs);
    DMInit(&Dmem);
    SregInit(&sreg);
}
/**
 * frees all allocated memory at the end of the program
 */
void terminate(){
    free(Imem) ;
    free(pc) ;
    free(gprs) ;
    free(Dmem) ;
    free(sreg);
    free(decoded);
}
int main(int argc, char *argv[]){
//    if(argc < 2){
//        printf("please input path of assembly program\n");
//        return 1;
//    }
    char programFile[100];
    printf("Enter the program file path: ");
    scanf("%s", programFile);


    init(programFile);
    printf("-----------------Start of execution--------------------\n");
    while(1){
        printf("Clock Cycle %d\n", clock);
        printf("PC: %d\n", pc->address);
        int status = execute();
        // status 1 means there was a jump
        if(status == 1){
            clock++;
            printf("\n");
            continue;
        } else if(status == -1){
        // status -1 means pc is out of range or terminate
            break;
        }

        decode();
        fetch();
        clock++;
        printf("\n");
    }
    printf("-----------------End of execution--------------------\n");
    IMPrint(Imem);
    DMPrint(Dmem);
    GPRsPrint(gprs);
    printf("PC = %d\n", pc->address);
    printStatus(sreg);

    terminate();
}
/**
 * updates N , S and Z flags of the status registers
 * and prints the status register
 */
void updateNZ(int res, int s){
    sreg->N = checkBit((char)res, 7);
    sreg->Z = ((char)res) == 0;
    if(s) sreg->S = sreg->N ^ sreg->V;
    printStatus(sreg);
}
void printRes(){
    printf("Result is %d\n", (char)result);
}
/**
 * Performs the addition instruction (ie. ADD R1 R2).
 * Adds register R2 to register R1
 * @return 0
 */
int add(){
    printf("adding R%d to R%d\n", decoded->operand2, decoded->operand1);
    regUpdating = decoded->operand1;
    result = decoded->reg1 + decoded->reg2;
    printRes();

    int posOp1 = checkBit(decoded->reg1, 7);
    int posOp2 = checkBit(decoded->reg2, 7);
    int posRes = checkBit(result, 7);

    sreg->C = checkBit(result, 8);
    sreg->V = posOp1 == posOp2 && posRes != posOp2;
    updateNZ(result, 1);

    GPRsWrite(gprs, decoded->operand1, result);
    return 0;
}
/**
 * Performs the subtraction instruction (ie. SUB R1 R2).
 * subtracts register R2 from register R1 and stores result in R1
 * @return 0
 */
int sub(){
    printf("subtracting R%d from R%d\n", decoded->operand2, decoded->operand1);
    regUpdating = decoded->operand1;
//    result = gprs->GPRegisters[operand1] - gprs->GPRegisters[operand2];
    result = decoded->reg1 - decoded->reg2;
    printRes();
    int posOp1 = checkBit(decoded->reg1, 7);
    int posOp2 = checkBit(decoded->reg2, 7);
    int posRes = checkBit(result, 7);

    sreg->V = posOp1 != posOp2 && posRes == posOp2;
    updateNZ(result, 1);

    GPRsWrite(gprs, decoded->operand1, result);
    return 0;

}
/**
 * Performs the multiplication instruction (ie. MUL R1 R2).
 * Multiplies register R2 to register R1
 * @return 0
 */
int mul(){
    printf("multiplying R%d into R%d\n", decoded->operand2, decoded->operand1);
    regUpdating = decoded->operand1;
//    result = gprs->GPRegisters[operand1] * gprs->GPRegisters[operand2];
    result = decoded->reg1 * decoded->reg2;
    printRes();

    updateNZ(result,0);

    GPRsWrite(gprs, decoded->operand1, result);
    return 0;

}
/**
 * Performs the LDI instruction
 * loads an immediate value into the specified register
 * @return 0
 */
int ldi(){
    printf("loading value %d into R%d\n", (char)decoded->immSigned, decoded->operand1);
    regUpdating = decoded->operand1;
    result = (int)decoded->immSigned;
    GPRsWrite(gprs, decoded->operand1, result);
    return 0;

}
/**
 * Performs the BEQZ instruction
 * if the register contains 0 then jumps relative the the current pc
 * @return returns -1 (terminate) if pc out of range after jumping
 * @return returns 1 if branch occurred
 * @return returns 0 if no branch occurred
 */
int beqz(){
    printf("checking if R%d = 0\n", decoded->operand1);
    regUpdating = -1;
    if(/*gprs->GPRegisters[operand1] */ decoded->reg1 == 0){
        pc->address += decoded->immSigned-1;
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
 * Does a bitwise and on two registers and stores the result
 * @return 0
 */
int and(){
    printf("and-ing  R%d and R%d\n", decoded->operand1, decoded->operand2);
    regUpdating = decoded->operand1;
//    result = gprs->GPRegisters[operand1] & gprs->GPRegisters[operand2];
    result = decoded->reg1 & decoded->reg2;
    printRes();

    updateNZ(result, 0);
    GPRsWrite(gprs, decoded->operand1, result);
    return 0;

}
/**
 * Performs the OR instruction
 * Does a bitwise or on two registers and stores the result
 * @return 0
 */
int or(){
    printf("or-ing  R%d and R%d\n", decoded->operand1, decoded->operand2);
    regUpdating = decoded->operand1;
//    result = gprs->GPRegisters[operand1] | gprs->GPRegisters[operand2];
    result = decoded->reg1 | decoded->reg2;
    printRes();

    updateNZ(result, 0);
    GPRsWrite(gprs, decoded->operand1, result);
    return 0;

}
/**
 * Performs the jump JR instruction
 * @return returns -1 if pc out of range and 1 otherwise
 */
int jr(){
    regUpdating = -1;
//    pc->address = (gprs->GPRegisters[operand1] << 8) | gprs->GPRegisters[operand2];
    pc->address = (decoded->reg1 << 8) | decoded->reg2;
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
/**
 * Performs the circular shift left SLC instruction
 * @return 0
 */
int slc(){
    printf("Circular shift left R%d by %d\n", decoded->operand1, decoded->operand2);
    regUpdating = decoded->operand1;

//    result = (gprs->GPRegisters[operand1] << imm) |
//            ((gprs->GPRegisters[operand1] >> (8-imm))/* & ((1<<imm) -1)*/ );
    result = (decoded->reg1 << decoded->operand2) | (decoded->reg1 >> (8-decoded->operand2));
    printRes();

    updateNZ(result, 0);
    GPRsWrite(gprs, decoded->operand1, result);
    return 0;
}
/**
 * Performs the circular shift right SRC instruction
 * @return 0
 */
int src(){
    printf("Circular shift right R%d by %d\n", decoded->operand1, decoded->operand2);
    regUpdating = decoded->operand1;
//    result = (gprs->GPRegisters[operand1] >> imm) |
//                                  (gprs->GPRegisters[operand1] << (8-imm));
    result = (decoded->reg1 >> decoded->operand2) | (decoded->reg1 << (8-decoded->operand2));
    printRes();

    updateNZ(result, 0);
    GPRsWrite(gprs, decoded->operand1, result);
    return 0;
}
/**
 * Performs the load byte LB instruction
 * @return 0
 */
int lb(){
    regUpdating = decoded->operand1;
    result = Dmem->Dmemory[decoded->operand2];
    printf("loading byte %d from memory address %d into R%d\n",
           (char)Dmem->Dmemory[decoded->operand2], decoded->operand2, decoded->operand1);

    GPRsWrite(gprs, decoded->operand1, result);
    return 0;
}
/**
 * Performs the store byte SB instruction
 * @return 0
 */
int sb(){
    regUpdating = -1;
    printf("storing byte %d from R%d into memory address %d\n",
           (char)gprs->GPRegisters[decoded->operand1], decoded->operand1, decoded->operand2);
    Dmem->Dmemory[decoded->operand2] = decoded->reg1;
    return 0;
}



