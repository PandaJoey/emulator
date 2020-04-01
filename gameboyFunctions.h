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

uint8_t* memory;
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
   This is going to be alot of comments as theres a lot of loads to cover so i will do
   them in order of the book and write what each one is then what it does.
   1) LD ss, n: Used to load any 16-bit source register or memory location into an 8-bit n
   ss = B,C,D,E,H,L,BC,DE,HL,SP
   n = 8 bit immediate value
   Instruction   Parameters   Opcode   Cycles
   LD            B,n          06       8
   LD            C,n          0E       8
   LD            D,n          16       8
   LD            E,n          1E       8
   LD            H,n          26       8
   LD            L,n          2E       8

   2)LD r1, r2: Used to put r2 into r1 where r1 and r2 8-bit registers
   r1,r2 = A,B,C,D,E,H,L,(HL)
   Instruction   Parameters   Opcode   Cycles
   LD            A,A          7F       4
   LD            A,B          78       4
   LD            A,C          79       4
   LD            A,D          7A       4
   LD            A,E          7B       4
   LD            A,H          7C       4
   LD            A,L          7D       4
   LD            A,(HL)       7E       8
   LD            B,B          40       4
   LD            B,C          41       4
   LD            B,D          42       4
   LD            B,E          43       4
   LD            B,H          44       4
   LD            B,L          45       4
   LD            B,(HL)       46       8
   LD            C,B          48       4
   LD            C,C          49       4
   LD            C,D          4A       4
   LD            C,E          4B       4
   LD            C,H          4C       4
   LD            C,L          4D       4
   LD            C,(HL)       4E       8
   LD            D,B          50       4
   LD            D,C          51       4
   LD            D,D          52       4
   LD            D,E          53       4
   LD            D,H          54       4
   LD            D,L          55       4
   LD            D,(HL)       56       8
   LD            E,B          59       4
   LD            E,D          5A       4
   LD            E,E          5B       4
   LD            E,H          5C       4
   LD            E,L          5D       4
   LD            E,(HL)       5E       8
   LD            H,B          60       4
   LD            H,C          61       4
   LD            H,D          62       4
   LD            H,E          63       4
   LD            H,H          64       4
   LD            H,L          65       4
   LD            H,(HL)       66       8
   LD            L,B          68       4
   LD            L,C          69       4
   LD            L,D          6A       4
   LD            L,E          6B       4
   LD            L,H          6C       4
   LD            L,L          6D       4
   LD            L,(HL)       6E       8
   LD            (HL),B       70       8
   LD            (HL),C       71       8
   LD            (HL),D       72       8
   LD            (HL),E       73       8
   LD            (HL),H       74       8
   LD            (HL),L       75       8
   LD            (HL),n       36       12

   3)LD A, s/ss: Used to put the value s/ss into A
   s/ss = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn),#
   nn = two byte immediate value. (LS byte first.)
   Instruction   Parameters   Opcode   Cycles
   LD            A,A          7F       4
   LD            A,B          78       4
   LD            A,C          79       4
   LD            A,D          7A       4
   LD            A,E          7B       4
   LD            A,H          7C       4
   LD            A,L          7D       4
   LD            A,(BC)       0A       8
   LD            A,(DE)       1A       8
   LD            A,(HL)       7E       8
   LD            A,(nn)       FA       16
   LD            A,#          3E       8

   4)LD s/ss/n, A: Used to load A into s/ss/n where
   s/ss/n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn)
   nn = two byte immediate value. (LS byte first.)
   Instruction   Parameters   Opcode   Cycles
   LD            A,A          7F       4
   LD            B,A          47       4
   LD            C,A          4F       4
   LD            D,A          57       4
   LD            E,A          5F       4
   LD            H,A          67       4
   LD            L,A          6F       4
   LD            (BC),A       02       8
   LD            (DE),A       12       8
   LD            (HL),A       77       8
   LD            (nn),A       EA       16

   5)LD A, (C): Used to put the value at address $FF00 + register C
   into register A, its the asme as: LD A, ($FF00+C)
   Instruction   Parameters   Opcode   Cycles
   LD            A,(C)        F2       8

   6)LD (C), A: Used to put A into the address $FF00 + register C
   Instruction   Parameters   Opcode   Cycles
   LD           ($FF00+C),A   E2       8

   7) LD A,(HLD)
   Description: Same as: LDD A,(HL)
   
   8) LD A,(HL-)
   Description: Same as: LDD A,(HL)

   9) LD A,(HLI)
   Description: Same as: LDI A,(HL)
   
   10) LD A,(HL+)
   Description: Same as: LDI A,(HL)
*/
void LD(register8_t* toRegister, register8_t* fromRegister);

/* Based on opcodes:
   There are multiple things this fucntion must do.
   1)LDD A,(HL): Used to put the value at the address HL into A,
   then decrements A, its the sae as LD A, (HL) - DEC HL
   Instruction   Parameters   Opcode   Cycles
   LD            A,(HLD)      3A       8
   LD            A,(HL-)      3A       8
   LDD           A,(HL)       3A       8
   
   2)LDD (HL), A Used to put A into memory address HL then decrements HL it
   is the same as LD (HL), A - DEC HL
   Instruction   Parameters   Opcode   Cycles
   LD            (HLD),A      32       8
   LD            (HL-),A      32       8
   LDD           (HL),A       32       8
*/
void LDD();

/* Based on opcodes:
   There are multiple things this fucntion must do.
   1)LDI A,(HL): Used to Put value at address HL into A. Increment HL.
   Same as: LD A,(HL) - INC HL
   Instruction   Parameters   Opcode   Cycles
   LD            A,(HLI)      2A       8
   LD            A,(HL+)      2A       8
   LDI           A,(HL)       2A       8

   2)LDI (HL), A Used to Put A into memory address HL. Increment HL.
   Same as: LD (HL),A - INC HL
   Instruction   Parameters   Opcode   Cycles
   LD            (HLI),A      22       8
   LD            (HL+),A      22       8
   LDI           (HL),A       22       8
*/
void LDI();


/* Based on opcodes:
   There are multiple things this fucntion must do.
   1)LDH (n),A: Used to put A into memory address $FF00+n
   n = one byte immediate value.
   Instruction   Parameters   Opcode   Cycles
   LD           ($FF00+n),A   E0       12

   2)LDI A,(n) Used to Put memory address $FF00+n into A.
   n = one byte immediate value.
   Instruction   Parameters   Opcode   Cycles
   LD           A,($FF00+n)   E0       12
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
   PUSH ss;
   Pushes register pair ss onto the stack and then decrements the stack pointer SP
   twice.
   ss=AF,BC,DE,HL
   Instruction     Parameters     Opcode     Cycles
   PUSH            AF             F5         16
   PUSH            BC             C5         16
   PUSH            DE             D5         16
   PUSH            HL             E5         16
                                  
*/
void PUSH(uint8_t register, cpu_register SP);

/* Based on opcodes:
   POP ss;
   Pops two bytes off the stack into a register pair ss, then increments SP twice;
   ss=AF,BC,DE,L
   Instruction     Parameters     Opcode     Cycles
   POP             AF             F1         12
   POP             BC             C1         12
   POP             DE             D1         12
   POP             HL             E1         12

*/
void POP(uint8_t register, cpu_register SP);

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// 8-bit ALU fucntions

/* Based on opcodes:
   ADD A,s; 
   Adds s to A 
   s=A,B,C,D,E,H,L,(HL),#
   Z - Set if result is zero.
   N - Reset.
   H - Set if carry from bit 3.
   C - Set if carry from bit 7.
   z=*, n=0, h=*, cy=*
   Instruction     Parameters     Opcode     Cycles
   ADD             A,A            87         4
   ADD             A,B            80         4
   ADD             A,C            81         4
   ADD             A,D            82         4
   ADD             A,E            83         4
   ADD             A,H            84         4
   ADD             A,L            85         4
   ADD             A,(HL)         86         8
   ADD             A,#            C6         8

*/
void ADD(uint8_t x);

/* Based on opcodes:
   ADC A, s;
   Adds s + CY to A
   s=A,B,C,D,E,H,L,(HL),#
   Z - Set if result is zero.
   N - Reset.
   H - Set if carry from bit 3.
   C - Set if carry from bit 7.
   z=*, n=0, h=*, cy=*
   Instruction     Parameters     Opcode     Cycles
   ADC             A,A            8F         4
   ADC             A,B            88         4
   ADC             A,C            89         4
   ADC             A,D            8A         4
   ADC             A,E            8B         4
   ADC             A,H            8C         4
   ADC             A,L            8D         4
   ADC             A,(HL)         8E         8
   ADC             A,#            CE         8


*/
void ADC(uint8_t x);

/* Based on opcodes:
   SUB s:
   Subtracts s from A
   s=A,B,C,D,E,H,L,(HL),#
   Z - Set if result is zero.
   N - Set.
   H - Set if no borrow from bit 4.
   C - Set if no borrow.
   z=*, n=1, h=*, cy=*
   Instruction     Parameters     Opcode     Cycles
   SUB             A              97         4
   SUB             B              90         4
   SUB             C              91         4
   SUB             D              92         4
   SUB             E              93         4
   SUB             H              94         4
   SUB             L              95         4
   SUB             (HL)           96         8
   SUB             #              D6         8

*/
void SUB(uint8_t x);

/* Based on opcodes:
   SBC A, s;
   Subtracts s + CY from A
   s=A,B,C,D,E,H,L,(HL),#
   Z - Set if result is zero.
   N - Set.
   H - Set if no borrow from bit 4.
   C - Set if no borrow.
   z=*, n=1, h=*, cy=*
   Instruction     Parameters     Opcode     Cycles
   SBC             A,A            9F         4
   SBC             A,B            98         4
   SBC             A,C            99         4
   SBC             A,D            9A         4
   SBC             A,E            9B         4
   SBC             A,H            9C         4
   SBC             A,L            9D         4
   SBC             A,(HL)         9E         8
   SBC             A,#            ??         ?


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
void DEC_16();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// General Purpose Arithmetic Operations and CPU Control Instructions


/* Based on opcodes:
   SWAP s: Swaps the contents of bits 0-3 with 4-7, swaps nibbles. where
   s = r, (HL) where s is any 8 bit source register or memory locaiton.
   1) Swap s; swaps nibbles, s=r,(HL); r=8, (HL)=16 clocks; z=*, n=0, h=0, cy=0;
*/
void SWAP();

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
void DAA();

/* Based on opcodes:
   Takes the ones Compliment of the contents of register A,
   if like me you forgot what that is its the inverted binary
   number where you invert each bit so 1 is 0 and 0 is 1.
   1) CPL; A <- /A (/A is 1's compliment); 4 clocks; z=-, n=1, h=1, cy=-;
*/
void CPL();

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

/* Based on opcodes:
   This function has 3 uses we might need 3 seperate functions, as follows
   1) Used to jump to an address nn, where nn is any 16-bit binary number
   in memory.
   nn = two byte immediate value. (LS byte first meaning least significante byte first.)
   Instruction Parameters Opcode Cycles
   JP          nn         C3     12

   2) Jumps to address n if the foolowing conditions are true where cc is 
   the flag condition code from C,NC,NZ,Z
   cc = NZ, Jump if Z flag is reset.
   cc = Z, Jump if Z flag is set.
   cc = NC, Jump if C flag is reset.
   cc = C, Jump if C flag is set.
   nn = two byte immediate value. (LS byte first.)
   Instruction Parameters Opcode Cycles
   JP          NZ, nn     C2     12
   JP          Z, nn      CA     12
   JP          NC, nn     D2     12
   JP          C, nn      DA     12

   3) Jumps to whatever address is contained in HL
   Instruction Parameters Opcode Cycles
   JP          HL         E9     4
*/
void JP();

/* Based on opcodes:
   This function has 2 uses we might need 2 seperate functions, as follows
   1) Used to add n to the current address and then jumps to it
   n = one byte signed immediate value
   Instruction Parameters Opcode Cycles
   JR          n          18     8

   2) Jumps to address n if the foolowing conditions are true where cc is
   the flag condition code from C,NC,NZ,Z
   n = one byte signed immediate value
   cc = NZ, Jump if Z flag is reset.
   cc = Z, Jump if Z flag is set.
   cc = NC, Jump if C flag is reset.
   cc = C, Jump if C flag is set.
   (* is whatever the outcome is with the carrys)
   Instruction Parameters Opcode Cycles
   JR          NZ, *      20     8
   JR          Z, *       28     8
   JR          NC, *      30     8
   JR          C, *       38     8
*/
void JR();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Call and Return Functions.

/* Based on opcodes:
   This function has 2 uses we might need 2 seperate functions, as follows
   1) Used to push the addres of the next instruction onto the stack and 
   then jump to the next nn address where nn is any 16 bit binary number.
   nn = two byte immediate value. (LS byte first.)
   Instruction Parameters Opcode Cycles
   CALL        nn         CD     8

   2) Call address n if the following condition is true where cc is
   the flag condition code from C,NC,NZ,Z
   n = one byte signed immediate value
   cc = NZ, Call if Z flag is reset.
   cc = Z, Call if Z flag is set.
   cc = NC, Call if C flag is reset.
   cc = C, Call if C flag is set.
   Instruction Parameters Opcode Cycles
   CALL        NZ,nn      C4     12
   CALL        Z,nn       CC     12
   CALL        NC,nn      D4     12
   CALL        C,nn       DC     12
*/                               
void CALL();

/* Based on opcodes:
   used to push the present address onto the stack it then
   jumps to the address $0000 + f where f 8 special call locations
   in page zero and is defined as follows
   n = $00,$08,$10,$18,$20,$28,$30,$38
   Instruction Parameters Opcode Cycles
   RST         00H        C7     32
   RST         08H        CF     32
   RST         10H        D7     32
   RST         18H        DF     32
   RST         20H        E7     32
   RST         28H        EF     32
   RST         30H        F7     32
   RST         38H        FF     32
*/
void RST(cpu_register PC, cpu_register SP);

/* Based on opcodes:
   This function has 2 uses and are as follows
   1) Used to pop two bytes from the stack and then jumps to
   the poped address
   n = $00,$08,$10,$18,$20,$28,$30,$38
   Instruction Parameters Opcode Cycles
   RET         -/-        C9     8

   2)Used to pop two bytes from the stack then jumps to the
   popped address only if the following conditions are true.
   cc = NZ, Return if Z flag is reset.
   cc = Z, Return if Z flag is set.
   cc = NC, Return if C flag is reset.
   cc = C, Return if C flag is set.
   Instruction Parameters Opcode Cycles
   RET         NZ         C0     8
   RET         Z          C8     8
   RET         NC         D0     8
   RET         C          D8     8
*/
void RET(cpu_register PC, cpu_register SP);

/* Based on opcodes:
   Used to pop two bytes from the stack and jumps to that address
   then enables the interupter
   Instruction Parameters Opcode Cycles
   RETI        -/-        D9     8
*/
void RETI(cpu_register PC, cpu_register SP /*someinterupt.*/);

