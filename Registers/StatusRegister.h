#ifndef PROCESSOR_DESIGN_STATUSREGISTER_H
#define PROCESSOR_DESIGN_STATUSREGISTER_H
#include <stdio.h>
#include <stdint.h>

typedef struct  {
    unsigned char flags; // to represent all flags in one byte
    static const unsigned char C_FLAG = 0b00000001; // Carry Flag
    static const unsigned char V_FLAG = 0b00000010; // Two's Complement Overflow Flag
    static const unsigned char N_FLAG = 0b00000100; // Negative Flag
    static const unsigned char S_FLAG = 0b00001000; // Sign Flag
    static const unsigned char Z_FLAG = 0b00010000; // Zero Flag
}SREG;

/**
 * Initialize Status Register
 * @param sreg : pointer to the Status Register
 */
void sreg_init(SREG *sreg) {
    sreg->flags = 0;
}

/**
 * Set Carry Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the carry flag (0 or 1)
 */
void setC_FLAG(SREG *sreg, uint8_t value) {
    if (value)
        sreg->flags |= SREG::C_FLAG; // bitwise OR
    else
        sreg->flags &= ~SREG::C_FLAG; // bitwise AND ~: not
}

/**
 * Set Two's Complement Overflow Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the two's complement overflow flag (0 or 1)
 */
void setV_FLAG(SREG *sreg, uint8_t value) {
    if (value)
        sreg->flags |= SREG::V_FLAG;
    else
        sreg->flags &= ~SREG::V_FLAG;
}

/**
 * Set Negative Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the negative flag (0 or 1)
 */
void setN_FLAG(SREG *sreg, uint8_t value) {
    if (value)
        sreg->flags |= SREG::N_FLAG;
    else
        sreg->flags &= ~SREG::N_FLAG;
}

/**
 * Set Sign Flag in Status Register
 * @param sreg : pointer to the Status Register
 */
void setS_FLAG(SREG *sreg) {
    if ((sreg->flags & SREG::N_FLAG) ^ (sreg->flags & SREG::V_FLAG))
        sreg->flags |= SREG::S_FLAG;
    else
        sreg->flags &= ~SREG::S_FLAG;
}

/**
 * Set Zero Flag in Status Register
 * @param sreg : pointer to the Status Register
 * @param value : value to set the zero flag (0 or 1)
 */
void setZ_FLAG(SREG *sreg, uint8_t value) {
    if (value)
        sreg->flags |= SREG::Z_FLAG;
    else
        sreg->flags &= ~SREG::Z_FLAG;
}




#endif //PROCESSOR_DESIGN_STATUSREGISTER_H
