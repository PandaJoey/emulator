# include <stdio.h>
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

int add(uint8_t x, uint8_t y);

int add(uint8_t x, uint8_t y) {
  return x+y;
}

int main() {
  cpu_register cpuRegister;
  cpuRegister.A = 3;
  cpuRegister.B = 2;
  int result = add(cpuRegister.A, cpuRegister.B);
  printf("%d", result);
  return 0;
}
