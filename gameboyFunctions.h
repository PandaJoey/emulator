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

// we need to make sure that if soemthing is added to f that the first 4 bits are always 0 even if a 1 is wrtten
// eg if we tried to put 1111111 in f it needs to be 11110000

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

// Flag functions

void set_CY(int x);
void set_Z(int x);
void set_H(int x);
void set_N(int x);

int get_CY();
int get_Z();
int get_H();
int get_N();

// Mathamatical functions

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// 8 Bit Load Functions.

/* Based on opcodes:
   There are 7 basic load commands we need to consider, below i will list them
   and say what each load needs to do, this does not mean we need 7 load functons
  
   1)LD r,s: loads s into r where r is any 8 bit register and s is any 8 bit source
     register or memory location s = r || n || HL.
   
   2)LD d,r: loads r into d where d is any 8 bit destination register or memory location
     and r is any 8 bit register d = r || HL
   
   3)LD d,n: loads n into d where n is any 8 bit binary number and d is any destination register
     or memory location d = r || HL
   
   4)LD A(ss): loads (ss) into A where ss is any 16 bit source register or memory location
     ss = BC || DE || HL || nn

   5)LD (dd),A: loads A into dd where dd is any 16 bit destination registery or memory location
     dd = BC || DE || HL || nn

   6)LD A, (C): A <-($FF00+C) loads a memory location + CY into register A were CY is the 
    carry flag

   7)LD (C), A: ($FF00+C) <- A loads A into the carry flag memory location.
   
   Takes 4 clocks to process an r, any 8 bit register.
   Takes 8 clocks to process an n, any 8 bit binary number.
   Takes 8 clocks to process a any register pair or 16 bit register.
   Takes 12 clocks to process any register pair if the LD d,n function is used.
*/
void LD(register8_t *toRegister, register8_t *fromRegister);

/* Based on opcodes:
   Loads and decrements whatever is called a the same time.
   1)LDD A, (HL): A <- (HL) || HL <- HL - 1 think this means loads any 2 bit regiser into A
   then decrements it by 1 or lodas HL into HL -1 into A.

   2)LDD (HL), A: (HL) <- A || HL <- HL - 1 think this means loads A into any 2 bit register
   then decrements it by 1 or loads HL into HL -1 into HL.
*/
void LDD();

/* Based on opcodes:
   Loads and increments whatever is called a the same time.
   1)LDI A, (HL): A <- (HL) || HL <- HL + 1 think this means loads any 2 bit regiser into A
   then increments it by 1 or lodas HL into HL -1 into A.

   2)LDI (HL), A: (HL) <- A || HL <- HL + 1 think this means loads A into any 2 bit register
   then decrements it by 1 or loads HL into HL -1 into HL.
*/
void LDI();


/* Based on opcodes:
   Loads A into the address $ff00+n or loads the address into A
   1)LDH(n),A: ($FF00+n) <- A
   2)LDH A,(n): A <- ($FF00+n)

*/
void LDH();

// 16 Bit Loads.


/* Based on opcodes:
   Loads NN into DD where nn is any 16 bit binary number and dd is any 16 bit 
   destination register eg
   1) LD dd, nn; dd <- nn; dd = BC,DE,HL,SP; takes 12 cpu clocks to process

   Can also load whatevers in SP into nn eg
   2) LD (nn), SP; (nn) <- SP; takes 20 cpu clocks to process

   Can also load HL onto the SP eg
   3) LD SP, HL; SP <- HL; takes 8 cpu clocks to process

   Finally it can load the SP+e into HL where e is any 8 bit signed 2's compliment
   discplacement (https://en.wikipedia.org/wiki/Two%27s_complement) eg
   4) LD HL, (SP+e); HL <- (SP+e); takes 12 cpu clocks; resets z=0 n=0 sets h=* cy=*; 

*/
void LD_16();

/* Based on opcodes:
   Pushes ss onto the SP, where ss is any 16-bit source register or memory location, 
   ss=BC,DE,HL,AF; takes 16 clocks eg
   1) PUSH ss; (SP-1) <- ssh (h denotes upper 8 bits)
               (SP-2) <- ssl (l denotes lower 8 bits)
               SP <- SP - 2  (-2 is used to show we added 2 things to the SP)
   So in the book says to decrement SP by 1 for each operation, so i think the
   final operation either just there to show how it works, or we have to send
   -2 to SP which will have some sort of counter to know whats in it.
   
*/
void PUSH(uint8_t register, cpu_register SP);

/* Based on opcodes:
   Pops dd off the SP where dd is any 16-bit destination register or memory location
   dd=BC,DE,HL,AF; takes 12 clocks; eg
   1) POP dd; ddl <- (SP) (where l is the lower 8 bits)
              ddh <- (SP + 1) (where h is the higher 8 bits)
              SP <- SP + 2 (+2 is used to represent the stack emptying by 2 values)
   again like with push i think we have to designate some value to allow the cpu
   to know that there is somenthing on the stack already.
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

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// 8-bit ALU fucntions

/* Based on opcodes:
   Adds A = A + s, s is any 8 bit source register or memory location s= r || n || HL.
   Takes 4 clocks to process an r, any 8 bit register
   Takes 8 clocks to process an n, any 8 bit binary number
   Takes 8 clocks to process a any register pair or 16 bit register.
   Z set if the result of the operation is 0 other wise is reset. 
   H set if there is a carry from bit 3 otherwise reset. 
   N is always reset.
   CY is set if there is a carry form bit 7 otherwise reset.
   z=*, n=0, h=*, cy=*

*/
void ADD(uint8_t x);

/* Based on opcodes:
   Adds A = A + s + CY, s is any 8 bit source register or memory location s = r || n || HL.
   Takes 4 clocks to process an r, any 8 bit register.
   Takes 8 clocks to process an n, any 8 bit binary number.
   Takes 8 clocks to process a any register pair or 16 bit register.
   Z set if the result of the operation is 0 other wise is reset. 
   H set if there is a carry from bit 3 otherwise reset. 
   N is always reset.
   CY is set if there is a carry form bit 7 otherwise reset.
   z=*, n=0, h=*, cy=*


*/
void ADC(uint8_t x);

/* Based on opcodes:
   Subtracts A = A - s, s is any 8 bit source register or memory location s = r || n || HL.
   Takes 4 clocks to process an r, any 8 bit register.
   Takes 8 clocks to process an n, any 8 bit binary number.
   Takes 8 clocks to process a any register pair or 16 bit register.
   Z set if the result of the operation is 0 other wise is reset. 
   H set if there is a carry from bit 3 otherwise reset. 
   N is always reset.
   CY is set if there is a carry form bit 7 otherwise reset.
   z=*, n=1, h=*, cy=*


*/
void SUB(uint8_t x);

/* Based on opcodes:
   Subtracts A = A - s - CY, s is any 8 bit source register or memory location s = r || n || HL.
   Takes 4 clocks to process an r, any 8 bit register.
   Takes 8 clocks to process an n, any 8 bit binary number.
   Takes 8 clocks to process a any register pair or 16 bit register.
   Z set if the result of the operation is 0 other wise is reset. 
   H set if there is a carry from bit 3 otherwise reset. 
   N is always reset.
   CY is set if there is a carry form bit 7 otherwise reset.
   z=*, n=1, h=*, cy=*

*/
void SBC(cpu_register A, uint8_t x);

// Logical Operators

/* Based on opcodes:
   Checks to see if register A is the same as register s where s is any 8-bit regsiter
   s=r,n,(HL) eg
   1) ADD s; A <- A && s; s=r,n,(HL); r=4, n=8, (HL)=8; z=*, n=0, h=1, cy=0
*/
void AND(uint8_t x);

/* Based on opcodoes:
   Checks to see if any of the bits in A are the same as s where s is any 8 bit register
   s=r,n,(HL) eg
   1) OR s: A <- A || s; s=r,n,(HL); r=4, n=8, (HL)=8; z=*, n=0, h=0, cy=0
*/
void OR(uint8_t x);

/* Based on opcodoes:
   Checks the XOR of the bits in A compared to s where s is any 8 bit register
   s=r,n,(HL) eg
   1) OR s: A <- A ^| s; s=r,n,(HL); r=4, n=8, (HL)=8; z=*, n=0, h=0, cy=0
*/
void XOR(uint8_t x);

/* Based on opcodoes:
   CP is used to compare the contents of A with s where s is any 8 bit register
   s=r,n,(HL) if they are the same flags are set eg
   1) CP s; A == s; s=r,n,(HL); r=4, n=8, (HL)=8; z=*, n=1, h=*, cy=*
*/
void CP(uint8_t x);

//Cremental functions

/* Based on opcodes:
   Increments s by 1 where s is any 8-bit source register or memory location
   where s=r,(HL) (not sure why HL, seems HL is a special case 16-bit register) eg
   1) INC s; s <- s + 1; s=r,(HL); r=4, (HL)=12 clocks; z=*, n=0, h=*, cy=-;
*/
void INC(uint8_t x);

/* Based on opcodes:
   Decrements s by 1 where s is any 8-bit source register or memory locaiton,
   where s=r,(HL) eg
   1) DEC s; s <- s - 1; s=r,(HL); r=4, (HL)=12 clocks; z=*, n=1, h=*, cy=-
*/
void DEC(uint8_t x);

// 16 bit Add

/* Based on opcodes:
   Adds either HL and ss or SP and e together where ss is any 16 bit source register or memory location 
   and e is the twos compliment of SP eg
   1) ADD HL, ss; HL <- HL + ss; ss=BC,DE,HL,SP; all 8 clocks; z=-, n=0, h=*, cy=*
   2) ADD SP,e; SP <- SP + e; ss=BC,DE,HL,SP; all 16 clocks; z=0, n=0, h=*, cy=*

   These might have to be seperated or might be abled to be done in standard add
*/
void ADD_16();


// 16 bit cremental functions (may not be required just here for clairty)

/* Based on opcodes:
   Increments ss by 1 where ss is any 16-bit source register or memory location
   where s=BC,DE,HL,SP eg
   1) INC ss; ss <- ss + 1; s=BC,DE,HL,SP; all 8 clocks; z=-, n=-, h=-, cy=-;
*/
void INC_16();

/* Based on opcodes:
   Decrements ss by 1 where ss is any 16-bit source register or memory locaiton,
   where s=BC,DE,HL,SP eg
   1) INC ss; ss <- ss - 1; s=BC,DE,HL,SP; all 8 clocks; z=-, n=-, h=-, cy=-;
*/
void DEC_16()

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// General Purpose Arithmetic Operations and CPU Control Instructions


/* Based on opcodes:
   SWAP s: Swaps the contents of bits 0-3 with 4-7, swaps nibbles. where
   s = r, (HL) where s is any 8 bit source register or memory locaiton.
   1) Swap s; swaps nibbles, s=r,(HL); r=8, (HL)=16 clocks; z=*, n=0, h=0, cy=0;
*/
void SWAP(uint8_t aRegister, uint16_t aRegisterPair);

/* Based on opcodes:
   Stands for decimal Adjust Acc, this is a fucntion that seems to be used
   after ADD, ADC, SUB,SBC instuctions, binary coded decimal representation
   is used to set the contents of register A to a BCD eg:
   ADD A, B; A <- 0x7D, N <- 0
   DAA     ; A <- 0x7D, 0X06 (0X83), CY <- 0
   SUB A, B; A <- 0x83 - 0X38 (0X4B) N <- 1
   DAA     ; A <- 0x4B + 0xFA (0x45)
   takes in no registries; takes 4 clocks; z=*, n=-, h=0, cy=*;
*/
void DAA(cpu_register A, cpu_register aRegister);

/* Based on opcodes:
   Takes the ones Compliment of the contents of register A,
   if like me you forgot what that is its the inverted binary
   number where you invert each bit so 1 is 0 and 0 is 1.
   1) CPL; A <- /A (/A is 1's compliment); 4 clocks; z=-, n=1, h=1, cy=-;
*/
void CPL(cpu_register A);

/* Based on opcodes:
   CY 1's compliment if cy is set then reset, if cy is reset then set it.
   1) CFF; CY <- /CY; 4 clocks; z=-, n=0, h=0, cy=*;
*/
void CCF();

/* Based on opcodes:
   Used to set the carry flag
   1) SCF; CY <- 1; 4 clocks; z=-, n=0, h=0, cy=1;
*/
void SCF();

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

/* Based on opcodes:
   This instruction disables interrupts but not immediately. Interrupts are disabled after
   instruction after DI is executed.
*/
void DI();

/* Based on opcodes:
   Enable interrupts. This intruction enables interrupts but not immediately. Interrupts are enabled after
   instruction after EI is executed.
*/
void EI();

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// Rotate shift instructions 

/* Based on opcodes:
   Used to rotate A left, the old bit 7 is placed into the carry flag
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 7 data. 
   z=0, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RLC         -/-        07     4
*/
void RLCA();

/* Based on opcodes:
   Used to rotate A left through the carry flag, instead of rotating then 
   setting the carry flag it incorperates the carry flag
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 7 data
   z=0, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RLC         -/-        17     4
*/
void RLA();

/* Based on opcodes:
   Used to rotate A right, the old bit 0 is placed into the carry flag
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 0 data.
   z=0, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RLC         -/-        0F     4
*/
void RRCA();

/* Based on opcodes:
   Used to rotate A right through the carry flag, instead of rotating then 
   setting the carry flag it incorperates the carry flag
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 0 data
   z=0, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RLC         -/-        1F     4
*/
void RRA();

/* Based on opcodes:
   Used to rotate s left the old bit 7 is placed into the carry flag, 
   where s is any 8-bit source register or memory location
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 7 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RLC         A          CB 07  8
   RLC         B          CB 00  8
   RLC         C          CB 01  8
   RLC         D          CB 02  8
   RLC         E          CB 03  8
   RLC         H          CB 04  8
   RLC         L          CB 05  8
   RLC         (HL)       CB 06  16
*/
void RLC();

/* Based on opcodes:
   Used to rotate s left through the carry flaginstead of rotating then 
   setting the carry flag it incorperates the carry flag, 
   where s is any 8-bit source register or memory location
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 7 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RL          A          CB 17  8
   RL          B          CB 10  8
   RL          C          CB 11  8
   RL          D          CB 12  8
   RL          E          CB 13  8
   RL          H          CB 14  8
   RL          L          CB 15  8
   RL          (HL)       CB 16  16
*/
void RL();

/* Based on opcodes:
   Used to rotate s right the old bit 0 is placed into the carry flag, 
   where s is any 8-bit source register or memory location
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 0 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RRC         A          CB 0F  8
   RRC         B          CB 08  8
   RRC         C          CB 09  8
   RRC         D          CB 0A  8
   RRC         E          CB 0B  8
   RRC         H          CB 0C  8
   RRC         L          CB 0D  8
   RRC         (HL)       CB 0E  16
*/
void RRC();

/* Based on opcodes:
   Used to rotate s right through the carry flaginstead of rotating then 
   setting the carry flag it incorperates the carry flag, 
   where s is any 8-bit source register or memory location
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 0 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   RR          A          CB 1F  8
   RR          B          CB 18  8
   RR          C          CB 19  8
   RR          D          CB 1A  8
   RR          E          CB 1B  8
   RR          H          CB 1C  8
   RR          L          CB 1D  8
   RR          (HL)       CB 1E  16
*/
void RR();

/* Based on opcodes:
   Used to shift s left the into the carry FLAG and the LSB of s is set to 0
   (LSB is the least significant bit.)
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 7 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   SLA         A          CB 27  8
   SLA         B          CB 20  8
   SLA         C          CB 21  8
   SLA         D          CB 22  8
   SLA         E          CB 23  8
   SLA         H          CB 24  8
   SLA         L          CB 25  8
   SLA         (HL)       CB 26  16
*/
void SLA();

/* Based on opcodes:
   Used to shift s right the into the carry FLAG and the MSB does not change
   (MSB is the most significant bit.)
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 0 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   SRA         A          CB 2F  8
   SRA         B          CB 28  8
   SRA         C          CB 29  8
   SRA         D          CB 2A  8
   SRA         E          CB 2B  8
   SRA         H          CB 2C  8
   SRA         L          CB 2D  8
   SRA         (HL)       CB 2E  16
*/
void SRA();

/* Based on opcodes:
   Used to shift s right the into the carry FLAG and the MSB is set to 0
   (MSB is the most significant bit.)
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if result is zero.
   N - Reset.
   H - Reset.
   C - Contains old bit 0 data.
   z=*, n=0, h=0, cy=*;
   Instruction Parameters Opcode Cycles
   SRA         A          CB 3F  8
   SRA         B          CB 38  8
   SRA         C          CB 39  8
   SRA         D          CB 3A  8
   SRA         E          CB 3B  8
   SRA         H          CB 3C  8
   SRA         L          CB 3D  8
   SRA         (HL)       CB 3E  16
*/
void SRL();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Bit Operators

/* Based on opcodes:
   Used to test a bit b in any s which is any sorce register or memory location.
   b = 0 - 7, 
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if bit b of register r is 0.
   N - Reset.
   H - Set.
   C - Not affected.
   z=*, n=0, h=1, cy=-;
   Instruction Parameters Opcode Cycles
   BIT         b,A        CB 47  8
   BIT         b,B        CB 40  8
   BIT         b,C        CB 41  8
   BIT         b,D        CB 42  8
   BIT         b,E        CB 43  8
   BIT         b,H        CB 44  8
   BIT         b,L        CB 45  8
   BIT         b,(HL)     CB 46  16
*/
void BIT();
 
/* Based on opcodes:
   Set Bit b in register s 
   b = 0 - 7, 
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if bit b of register r is 0.
   N - Reset.
   H - Set.
   C - Not affected.
   z=-, n=-, h=-, cy=-;
   Instruction Parameters Opcode Cycles
   SET         b,A        CB C7  8
   SET         b,B        CB C0  8
   SET         b,C        CB C1  8
   SET         b,D        CB C2  8
   SET         b,E        CB C3  8
   SET         b,H        CB C4  8
   SET         b,L        CB C5  8
   SET         b,(HL)     CB C6  16
*/
void SET();

/* Based on opcodes:
   Reset Bit b in register s 
   b = 0 - 7, 
   s = A,B,C,D,E,H,L,(HL)
   Z - Set if bit b of register r is 0.
   N - Reset.
   H - Set.
   C - Not affected.
   z=-, n=-, h=-, cy=-;
   Instruction Parameters Opcode Cycles
   RES         b,A        CB 87  8
   RES         b,B        CB 80  8
   RES         b,C        CB 81  8
   RES         b,D        CB 82  8
   RES         b,E        CB 83  8
   RES         b,H        CB 84  8
   RES         b,L        CB 85  8
   RES         b,(HL)     CB 86  16
*/
void RES();

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
void JP();

/* Used to jump an amount of steps between -127 and +129 from the current 
   address. eg if cc and the flag status do not match the instruction 
   following the current JP will be executed.
*/
void JR();

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