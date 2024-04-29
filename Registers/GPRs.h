#ifndef PROCESSOR_DESIGN_GPRS_H
#define PROCESSOR_DESIGN_GPRS_H
// address here should be 6 bits not 8 ????
#include <stdio.h>
#include <stdint.h>
#define GPR_SIZE 8
#define GPRS_SIZE 64

// Only one GPR
//typedef struct {
//    uint8_t size ;
//}GPR;

// Array of GPRs
typedef struct {
    uint8_t GPRegisters[GPRS_SIZE];
}GPRs;

/**
 * Initializing GPRs with values
 * @param gprs : refrence to the GPRs
 */
void GPRsInit(GPRs **gprs){
    *gprs = calloc(1, sizeof(GPRs));
}

/**
 * Reading from GPRs
 * @param gprs : refrence to the GPRs
 * @param address : address is 6 bits because GPRS_SIZE =64 So
 *                  log(64) =6 --> 8
 */
uint16_t GPRsRead(GPRs *gprs, uint8_t address){
    if(address >= GPRS_SIZE){
        printf("This Address not in GPRs range of Addresses");
        return -1 ;
    }
    printf("reading from R%d",address);
    return gprs->GPRegisters[address];
}


/**
 * Writing To GPRs
 * @param gprs : refrence to the GPRs
 * @param address : address is 6 bits because GPRS_SIZE =64 So
 *                  log(64) =6 --> 8
 * @param data : data is 8 bit because GPRS_SIZE = 8
 */

void GPRsWrite(GPRs *gprs, uint8_t address, uint8_t data){

    if(address >= GPRS_SIZE){
        printf("This Address not in GPRs range of Addresses");
        return ;
    }
    gprs->GPRegisters[address] = data ;
    printf("writing %d to R%d",data, address);
}


#endif //PROCESSOR_DESIGN_GPRS_H
