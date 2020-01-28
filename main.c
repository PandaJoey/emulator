#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gameboyFunctions.h"

// typedef struct {
//   uint8_t A;
//   uint8_t B;
//   uint8_t C;
//   uint8_t D;
//   uint8_t E;
//   uint8_t F;
//   uint8_t H;
//   uint8_t L;
//   uint16_t PC;
//   uint16_t SP;
// } cpu_register;

// void ADD(uint8_t x);
// void LD(uint8_t *toregister, uint8_t *fromregister);

// uint8_t *memory;
// int memorysize = 0xFFFF;

cpu_register cpuRegister;


void LD(register8 *toregister, register8 *fromregister){
  toregister = &fromregister;
=======
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gameboyFunctions.h"

// typedef struct {
//   uint8_t A;
//   uint8_t B;
//   uint8_t C;
//   uint8_t D;
//   uint8_t E;
//   uint8_t F;
//   uint8_t H;
//   uint8_t L;
//   uint16_t PC;
//   uint16_t SP;
// } cpu_register;

// void ADD(uint8_t x);
// void LD(uint8_t *toregister, uint8_t *fromregister);

// uint8_t *memory;
// int memorysize = 0xFFFF;

cpu_register cpuRegister;

/* this function needs to not only load data from one register to another
   but also load data from register pairs and possibly register ranges
   and example in the book shows a memory location + a register to show a
   register range, this is more complex fucntion than we first thought.
*/ 
void LD(uint8_t *toRegister, uint8_t *fromRegister){
  toRegister = fromRegister;
>>>>>>> joe
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
  cpuRegister.A = 200;
  cpuRegister.A++;
  cpuRegister.B = 111;
<<<<<<< HEAD
  LD(cpuRegister.*A cpuRegister.*B);
  printf("%d", cpuRegister.A);
=======
 // uint8_t result = LD((*cpuRegister).A, (*cpuRegister).B);
 // printf("%d", result);
>>>>>>> joe
  return 0;
}
