#include "map.h"
#include "controller.h"

bool pandoraInvertAxis(uint8_t index) {
  //Invert axis if necessary
  if (index == PANDORA_Y_LEFT) { return true; }
  if (index == PANDORA_Y_RIGHT) { return true; }
  return false;
}

uint16_t pandoraToN64Digital(uint8_t index) {
  //Anything to do with digital only controls
  //NOTE: Highest allowed return value is 13

  if (index == PANDORA_RIGHT) { return N64_RIGHT; }
  if (index == PANDORA_LEFT) { return N64_LEFT; }
  if (index == PANDORA_DOWN) { return N64_DOWN; }
  if (index == PANDORA_UP) { return N64_UP; }
  if (index == PANDORA_START) { return N64_START; }
  if (index == PANDORA_X) { return N64_Z; }
  if (index == PANDORA_B) { return N64_B; }
  if (index == PANDORA_A) { return N64_A; }
/*
  //Mapped to analog device below..
  if (index == 0) { return N64_C_RIGHT; }
  if (index == 0) { return N64_C_LEFT; }
  if (index == 0) { return N64_C_DOWN; }
  if (index == 0) { return N64_C_UP; }
*/
  if (index == PANDORA_L) { return N64_L; }
  if (index == PANDORA_R) { return N64_R; }
  return N64_TRASH;
}

void pandoraToN64Analog(DWORD* buffer) {
  //Anything to do with analog controls (or raw buffer access)

  *buffer |= (hardware_controllerGetAxis(PANDORA_X_RIGHT)<0x3F)?(1 << N64_C_LEFT):0;
  *buffer |= (hardware_controllerGetAxis(PANDORA_X_RIGHT)>0xC0)?(1 << N64_C_RIGHT):0;
  *buffer |= (hardware_controllerGetAxis(PANDORA_Y_RIGHT)<0x3F)?(1 << N64_C_UP):0;
  *buffer |= (hardware_controllerGetAxis(PANDORA_Y_RIGHT)>0xC0)?(1 << N64_C_DOWN):0;

  int8_t real = hardware_controllerGetAxis(PANDORA_X_LEFT)-0x80;
  *buffer |= *(uint8_t*)&real << (16+8*N64_X);

  real = hardware_controllerGetAxis(PANDORA_Y_LEFT)-0x80; 
  *buffer |= *(uint8_t*)&real << (16+8*N64_Y);
}
