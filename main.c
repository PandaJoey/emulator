#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gameboyFunctions.h"

cpu_register cpuRegister;

// Left shift first register 8 bits and adds second register
uint16_t get_AF() { return (cpuRegister.A <<= 8) + cpuRegister.F; }
uint16_t get_BC() { return (cpuRegister.B <<= 8) + cpuRegister.C; }
uint16_t get_DE() { return (cpuRegister.D <<= 8) + cpuRegister.E; }
uint16_t get_HL() { return (cpuRegister.H <<= 8) + cpuRegister.L; }

/* this function needs to not only load data from one register to another
   but also load data from register pairs and possibly register ranges
   and example in the book shows a memory location + a register to show a
   register range, this is more complex fucntion than we first thought.
*/ 
void LD(register8 *toregister, register8 *fromregister){
  toregister = &fromregister;
}

void ADD(uint8_t x) {
  cpuRegister.A += x;
}

void SUB(uint8_t x) {
  cpuRegister.A -= x;
}

void RLCA(cpu_register A, cpu_register F) {
    cpuRegister.A <<= 1;
    // correct?? check docs
}

// RLA goes here

void RRCA(cpu_register A, cpu_register F) {
    cpuRegister.A >>= 1;
    // correct? check docs
}

// RRA goes here

void AND(cpu_register A, uint8_t x) {
    cpuRegister.A &= x;
}

void OR(cpu_register A, uint8_t x) {
    cpuRegister.A |= x;
}

void XOR(cpu_register A, uint8_t x) {
    cpuRegister.A ^= x;
}

int main() {
  memory = (uint8_t *) malloc(memorysize * sizeof(uint8_t));
  return 0;
}
