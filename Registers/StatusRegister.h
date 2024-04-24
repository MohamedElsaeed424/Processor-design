#ifndef PROCESSOR_DESIGN_STATUSREGISTER_H
#define PROCESSOR_DESIGN_STATUSREGISTER_H
#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>

typedef struct  {
    uint8_t C : 1 ;
    uint8_t V : 1 ;
    uint8_t N : 1 ;
    uint8_t S : 1 ;
    uint8_t Z : 1 ;
    uint8_t rest : 3 ;
} SREG;

/**
 * Initialize Status Register
 * @param sreg : pointer to the Status Register
 */
void SregInit(SREG **sreg) {
    *sreg = calloc(1, sizeof(SREG));
}

/**
 * Set Carry Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the carry flag (0 or 1)
 */
void setC_FLAG(SREG *sreg, uint8_t value) {
    sreg->C = value;
}

/**
 * Set Two's Complement Overflow Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the two's complement overflow flag (0 or 1)
 */
void setV_FLAG(SREG *sreg, uint8_t value) {
    sreg->V = value;
}

/**
 * Set Negative Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the negative flag (0 or 1)
 */
void setN_FLAG(SREG *sreg, uint8_t value) {
    sreg->N = value;
}

/**
 * Set Sign Flag in Status Register
 * @param sreg : pointer to the Status Register
 */
void setS_FLAG(SREG *sreg) {
    sreg->S = (sreg->N ^ sreg->V);
}

/**
 * Set Zero Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the zero flag (0 or 1)
 */
void setZ_FLAG(SREG *sreg, uint8_t value) {
    sreg->Z = value;
}



#endif //PROCESSOR_DESIGN_STATUSREGISTER_H
