#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gameboyFunctions.h"
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


////////////////////////////////////////////////////////////////////
//Setup

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

// void LD(register8_t *toregister, register8_t *fromregister){
//   toregister = &fromregister; 
// }

uint8_t flag_H_calc(uint8_t x, uint8_t y) {
  x <<= 4;
  x >>= 4;
  y <<= 4;
  y >>= 4;
  return (x + y) >> 4;
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetric functions

void ADD(uint8_t x) {
  // Set flag registers
  set_N(0);
  set_H(flag_H_calc(cpuRegister.A, x));
  set_CY((cpuRegister.A + x) >> 8);
  cpuRegister.A += x;
  if(cpuRegister.A == 0) {
    set_Z(1);
  } else {
    set_Z(0);
  }
}

void ADC(uint8_t x) {
  // Set flag registers
  set_N(0);
  set_H(flag_H_calc(cpuRegister.A, (x + get_CY())));
  int cyValue = (cpuRegister.A + x + get_CY()) >> 8;
  cpuRegister.A += (x + get_CY());
  set_CY(cyValue);
  if(cpuRegister.A == 0) {
    set_Z(1);
  } else {
    set_Z(0);
  }
}

void SUB(uint8_t x) {
  cpuRegister.A -= x;
}


///////////////////////////////////////////////////////////////////////////////
// Rotation functions


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

///////////////////////////////////////////////////////////////////////////////
// Logical functions

void AND(uint8_t x) {
    cpuRegister.A &= x;
}

void OR(uint8_t x) {
    cpuRegister.A |= x;
}

void XOR(uint8_t x) {
    cpuRegister.A ^= x;
}

///////////////////////////////////////////////////////////////////////////////
// Main

int main() {
  memory = (uint8_t *) malloc(memorysize * sizeof(uint8_t));
  cpuRegister.A = 0x80;
  cpuRegister.F = 0;
  printf("value of A before RRCA %x\n", cpuRegister.A);
  printf("value of F before RRCA %x\n", cpuRegister.F);
  printf("First Print Value %d\n\n\n", get_AF());
  set_CY(0);
  ADC(0x80);
  printf("value of A after RRCA %x\n", cpuRegister.A);
  printf("value of F after RRCA %x\n", cpuRegister.F);
  printf("Value of AF Pair after RLCA %d\n", get_AF());
  printf("CY is: %d\n", get_CY());
  printf("Z is: %d\n", get_Z());
  printf("N is: %d\n", get_N());
  printf("H is: %d\n", get_H());

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