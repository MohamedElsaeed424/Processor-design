#ifndef PROCESSOR_DESIGN_DATAMEMORY_H
#define PROCESSOR_DESIGN_DATAMEMORY_H

#include <stdio.h>
#include <stdint.h>
#define WORD_SIZE 8
#define DATA_MEM_SIZE 2024

//typedef struct {
//    uint8_t size ;
//}Word;

// Array of words
typedef struct  {
    uint8_t Dmemory[DATA_MEM_SIZE];
}DataMemory;

/**
 * Initializing DM with values
 * @param mem : refrence to the DM
 */
void DMInit(DataMemory *mem){
    for (int i = 0; i < DATA_MEM_SIZE ; ++i) {
        mem->Dmemory[i] = 0 ;
    }
}
/**
 * Reading from DM
 * @param mem : refrence to the DM
 * @param address : address is 16 bit because DATA_MEM_SIZE =2024 So
 *                  log(2024) =10.9 --> 16
 */
uint16_t DMRead(DataMemory *mem, uint16_t address){
    if(address >= DATA_MEM_SIZE){
        printf("This Address not in Data Memory range of Addresses");
        return -1 ;
    }
    return mem->Dmemory[address];
}
/**
 * Writing To DM
 * @param mem : refrence to the DM
 * @param address : address is 16 bit because DATA_MEM_SIZE =2024 So
 *                  log(2024) =10.9 --> 16
 * @param data : data is 8 bit because WORD_SIZE = 8
 */
void DMWrite(DataMemory *mem, uint16_t address, uint16_t data){
    if(address >= DATA_MEM_SIZE){
        printf("This Address not in Data Memory range of Addresses");
        return  ;
    }
    mem->Dmemory[address] = data ;
}



#endif //PROCESSOR_DESIGN_DATAMEMORY_H
