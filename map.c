#include "map.h"
#include "controller.h"
#include "stdio.h"
#include "math.h"

#include "config.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


//Invert axis if necessary
bool pandoraInvertAxis(uint8_t index) 
{
	return ((index == PANDORA_Y_LEFT) || (index == PANDORA_Y_RIGHT));
}

extern bool keys[256];
extern bool pandoraButton[N64_TRASH+1];
extern int16_t pandoraAxis[4];

//Anything to do with digital only controls
void pandoraToN64Digital() 
{
	if (keys[config.exitMap.mapping])
	{
		system("killall -9 mupen64plus");
	}

	for(int i=0; i<N64_TRASH;i++)
	{
		ButtonMap *bm = &config.buttonMap[i];
		if (bm->type == BUTTON || bm->type == KBOARD)
			pandoraButton[i] = keys[bm->mapping];
		else if (bm->type == ANALOG)
		{
			switch(bm->mapping)
			{
				case ANALOG_LEFT_UP: 
					pandoraButton[i] = (pandoraAxis[PANDORA_Y_LEFT] < -bm->threshold);
					break;
				case ANALOG_LEFT_DOWN: 
					pandoraButton[i] = (pandoraAxis[PANDORA_Y_LEFT] > bm->threshold);
					break;
				case ANALOG_LEFT_LEFT: 
					pandoraButton[i] = (pandoraAxis[PANDORA_X_LEFT] < -bm->threshold);
					break;
				case ANALOG_LEFT_RIGHT: 
					pandoraButton[i] = (pandoraAxis[PANDORA_X_LEFT] > bm->threshold);
					break;
				case ANALOG_RIGHT_UP: 
					pandoraButton[i] = (pandoraAxis[PANDORA_Y_RIGHT] < -bm->threshold);
					break;
				case ANALOG_RIGHT_DOWN: 
					pandoraButton[i] = (pandoraAxis[PANDORA_Y_RIGHT] > bm->threshold);
					break;
				case ANALOG_RIGHT_LEFT: 
					pandoraButton[i] = (pandoraAxis[PANDORA_X_RIGHT] < -bm->threshold);
					break;
				case ANALOG_RIGHT_RIGHT: 
					pandoraButton[i] = (pandoraAxis[PANDORA_X_RIGHT] > bm->threshold);
					break;

			}
		}
	}
}

//Anything to do with analog controls (or raw buffer access)
void pandoraToN64Analog(DWORD* buffer) 
{
	static float analogx=0.0f, analogy=0.0f;
	switch(config.analogMap.mapping)
	{
		case 0:
		{
			int8_t realx, realy;
			realx = pandoraAxis[PANDORA_X_LEFT];
			realy = -pandoraAxis[PANDORA_Y_LEFT]; 
			*buffer |= *(uint8_t*)&realx << (16+8*N64_X);	
			*buffer |= *(uint8_t*)&realy << (16+8*N64_Y);
			break;
		}	

		case 1:			
		{
			int8_t realx, realy;
			realx = pandoraAxis[PANDORA_X_RIGHT];
			realy = -pandoraAxis[PANDORA_Y_RIGHT]; 
			*buffer |= *(uint8_t*)&realx << (16+8*N64_X);	
			*buffer |= *(uint8_t*)&realy << (16+8*N64_Y);
			break;
		}
	
		case 2:
		{
			float xx = 0.0f, yy = 0.0f;
			
			if (keys[KEY_UP]) 		yy += config.analogMap.velocity;
			if (keys[KEY_DOWN]) 	yy -= config.analogMap.velocity;
			if (keys[KEY_RIGHT]) 	xx += config.analogMap.velocity;
			if (keys[KEY_LEFT]) 	xx -= config.analogMap.velocity;
			
			if (analogx>0 && xx<0) analogx = 0.0f;
			if (analogx<0 && xx>0) analogx = 0.0f;
			if (analogy>0 && yy<0) analogy = 0.0f;
			if (analogy<0 && yy>0) analogy = 0.0f;
			if (xx == 0.0f) 		analogx = 0.0f;
			if (yy == 0.0f) 		analogy = 0.0f;
			
			analogx += xx;
			analogy += yy;
			if (analogx > +127.0f) analogx = +127.0f;
			else if (analogx < -127.0f) analogx = -127.0f;
			if (analogy > +127.0f) analogy = +127.0f;
			else if (analogy < -127.0f) analogy = -127.0f;
				
			int8_t real;
			real = (int8_t) analogx;
			*buffer |= *(uint8_t*)&real << (16+8*N64_X);
			real = (int8_t) analogy;
			*buffer |= *(uint8_t*)&real << (16+8*N64_Y);
			break;
		}
	}
}
