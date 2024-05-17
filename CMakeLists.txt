cmake_minimum_required(VERSION 3.27)
project(Processor_design C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(Memory)
include_directories(Registers)
include_directories(Instructions)

add_executable(
        Processor-design
        Memory/DataMemory.h
        Memory/InstructionMemory.h
        Registers/GPRs.h
        Registers/PC.h
        Registers/StatusRegister.h
        Registers/PC.h
        main.c
)
