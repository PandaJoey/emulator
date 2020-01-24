# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

typedef uint8_t register8;
typedef uint16_t register16;

typedef struct {
  register8 A;
  register8 B;
  register8 C;
  register8 D;
  register8 E;
  register8 F;
  register8 H;
  register8 L;
  register16 PC;
  register16 SP;
} cpu_register;

void ADD(uint8_t x);
void LD(register8 toregister, register8 fromregister);

uint8_t *memory;
int memorysize = 0xFFFF;

cpu_register cpuRegister;


void LD(register8 *toregister, register8 *fromregister){
  toregister = &fromregister;
}

void ADD(uint8_t x) {
  cpuRegister.A += x;
}

int main() {
  memory = (uint8_t *) malloc(memorysize * sizeof(uint8_t));
  cpuRegister.A = 200;
  cpuRegister.A++;
  cpuRegister.B = 111;
  LD(cpuRegister.*A cpuRegister.*B);
  printf("%d", cpuRegister.A);
  return 0;
}
