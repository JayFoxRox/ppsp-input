#include <stdint.h>
#include <stdbool.h>

void hardware_controllerInitialize();
void hardware_controllerRefresh();
uint8_t hardware_controllerGetAxis(uint8_t index);
bool hardware_controllerGetButton(uint8_t index);
