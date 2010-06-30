#include <linux/types.h>
#include "map.h"

#define ANALOG 		0
#define BUTTON 		1
#define KBOARD 		2

//map these to unused KEY_* 
#define ANALOG_LEFT_UP 		128
#define ANALOG_LEFT_DOWN	129
#define ANALOG_LEFT_LEFT	130
#define ANALOG_LEFT_RIGHT	131
#define ANALOG_RIGHT_UP 	132
#define ANALOG_RIGHT_DOWN	133
#define ANALOG_RIGHT_LEFT	134
#define ANALOG_RIGHT_RIGHT	135

struct ButtonMap
{
	int mapping;
	int type;
	int threshold;
};

struct AnalogMap
{
	int mapping;
	float velocity;
};

struct Config 
{
	float calibLeft[4];
	float calibRight[4];

	ButtonMap buttonMap[N64_TRASH], exitMap;

	AnalogMap analogMap;
};

extern Config config;

void config_setDefault();
void config_read();
void config_write(const char *filename);

