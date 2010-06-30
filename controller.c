//#Maps to Keyboard and Mouse
//Maps to Pandora-Controls (priority) and network
//Controlled by function-access

#include "controller.h"
#include "map.h"
#include "config.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define NAME_LENGTH 128
#define PATH_LENGTH 512

bool 	keys[512];

int 	analogHandle[2];
int 	buttonsHandle;
int 	keyboardHandle;

bool pandora;
bool pandoraButton[N64_TRASH+1];
int16_t pandoraAxis[4];

bool readEvent(int handle,struct input_event* event) {
  fd_set events;
  FD_ZERO(&events);
  FD_SET(handle, &events);
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  select(handle+1,&events,NULL,NULL,&timeout);
  if (!FD_ISSET(handle,&events)) { return false; }
  int rd = read(handle, event, sizeof(struct input_event));
  if (rd < (int)sizeof(struct input_event)) {
    printf("Read error from event!\n");
    usleep(1000*1000);
    return false;
  }
  return true;
}

int openEvent(uint8_t index) {
  char fp[PATH_LENGTH];
  sprintf(fp, "/dev/input/event%i", index);
  return open(fp, O_RDONLY);
}

void closeEvent(int handle) {
  close(handle);
}

uint8_t searchEvent(const char* search) {  
  char fp[PATH_LENGTH];
  char name[NAME_LENGTH];
  int fd;
  for (int i = 0; 1; i++)
  {
    sprintf(fp, "/dev/input/event%i", i);
    fd = open(fp, O_RDONLY);
    if (fd < 0) {
      return 0x80;
      break;
    }
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    close(fd);
    if (strcmp(name, search) == 0) {
      return i & 0x7F;
    }
  }
  usleep(1000*1000);
}

uint8_t hardware_controllerGetAxis(uint8_t index) {
  if (pandora == false) { return 0; }
  float tempf = (float)pandoraAxis[index]; //[-256,256]
  if (pandoraInvertAxis(index)) { tempf *= -1.0f; }
  tempf += 256.0f; //[0,512]
  tempf /= 512.0f; //[0,1]
  tempf *= 255; //[0,255]
  return (uint8_t)tempf;
}

bool hardware_controllerGetButton(uint8_t index) {
  if (pandora == false) { return false; }
  return pandoraButton[index];
}

//
int hardware_controllerAnalogCalib(int value, int analog) 
{
	float *c = ((analog == 0) ? config.calibLeft : config.calibRight);
	float v = ((float)abs(value))/127;
	float r;
	
	r = c[3];
	for(int i=2; i>=0; i--)
	{
		r = r*v + c[i];
	}
	r = r*v;	

	if (r>1) r=1;
	if (value<0) r = -r;
	return (int)(r*127);
}

void hardware_controllerRefresh() {
  if (pandora == false) { return; }
  //for(int i=0;i<256;i++) keys[i] = false;
  struct input_event event;
  while(readEvent(analogHandle[0],&event)) {
    if(event.type == EV_ABS) {
	  int v = hardware_controllerAnalogCalib(event.value/2, 0);
      if (event.code == ABS_X) {pandoraAxis[PANDORA_X_LEFT] = v;}
      if (event.code == ABS_Y) {pandoraAxis[PANDORA_Y_LEFT] = v;}
    }
  }  
  while(readEvent(analogHandle[1],&event)) {
    if(event.type == EV_ABS) {
	  int v = hardware_controllerAnalogCalib(event.value/2, 1);
      if (event.code == ABS_X) { pandoraAxis[PANDORA_X_RIGHT] = v; }
      if (event.code == ABS_Y) { pandoraAxis[PANDORA_Y_RIGHT] = v; }
    }
  }
  while(readEvent(buttonsHandle,&event)) {
    if(event.type == EV_KEY) {
      keys[event.code] = event.value;
	}
  }
  while(readEvent(keyboardHandle,&event)) {
    if(event.type == EV_KEY) {
      keys[event.code] = event.value;
    }
  }
}

void hardware_controllerInitialize() {
  uint8_t analog[2];
  analog[0] = searchEvent("nub0");
  analog[1] = searchEvent("nub1");
  uint8_t buttons = searchEvent("gpio-keys");
  uint8_t keyboard = searchEvent("keypad");
  uint8_t touchscreen = searchEvent("touchscreen");

  if ((analog[0] | analog[1] | buttons) & 0x80) { //TODO: In final version check for keyboard | touchscreen too?
    pandora = false;
    printf("Unable to find pandora hardware (%i%i%i/%i%i), trying remote\n",(analog[0] & 0x80)?1:0,(analog[1] & 0x80)?1:0,(buttons & 0x80)?1:0,(keyboard & 0x80)?1:0,(touchscreen & 0x80)?1:0);
    usleep(1000*1000);
  }
  else {
    pandora = true;
    analogHandle[0] = openEvent(analog[0]);
    analogHandle[1] = openEvent(analog[1]);
    buttonsHandle = openEvent(buttons);
	keyboardHandle = openEvent(keyboard);
  }
  //usleep(1000*1000);
}
