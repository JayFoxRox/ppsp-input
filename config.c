#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include <linux/input.h>

#include "config.h"

#define CALIBRATION		0
#define MAPPING_ANALOG 	1
#define MAPPING_DIGITAL 2

Config config;

struct Option
{
    const char* name;
    void*  data;
	int		type;
    const char* initial;
};

struct InputName
{
	int map;
	const char *name;
};

InputName nameMap[] = 
{
	{ANALOG_LEFT_UP, "ANALOG LEFT UP"},
	{ANALOG_LEFT_DOWN, "ANALOG LEFT DOWN"},
	{ANALOG_LEFT_LEFT, "ANALOG LEFT LEFT"},
	{ANALOG_LEFT_RIGHT, "ANALOG LEFT RIGHT"},
	{ANALOG_RIGHT_UP, "ANALOG RIGHT UP"},
	{ANALOG_RIGHT_DOWN, "ANALOG RIGHT DOWN"},
	{ANALOG_RIGHT_LEFT, "ANALOG RIGHT LEFT"},
	{ANALOG_RIGHT_RIGHT, "ANALOG RIGHT RIGHT"},
	{KEY_RIGHT, "BUTTON DPAD RIGHT"},
	{KEY_LEFT, "BUTTON DPAD LEFT"},
	{KEY_UP, "BUTTON DPAD UP"},
	{KEY_DOWN, "BUTTON DPAD DOWN"},
	{KEY_LEFTALT, "BUTTON START"},
	{KEY_LEFTCTRL, "BUTTON SELECT"},
	{KEY_MENU, "BUTTON MENU"},
	{KEY_PAGEDOWN, "BUTTON DOWN"},
	{KEY_END, "BUTTON RIGHT"},
	{KEY_HOME, "BUTTON LEFT"},
	{KEY_PAGEUP, "BUTTON UP"},
	{KEY_RIGHTCTRL, "BUTTON R"},
	{KEY_RIGHTSHIFT, "BUTTON L"},
	{KEY_1, "KBOARD 1"}, {KEY_2, "KBOARD 2"}, {KEY_3, "KBOARD 3"},
	{KEY_4, "KBOARD 4"}, {KEY_5, "KBOARD 5"}, {KEY_6, "KBOARD 6"},
	{KEY_7, "KBOARD 7"}, {KEY_8, "KBOARD 8"}, {KEY_9, "KBOARD 9"},
	{KEY_A, "KBOARD A"}, {KEY_B, "KBOARD B"}, {KEY_C, "KBOARD C"},
	{KEY_D, "KBOARD D"}, {KEY_E, "KBOARD E"}, {KEY_F, "KBOARD F"},
	{KEY_G, "KBOARD G"}, {KEY_H, "KBOARD H"}, {KEY_I, "KBOARD I"},
	{KEY_J, "KBOARD J"}, {KEY_K, "KBOARD K"}, {KEY_L, "KBOARD L"},
	{KEY_M, "KBOARD M"}, {KEY_N, "KBOARD N"}, {KEY_O, "KBOARD O"},
	{KEY_P, "KBOARD P"}, {KEY_Q, "KBOARD Q"}, {KEY_R, "KBOARD R"},
	{KEY_S, "KBOARD S"}, {KEY_T, "KBOARD T"}, {KEY_U, "KBOARD U"},
	{KEY_V, "KBOARD V"}, {KEY_W, "KBOARD W"}, {KEY_X, "KBOARD X"},
	{KEY_Y, "KBOARD Y"}, {KEY_Z, "KBOARD Z"}, 
	{KEY_LEFTSHIFT, "KBOARD LEFTSHIFT"},
	{KEY_ENTER, "KBOARD ENTER"},
	{KEY_SPACE, "KBOARD SPACE"},
	{KEY_COMMA, "KBOARD COMMA"},
	{KEY_DOT, "KBOARD DOT"},
	{KEY_BACKSPACE, "KBOARD BACKSPACE"},
	{KEY_ESC, "KBOARD ESC"},
	{KEY_MINUS, "KBOARD MINUS"},
	{KEY_EQUAL, "KBOARD EQUAL"},
	{KEY_TAB, "KBOARD TAB"},
	{KEY_LEFTBRACE, "KBOARD LEFTBRACE"},
	{KEY_RIGHTBRACE, "KBOARD RIGHTBRACE"},
	{KEY_SEMICOLON, "KBOARD SEMICOLON"},
	{KEY_APOSTROPHE, "KBOARD APOSTROPHE"},
	{KEY_GRAVE, "KBOARD GRAVE"},
	{KEY_BACKSLASH, "KBOARD BACKSLASH"},
	{KEY_SLASH, "KBOARD SLASH"},
	{KEY_F1, "KBOARD F1"},	{KEY_F2, "KBOARD F2"},	{KEY_F3, "KBOARD F3"},
	{KEY_F4, "KBOARD F4"}, {KEY_F5, "KBOARD F5"},	{KEY_F6, "KBOARD F6"},
	{KEY_F7, "KBOARD F7"}, {KEY_F8, "KBOARD F8"},	{KEY_F9, "KBOARD F9"},
};

const int nameMapSize = sizeof(nameMap) / sizeof(InputName);

const char* mapToName(int map)
{
	for(int i=0; i<nameMapSize; i++)
	{
		InputName *in = &nameMap[i];
		if (in->map == map) return (in->name);
	}
	return "ERROR";
}

int nameToMap(char *name)
{
	for(int i=0; i<nameMapSize; i++)
	{
		InputName *in = &nameMap[i];
		if (strcasecmp(in->name, name) == 0) return (in->map);
	}
	return -1;
}

Option configOptions[] =
{
    {"#PPSP Input Plugin for N64",  NULL, 0, 0},
    {"#by JayFoxRox and Adventus.", NULL, 0, 0},

    {"#These values specify the analog mapping:", NULL, 0},

    {"N64 ANALOG", &config.analogMap, MAPPING_ANALOG, "ANALOG LEFT"},
    
	{"#These values specify the button mapping:", NULL, 0},

    {"N64 DPAD UP", &config.buttonMap[N64_UP], MAPPING_DIGITAL, "BUTTON DPAD UP"},
    {"N64 DPAD DOWN", &config.buttonMap[N64_DOWN], MAPPING_DIGITAL, "BUTTON DPAD DOWN"},
    {"N64 DPAD LEFT", &config.buttonMap[N64_LEFT], MAPPING_DIGITAL, "BUTTON DPAD LEFT"},
    {"N64 DPAD RIGHT", &config.buttonMap[N64_RIGHT], MAPPING_DIGITAL, "BUTTON DPAD RIGHT"},
    {"N64 A", &config.buttonMap[N64_A], MAPPING_DIGITAL, "BUTTON RIGHT"},
    {"N64 B", &config.buttonMap[N64_B], MAPPING_DIGITAL, "BUTTON DOWN"},
    {"N64 L", &config.buttonMap[N64_L], MAPPING_DIGITAL, "KBOARD P"},
    {"N64 R", &config.buttonMap[N64_R], MAPPING_DIGITAL, "BUTTON R"},
    {"N64 Z", &config.buttonMap[N64_Z], MAPPING_DIGITAL, "BUTTON L"},
    {"N64 C UP", &config.buttonMap[N64_C_UP], MAPPING_DIGITAL, "ANALOG RIGHT UP : 64"},
    {"N64 C DOWN", &config.buttonMap[N64_C_DOWN], MAPPING_DIGITAL, "ANALOG RIGHT DOWN : 64"},
    {"N64 C LEFT", &config.buttonMap[N64_C_LEFT], MAPPING_DIGITAL, "ANALOG RIGHT LEFT : 64"},
    {"N64 C RIGHT", &config.buttonMap[N64_C_RIGHT], MAPPING_DIGITAL, "ANALOG RIGHT RIGHT : 64"},
    {"N64 START", &config.buttonMap[N64_START], MAPPING_DIGITAL, "BUTTON START"},
    {"EXIT EMULATOR", &config.exitMap, MAPPING_DIGITAL, "BUTTON MENU"},
	
    {"#These values specify the analog calibration function", NULL, 0, 0},
    {"#in = [0,255], out = [0, 255]", NULL, 0, 0},
    {"#out = in*v[0] + in^2 * v[1] + in^3 * v[2] ....", NULL, 0, 0},

    {"CALIB LEFT[0]", &config.calibLeft[0], CALIBRATION, "0.5"},
    {"CALIB LEFT[1]", &config.calibLeft[1], CALIBRATION, "0.5"},
    {"CALIB LEFT[2]", &config.calibLeft[2], CALIBRATION, "0.0"},
    {"CALIB LEFT[3]", &config.calibLeft[3], CALIBRATION, "0.0"},
    {"CALIB RIGHT[0]", &config.calibRight[0], CALIBRATION, "0.5"},
    {"CALIB RIGHT[1]", &config.calibRight[1], CALIBRATION, "0.5"},
    {"CALIB RIGHT[2]", &config.calibRight[2], CALIBRATION, "0.0"},
    {"CALIB RIGHT[3]", &config.calibRight[3], CALIBRATION, "0.0"},

};

const int configOptionsSize = sizeof(configOptions) / sizeof(Option);

int getType(char *val)
{
	if (strncasecmp("ANALOG", val, 6) == 0)
		return ANALOG;
	else if (strncasecmp("BUTTON", val, 6) == 0)
		return BUTTON;
	else if (strncasecmp("KBOARD", val, 6) == 0)
		return KBOARD;
	
	return -1;
}


void parseConfigOption(char *val, Option *o)
{
	char tmp[255];
	strcpy(tmp, val);
	
	if (!o->data) return;
	
	switch(o->type)
	{
		case CALIBRATION:
			*(float*)(o->data) = atof(tmp);
			break;
			
		case MAPPING_DIGITAL:
		{
			ButtonMap newbm;
			newbm.threshold = 0;
			newbm.type = getType(tmp);
			if (newbm.type == ANALOG)
			{
				char *th = strchr(tmp,':');
				if (th)
				{
					newbm.threshold = atoi(th+2);
					*(th-1) = '\0';
				}
			}

			newbm.mapping = nameToMap(tmp);
			if (newbm.mapping != -1) *(ButtonMap*)o->data = newbm;
			printf("%s, %s type %i mapped to %i with threshold %i\n",o->name, tmp, newbm.type, newbm.mapping, newbm.threshold);
			break;
		}
		
		case MAPPING_ANALOG:
		{
			AnalogMap newam;
			newam.velocity = 0;
			newam.mapping = -1;
			char *v = strchr(tmp,':');
			if (v)
			{
				newam.velocity = atof(v+2);
				*(v-1) = '\0';
			}

			if (strcasecmp(tmp, "ANALOG LEFT") == 0)
				newam.mapping = 0;
			else if (strcasecmp(tmp, "ANALOG RIGHT") == 0)
				newam.mapping = 1;
			else if (strcasecmp(tmp, "BUTTON DPAD") == 0) 
				newam.mapping = 2;

			if (newam.mapping != -1) *(AnalogMap*)o->data = newam;
			printf("%s, %s mapped to %i with velocity %f\n",o->name, tmp, newam.mapping, newam.velocity);

			break;
		}
	}

}

extern char configdir[PATH_MAX];

void config_setDefault()
{
    for(int i=0; i < configOptionsSize; i++)
    {
        Option *o = &configOptions[i];
        if (o->data) parseConfigOption((char*)o->initial, o);	
	}

}

void config_read()
{	
	FILE *f;
    char line[4096];

    // read configuration
    printf("[ppsp_input]: loading config\n");
	
    // default configuration
	config_setDefault();
	
    // read configuration
    char filename[PATH_MAX];
    snprintf( filename, PATH_MAX, "%s/ppsp_input.conf", configdir);
    f = fopen( filename, "r" );
    if (!f)
    {
        config_write(filename);
    }
    else
    {
		printf("[ppsp_input]: Found config %s\n", filename);

        while (!feof( f ))
        {
            char *val;
			int i;
			
            fgets( line, 4096, f );

            if (line[0] == '#' || line[0] == '\n')
                continue;

            val = strchr( line, '=' );
            if (!val) continue;

            *val++ = '\0';
			val[strlen(val)-1] = '\0';

            for(i = 0; i< configOptionsSize; i++)
            {
                Option *o = &configOptions[i];
                if (strcasecmp(line, o->name) == 0)
                {
					parseConfigOption(val, o);
                    break;
                }
            }
			
			if (i == configOptionsSize)
			{
				printf("Could not find option: %s\n", line);
			}

        }
    }
    if (f) fclose(f);
}

void config_write(const char *filename)
{
    FILE* f = fopen(filename, "w");
    if (!f)
    {
        printf("Could not open %s for writing\n", filename);
    }

    for(int i=0; i<configOptionsSize; i++)
    {
        Option *o = &configOptions[i];
        fprintf(f, o->name);		
        if (o->data) 
		{
			if (o->type == MAPPING_DIGITAL)
			{
				ButtonMap *bm = (ButtonMap*)o->data;
				fprintf(f,"=%s", mapToName(bm->mapping));	
				if (bm->threshold > 0)
					fprintf(f," : %i", bm->threshold);	

			} else if (o->type == MAPPING_ANALOG) {
				
				AnalogMap *am = (AnalogMap*)o->data;
				switch(am->mapping)
				{
					case 0: fprintf(f,"=ANALOG LEFT"); break;	
					case 1: fprintf(f,"=ANALOG RIGHT"); break;	
					case 2: fprintf(f,"=BUTTON DPAD : %f", am->velocity); break;
				}
							
			} else if (o->type == CALIBRATION) {
				fprintf(f,"=%f", *(float*)o->data);	
			}
			
        }
		fprintf(f, "\n");
    }

    fclose(f);
}

