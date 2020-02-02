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

/*Ok so this is working as follows:

  Edit: After messing around some more i made a more simple version, 
  however, i think we need to impliment F properly before we can make
  these work, we need to store that extra bit in CY, then call that bit
  from CY into the first bit of A eg the number 200(11001000) has a carry
  currenty this code will turn it into 144(10010000) it should return 
  145(10010001)


  If the first bit is 0 it will run the first block of code and only
  works if the number is as it will rotate right to 254 which is
  1111110, then takes that bit stores it in what will be CY(128 is 1000000)
  its a badly hard coded way of doing it but its along the right lines.

  Else if the 8th bit is 1 it will do the same as the other but +1 again
  only works if 64 is bit shifted.

  TODO, need to now make it work for all numbers that dont have a carry
  or a follow. eg if you input 33 (00100001) it will become 66 (1000010)
  which will go into the first if turn it to 127 and not be correct.

  but its a good start.
*/
void RLCA(register8_t A, register8_t F) {
    cpuRegister.A <<= 1;
    printf("value of A after first rotation %d\n\n ", cpuRegister.A);
    
    if(cpuRegister.A == 254){
      printf("Value of F before adding %d\n\n ", cpuRegister.F);
      cpuRegister.F = 128;
      printf("Value of F after adding %d\n\n ", cpuRegister.F);
      cpuRegister.A = cpuRegister.F - 1;
      printf("the value of A before rotation %d \n\n", cpuRegister.A);
     // cpuRegister.A <<= 1;
      printf("the value of A after rotation %d \n\n", cpuRegister.A);
    }else if(cpuRegister.A == 128){
      printf("Value of F before adding %d\n\n ", cpuRegister.F);
      cpuRegister.F = 128;
      printf("Value of F after adding %d\n\n ", cpuRegister.F);
      cpuRegister.A = cpuRegister.F + 1;
     printf("the value of A before rotation %d \n\n", cpuRegister.A);
     // cpuRegister.A <<= 1;
      printf("the value of A after rotation %d \n\n", cpuRegister.A);
    }else if(cpuRegister.A & (1 << 7)) {
      cpuRegister.A += 1;
      printf("value of A after first rotation %d\n\n ", cpuRegister.A);
    }


    // if(!!cpuRegister.A & (1 << 0)){
    //   printf("Value of F before adding %d\n\n ", cpuRegister.F);
    //   cpuRegister.F = 128;
    //   printf("Value of F after adding %d\n\n ", cpuRegister.F);
    //   cpuRegister.A = cpuRegister.F - 1;
    //  printf("the value of A before rotation %d \n\n", cpuRegister.A);
    //  // cpuRegister.A <<= 1;
    //   printf("the value of A after rotation %d \n\n", cpuRegister.A);
    // }else if(cpuRegister.A & (1 << 7)){
    //   printf("Value of F before adding %d\n\n ", cpuRegister.F);
    //   cpuRegister.F = 128;
    //   printf("Value of F after adding %d\n\n ", cpuRegister.F);
    //   cpuRegister.A = cpuRegister.F + 1;
    //  printf("the value of A before rotation %d \n\n", cpuRegister.A);
    //  // cpuRegister.A <<= 1;
    //   printf("the value of A after rotation %d \n\n", cpuRegister.A);
    // }
    
    

    // correct?? check docs
}

// RLA goes here
void RLA(register8_t A, register8_t F) {
    if(CHECK_BIT(cpuRegister.F, cpuRegister.F-1 )) {
      cpuRegister.A <<= 1;
    }

}

void RRCA(register8_t A, register8_t F) {
    cpuRegister.A <<= 1;
    cpuRegister.F <<= 1;
}

// RRA goes here
void RRA(register8_t A, register8_t F) {
    cpuRegister.A <<= 1;
    cpuRegister.F <<= 1;
}

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
  cpuRegister.A = 111;
  cpuRegister.F = 0;
  printf("value of A after RLCA %d\n", cpuRegister.A);
  printf("value of F after RLCA %d\n", cpuRegister.F);
  printf("First Print Value %d\n", get_AF());
  RLCA(cpuRegister.A, cpuRegister.F);
  printf("value of A after RLCA %d\n", cpuRegister.A);
  printf("value of F after RLCA %d\n", cpuRegister.F);
  printf("Value of AF Pair after RLCA %d\n", get_AF());
  
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
