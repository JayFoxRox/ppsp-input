#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include <linux/input.h>

#ifndef __LINUX__
#include <windows.h>
#else
#include "winlnxdefs.h"
#endif

//Digital
#define N64_RIGHT	0
#define N64_LEFT 	1
#define N64_DOWN 	2
#define N64_UP 		3
#define N64_START 	4
#define N64_Z 		5
#define N64_B 		6
#define N64_A 		7
#define N64_C_RIGHT 8
#define N64_C_LEFT 	9
#define N64_C_DOWN 	10
#define N64_C_UP 	11
#define N64_R 		12
#define N64_L 		13
#define N64_TRASH 	14

//Analog
#define N64_X 		0
#define N64_Y 		1

//Digital
#define PANDORA_RIGHT 	KEY_RIGHT
#define PANDORA_LEFT 	KEY_LEFT
#define PANDORA_DOWN 	KEY_DOWN
#define PANDORA_UP 		KEY_UP
#define PANDORA_START 	KEY_LEFTALT
#define PANDORA_SELECT 	KEY_LEFTCTRL
#define PANDORA_PANDORA KEY_MENU


#define PANDORA_X 		KEY_PAGEDOWN
#define PANDORA_B 		KEY_HOME
#define PANDORA_A 		KEY_END
#define PANDORA_Y 		KEY_PAGEUP
#define PANDORA_R 		KEY_RIGHTCTRL
#define PANDORA_L 		KEY_RIGHTSHIFT

//Analog
#define PANDORA_X_LEFT 	0
#define PANDORA_Y_LEFT 	1
#define PANDORA_X_RIGHT 2
#define PANDORA_Y_RIGHT 3

bool pandoraInvertAxis(uint8_t index);
void pandoraToN64Digital();
void pandoraToN64Analog(DWORD* buffer);
