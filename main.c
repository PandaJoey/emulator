#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gameboyFunctions.h"
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

cpu_register cpuRegister;


// Left shift first register 8 bits and adds second register
uint16_t get_AF() { return (cpuRegister.A << 8) + cpuRegister.F; }
uint16_t get_BC() { return (cpuRegister.B << 8) + cpuRegister.C; }
uint16_t get_DE() { return (cpuRegister.D << 8) + cpuRegister.E; }
uint16_t get_HL() { return (cpuRegister.H << 8) + cpuRegister.L; }

// Getters for flag registers
int get_CY() { return (cpuRegister.F >> 4) & 1; }
int get_H() { return (cpuRegister.F >> 5) & 1; }
int get_N() { return (cpuRegister.F >> 6) & 1; }
int get_Z() { return (cpuRegister.F >> 7) & 1; }

// Setters for flag registers
void set_CY(int x) {
  if (x == 1) { cpuRegister.F |= 16; } 
  else { cpuRegister.F &= ~(16); }
}

void set_H(int x) {
  if (x == 1) { cpuRegister.F |= 32; } 
  else { cpuRegister.F &= ~(32); }
}

void set_N(int x) {
  if (x == 1) { cpuRegister.F |= 64; } 
  else { cpuRegister.F &= ~(64); }
}

void set_Z(int x) {
  if (x == 1) { cpuRegister.F |= 128; } 
  else { cpuRegister.F &= ~(128); }
}


/* this function needs to not only load data from one register to another
   but also load data from register pairs and possibly register ranges
   and example in the book shows a memory location + a register to show a
   register range, this is more complex fucntion than we first thought.
*/ 
// void LD(register8_t *toregister, register8_t *fromregister){
//   toregister = &fromregister; 
// }

void ADD(uint8_t x) {
  cpuRegister.A += x;
}

void SUB(uint8_t x) {
  cpuRegister.A -= x;
}

void RLCA() {
    // Check for a carry from bit 7, set CY accordingly
    set_CY((cpuRegister.A >> 7) & 1);
    // Set other flag registers to 0
    set_N(0);
    set_Z(0);
    set_H(0);
    // Bitshift left by 1 and add bit 7 (old bit 6)
    cpuRegister.A = (cpuRegister.A << 1) + ((cpuRegister.A >> 6) & 1);
}

// RLA goes here
//TODO is this done? check and update params
void RLA() {
    // Check for a carry from bit 7, set CY accordingly
    set_CY((cpuRegister.A >> 7) & 1);
    // Set other flag registers to 0
    set_N(0);
    set_Z(0);
    set_H(0);
    // Left shift register A and carry over to bit 1
    cpuRegister.A = (cpuRegister.A << 1) + (cpuRegister.A >> 7);
}

void RRCA() {
    // Check for a carry from bit 7, set CY accordingly
    set_CY((cpuRegister.A >> 7) & 1);
    // Set other flag registers to 0
    set_N(0);
    set_Z(0);
    set_H(0);
    // Bitshift left by 1 and add bit 7 (old bit 6)
    cpuRegister.A = (cpuRegister.A >> 1) + ((cpuRegister.A & 1) << 7);
}

void RRA() {
    // Check for a carry from bit 7, set CY accordingly
    set_CY((cpuRegister.A >> 7) & 1);
    // Set other flag registers to 0
    set_N(0);
    set_Z(0);
    set_H(0);
    // Bitshift left by 1 and add bit 7 (old bit 6)
    cpuRegister.A >>= 1;
}

void AND(uint8_t x) {
    cpuRegister.A &= x;
}

void OR(uint8_t x) {
    cpuRegister.A |= x;
}

void XOR(uint8_t x) {
    cpuRegister.A ^= x;
}

int main() {
  memory = (uint8_t *) malloc(memorysize * sizeof(uint8_t));
  cpuRegister.A = 0x81;
  cpuRegister.F = 0;
  printf("value of A before RRCA %x\n", cpuRegister.A);
  printf("value of F before RRCA %x\n", cpuRegister.F);
  printf("First Print Value %d\n\n\n", get_AF());
  RRA();
  printf("value of A after RRCA %x\n", cpuRegister.A);
  printf("value of F after RRCA %x\n", cpuRegister.F);
  printf("Value of AF Pair after RLCA %d\n", get_AF());
  printf("CY is: %d\n", get_CY());

  // printf("First Print Value %d\n", get_AF());
  // ADD(5);
  // printf("Value after ADD %d\n", get_AF());
  // SUB(5);
  // printf("Value After SUB%d\n", get_AF());
  // RLCA(cpuRegister.A, cpuRegister.F);
  // printf("Value after RLCA %d\n", cpuRegister.A);
  // printf("Value after RLCA %d\n", cpuRegister.F);
  // printf("Value after RLCA %d\n", get_AF());
  return 0;
}
/*
-	Flag is not affected by this operation.
*	Flag is affected according to result of operation.
b	A bit number in any 8-bit register or memory location.
C	Carry flag.
cc	Flag condition code: C,NC,NZ,Z
d	Any 8-bit destination register or memory location.
dd	Any 16-bit destination register or memory location.
e	8-bit signed 2's complement displacement.
f	8 special call locations in page zero.
H	Half-carry flag.
N	Subtraction flag.
NC	Not carry flag
NZ	Not zero flag.
n	Any 8-bit binary number.
nn	Any 16-bit binary number.
r	Any 8-bit register. (A,B,C,D,E,H, or L)
s	Any 8-bit source register or memory location.
sb	A bit in a specific 8-bit register or memory location.
ss	Any 16-bit source register or memory location.
Z	Zero Flag.
*/