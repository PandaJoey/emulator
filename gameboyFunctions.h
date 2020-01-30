/* Everything in this document is just how im thinking about how it might
   work, if you disagree or think im taking the wrong approch please tell
   me so we can move forward together faster on the same mind set and thinking.
   EG some of the fucntion parameters might be total bullshit but its just how im
   thinking what is required looked at the book.

   There is a lot to read here, but i think it is laid out nicely so its easy 
   to understand. I was going to split the file off into subsections but for 
   now i think its just easier to have it all in one file as a reference.

   I am wondering if we need to turn some of these functions into structs as 
   a lot of them take in the same paramenters, if this is even possible. 
   something to look into.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef uint8_t register8_t;
typedef uint8_t register16_t;

typedef struct {
  register8_t A;
  register8_t B;
  register8_t C;
  register8_t D;
  register8_t E;
  register8_t F;
  register8_t H;
  register8_t L;
  register16_t PC;
  register16_t SP;
} cpu_register;

/* dont think this is right just put it here as a representation for 
*/
typedef struct {
  uint16_t AF;
  uint16_t BC;
  uint16_t DE;
  uint16_t HL;
} cpu_register_pairs;	 

uint8_t *memory;
int memorysize = 0xFFFF;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// Get register pairs

uint16_t get_AF();
uint16_t get_BC();
uint16_t get_DE();
uint16_t get_HL();

// Mathamatical functions

/* Needs to add the contents of a register to A and also make a call
   to FLAGREGISTER in this case Z sets if the result is 0 otherwise reset,
   H sets if there is a carry from bit 3 otherwise reset,
   N resets and CY is set if there is a carry from bit 7 otherwise reset.
*/
void ADD(uint8_t x);

/* Needs to subtract from the contents of register a and restores it back into A
   Needs to make a call to FLAGREGISTER, Z sets if result is 0 otherwise reset,
   H sets if there is a borrow from bit 4 otherwise resets, N is set and
   CY is set if there is a borrorw otherwise reset
*/
void SUB(uint8_t x);

/* Needs to be able to add the contents of multiple registers to 
   register A eg you can add CD, HL and CY to what is in A already.
*/
void ADC(cpu_register A, uint8_t x);

// Same as above by taking away
void SBC(cpu_register A, uint8_t x);

/* Needs to be able to take in single register data, paired register
   data, internal ram values 8 bit and 16 bit, has to be able to increment
   or decrement, needs to turn internal ram into hex.
*/
void LD(register8_t *toRegister, register8_t *fromRegister);

/* Needs to push the contents of a register pair onto the stack
   the first 1 is subtraced from SP and the contents of the higher
   portion of the pair are placed on the stack, then the lower portion
   then SP must be decremented by 2.
*/
void PUSH(uint8_t register, cpu_register SP);

/* Needs to pop the contents of the memory stack into a register pair
   Then increments SP by 1 and then,
   Needs to load the lower portion of the stack into the register pair, 
   then needs to load the upper portion of the stack into the register pair
   Then sp needs to be incremented again.
*/
void POP(uint8_t register, cpu_register SP);

/* Needs to set Z to 1 when the result of an operation is 0; otherwise reset
   Needs to set N to 1 following execution of the substruction instruction, 
   regardless of the result
   Needs to set H to 1 when an operation results in carrying from or borrowing 
   to bit 3
   Needs to set CY to 1 when an operation results in carry from or borrowing 
   to bit 7
*/
void FLAGREGISTER(uint8_t  Z, uint8_t  N, uint8_t  H, uint8_t  CY);

/* Used to add the 8 bit operand e to SP, e goes from -128 to +127
   has to make a call to the FLAGREGISTER in each case, Z is reset
   H is set if there is a carry from bit 11 otherwise is reset,
   N is reset and CY is set if there is a carry from bit 15
*/
void LDHL(cpu_register SP, double exp(double e));

/* Increments the contents of a register by 1 not sure if it should take a
   cpu_register too, can also increment by a register value too  
*/
void INC(uint8_t x);

/* Used to Decrement the contents of a register by 1
   Can also decrement a register by a register value
*/
void DEC(uint8_t x);


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// Logical Operators


/* Needs to compare the logical and for each bit of A and the value and
   store this result in A, not sure if it has to logically be correct or not.
*/
void AND(cpu_register A, uint8_t x);

// Same as above by for OR
void OR(cpu_register A, uint8_t x);

// Same as above but for XOR
void XOR(cpu_register A, uint8_t x);

// Used to compare the contents of A with the value x sets flag if they are equal
void CP(cpu_register A, uint8_t x);



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// Rotate shift instructions 

/* Used to rotate components of register A to the left, eg bit 0 is copies to 
   bit 1 and so on, contents of bit 7 are placed in CY and bit 0 of register A. 
   
   My thinking here is we need to take in register a and then place 
   the remained in F.CY, could be the wrong way to do it
*/
void RLCA(cpu_register A, cpu_register F);

/* Rotates the contents of register A to the left, seems to be used if CY=1 in
   the example so i guess you use this in that case.
*/
void RLA(cpu_register A, cpu_register F);

/* Same as RLCA but to the right
*/
void RRCA(cpu_register A, cpu_register F);

/* Same as RLA but to the right
*/
void RRA(cpu_register A, cpu_register F);

/* Rotates the input of a register and/or register pair to the left, stores
   the rotation from the final bit in CY and back in the register, i guess?
*/
void RLC(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Same as above but if CY is 1
*/
void RL(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Same as RLC but for right movement
*/
void RRC(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Same as RL but for right movement, not sure if these right or left
   only ones need the F register.
*/
void RR(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Shifts the contents of the operand to the left, contents of bit 0
   are copied to bit 1 and bit one to bit 1, bit 7 is copied to CY and bit 
   0 is reset
*/
void SLA(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Shifts the contents a register to the right of bit 7 to the right 
   to bit 6 and so on, bit 0 is copied to CY and bit 7 remaines unchanged
*/
void SRA(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Shifts the contents of a register to the right same as above, but bit
   7 is reset instead of unchanged.
*/
void SRL(uint8_t aRegister, uint16_t aRegisterPair, cpu_register F);

/* Swaps the contents of bits 0-3 with 4-7 
*/
void SWAP(uint8_t aRegister, uint16_t aRegisterPair);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Bit Operators

/* Copies the complement of contents of a specified bit in a register to the Z
   Flag of the program status word (PSW), i think the compliment is just a bit
   that represents the memory, not sure what arguments this should take.
*/
void BIT(uint8_t aRegister);
 
/* Used to set a specified bit to 1 in a specified register.
*/
void SET(int bitToChange, cpu_register aRegisterToChange);

/* Used to reset a specified bit in a specified register
*/
void RES(int bitToChange, cpu_register aRegisterToChange);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Jump Instructions

/* Used to load the operand nn to the PC, nn specifies the address of the 
   subsequently executed instruction. the lower order byte is placed into 
   byte 2 of the object code and the higher order byte is placed into byte 3.

   not sure what this needs to take but it feels like a pointer to denote nn and 
   then the register it points to. this is probably wrong, seems more complex 
   in the book
*/
void JP(uint8_t *pointerToAddress, cpu_register F);

/* Used to jump an amount of steps between -127 and +129 from the current 
   address. eg if cc and the flag status do not match the instruction 
   following the current JP will be executed.
*/
void JR(uint8_t *pointedToAddress, uint8_t *pointToJumpTo, cpu_register F);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Call and Return Functions.

/* In Memory, pushes the PC value corresponding to the instruction at the address 
   following that of the CALL instuction to the 2 bytes following the byte 
   specified by the current SP, operand nn is then loaded into the PC
  
   The subroutine is placed after the location specified by the new PC value. 
   When the SR finishes control is returned to the SP using a return instrcution 
   and by popping the start address of the next instruction which was just 
   pushed and moving it to PC.

   With the push the current value of SP is decremented by 1 and the HOB of 
   the PC is loaded into the memory address specified by SP and then SP is 
   decremented again.

   There is also a version where you put in a conditional and it if matches 
   the flag in the F register you can then do the call command so if CC is 
   true then do nn, needs to make a call to the RET function at somepoint.
*/

void CALL(uint8_t *pointerToAddress, cpu_register SP, cpu_register PC);

/* Pops from the memory stack the PC value that was pushed when CALL is called, 
   returning control to the SP. needs to increment the sp by 2 to get it back 
   to the top of the stack. like above can have a condition if true thing
*/
void RET(cpu_register PC, cpu_register SP);

/* Used when an interupt-service routine finishes, executed as follows.
   the address for the return from the interupt is loaded into the PC
   the master interrupt enable flag is returned to its pre-interupt status.
   so it pops the stack then gets interupted.
*/
void RETI(cpu_register PC, cpu_register SP /*someinterupt.*/);

/* Pushes the current value of the PC to the memory stack and loads the  
   page 0 memory addresses provided by some operand t. Then the next
   instuction is fetched from the addess specified by the new content of PC 

   same steps as before with the poping and pushing.

   the RST instruction canbe used to jump from 1 of 8 addresses though.
   need to check the book for these references, dont want to write them here.
*/
void RST(cpu_register PC, cpu_register SP);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// General Purpose Arithmetic Operations and CPU Control Instructions

/* Stands for decimal Adjust Acc, this is a fucntion that seems to be used
   after ADD, ADC, SUB,SBC instuctions, binary coded decimal representation
   is used to set the contents of register A to a BCD eg:
   ADD A, B; A <- 0x7D, N <- 0
   DAA     ; A <- 0x7D, 0X06 (0X83), CY <- 0
   SUB A, B; A <- 0x83 - 0X38 (0X4B) N <- 1
   DAA     ; A <- 0x4B + 0xFA (0x45)
*/
void DAA(cpu_register A, cpu_register aRegister);

/* Takes the ones Compliment of the contents of register A,
   if like me you forgot what that is its the inverted binary
   number where you invert each bit so 1 is 0 and 0 is 1.
*/
void CPL(cpu_register A);

/* Only advnaces the program counter by 1 has no other operations that have
   an effect.
*/
void NOP(cpu_register PC);

/* After a HALT is executed, the system clock is stopped and HALT mode is 
   enabled, even when halted the LCD will still work. Also the status of 
   internal ram register ports remains unchanged.

   HALT can be canceled by an interupt or reset signal.

   the PC is halted at the step after the HALT, if both the interupt
   request flag and the corrosponding interupt enable flag are set
   HALT can be exited. or you can use a master interupt
*/
void HALT();

/* STOP instructions stops both system clock and oscillator circuits, 
   STOP is entered and the LCD is also stopped. the internal RAM is unchangd.
   before entering stop you must make sure all interupt-enable(IE) flags are reset
   all input to p10-p13 is LOW for all
*/
void STOP();
