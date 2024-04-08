#ifndef PROCESSOR_DESIGN_GPRS_H
#define PROCESSOR_DESIGN_GPRS_H
// address here should be 6 bits not 8 ????
#include <stdio.h>
#define GPR_SIZE 8
#define GPRS_SIZE 64

// Only one GPR
typedef struct GPR{
    uint8_t size ;
};

// Array of GPRs
struct GPRs{
    GPR GPRegisters[GPRS_SIZE];
};

/**
 * Initializing GPRs with values
 * @param gprs : refrence to the GPRs
 */
void GPRsInit(GPRs *gprs){
    for (int i = 0; i < GPRS_SIZE ; ++i) {
        gprs->GPRegisters[i] = 0 ;
    }
}

/**
 * Reading from GPRs
 * @param gprs : refrence to the GPRs
 * @param address : address is 6 bits because GPRS_SIZE =64 So
 *                  log(64) =6 --> 8
 */
uint16_t GPRsRead(DGPRs *gprs, uint8_t address){
    if(address >= GPRS_SIZE){
        printf("This Address not in GPRs range of Addresses");
        return -1 ;
    }
    return gprs->GPRegisters[address];
}


/**
 * Writing To GPRs
 * @param gprs : refrence to the GPRs
 * @param address : address is 6 bits because GPRS_SIZE =64 So
 *                  log(64) =6 --> 8
 * @param data : data is 8 bit because GPRS_SIZE = 8
 */

void GPRsWrite(DGPRs *gprs, uint8_t address, uint8_t data){
    if(address >= GPRS_SIZE){
        printf("This Address not in GPRs range of Addresses");
        return ;
    }
    gprs->GPRegisters[address] = data ;
}


#endif //PROCESSOR_DESIGN_GPRS_H
