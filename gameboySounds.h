
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "gameboyFunctions.h"

typedef struct {
    uint8_t NR10;
    uint8_t NR11;
    uint8_t NR12;
    uint8_t NR13;
    uint8_t NR14;
} square1_sound_registers;

typedef struct {
    uint8_t NR21;
    uint8_t NR22;
    uint8_t NR23;
    uint8_t NR24;
} square2_sound_registers;

typedef struct {
    uint8_t NR30;
    uint8_t NR31;
    uint8_t NR32;
    uint8_t NR33;
    uint8_t NR34;
} wave_sound_registers;

typedef struct {
    uint8_t NR41;
    uint8_t NR42;
    uint8_t NR43;
    uint8_t NR44;
} noise_sound_registers;

typedef struct {

}control_sound_registers;
