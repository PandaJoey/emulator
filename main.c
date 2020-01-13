# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

typedef struct {
  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t H;
  uint8_t L;
  uint16_t PC;
  uint16_t SP;
} cpu_register;

void ADD(uint8_t x);
void LD(uint8_t *toregister, uint8_t *fromregister);

uint8_t *memory;
int memorysize = 0xFFFF;

cpu_register cpuRegister;


void LD(uint8_t *toregister, uint8_t *fromregister){
  toregister = fromregister;
}

void ADD(uint8_t x) {
  cpuRegister.A += x;
}

int main() {
  memory = (uint8_t *) malloc(memorysize * sizeof(uint8_t));
  cpuRegister.A = 200;
  cpuRegister.B = 111;
  uint8_t result = LD((*cpuRegister).A, (*cpuRegister).B);
  printf("%d", result);
  return 0;
}
