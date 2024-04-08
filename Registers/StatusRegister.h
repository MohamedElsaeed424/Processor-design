#ifndef PROCESSOR_DESIGN_STATUSREGISTER_H
#define PROCESSOR_DESIGN_STATUSREGISTER_H


struct SREG {
    unsigned char flags; // to represent all flags in one byte
    static unsigned char C_FLAG = 0b00000001; // Carry Flag
    static unsigned char V_FLAG = 0b00000010; // Two's Complement Overflow Flag
    static unsigned char N_FLAG = 0b00000100; // Negative Flag
    static unsigned char S_FLAG = 0b00001000; // Sign Flag
    static unsigned char Z_FLAG = 0b00010000; // Zero Flag
};

#endif //PROCESSOR_DESIGN_STATUSREGISTER_H
