//PPSP Input (Pandora-PSP Input) for N64 emulators
//(c) JayFoxRox 2009

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef __LINUX__
#include <windows.h>
#else
# include "winlnxdefs.h"
#endif

#include "Input_1.1.h"

#include "controller.h"
#include "map.h"
#include "config.h"

extern char pluginName[];
extern char configdir[PATH_MAX];

char pluginName[] = "PPSP Input 2";

char configdir[PATH_MAX] = {0};

void startup() {
  printf("[ppsp_input]: Starting up input driver!\n");
  hardware_controllerInitialize();
  usleep(1000*1000);
  config_read();
}

#ifndef __LINUX__

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
      startup();
    }
    return TRUE;
}
#else
void _init( void )
{
  startup();
}
#endif // !__LINUX__

EXPORT void CALL GetDllInfo ( PLUGIN_INFO * PluginInfo )
{
    PluginInfo->Version = 0x0101;
    PluginInfo->Type = PLUGIN_TYPE_CONTROLLER;
    strcpy( PluginInfo->Name, pluginName );
    PluginInfo->Reserved1 = FALSE;
    PluginInfo->Reserved2 = FALSE;

}

EXPORT void CALL InitiateControllers (CONTROL_INFO ControlInfo)
{
	ControlInfo.Controls[0].Present = TRUE;
	ControlInfo.Controls[0].RawData = FALSE;
	ControlInfo.Controls[0].Plugin = PLUGIN_MEMPAK;
	for(int i=1; i<4; i++)
	{	
		ControlInfo.Controls[i].Present = FALSE;
		ControlInfo.Controls[i].RawData = FALSE;
		ControlInfo.Controls[i].Plugin = PLUGIN_NONE;
	}

    printf("[ppsp_input]: Init. Controllers!\n");
    startup();
    usleep(1000*500);
}

EXPORT void CALL GetKeys(int Control, BUTTONS * Keys )
{
	hardware_controllerRefresh();
	pandoraToN64Digital();
	Keys->Value = 0x00000000;
	for(int i = 0; i < N64_TRASH; i++) {
		Keys->Value |= hardware_controllerGetButton(i)?(1 << i):0;
	}
	Keys->Value &= 0x0000FFFF; //Might have set a bit too much
	pandoraToN64Analog(&Keys->Value);
}

EXPORT void CALL SetConfigDir( char *configDir )
{
	strcpy(configdir, configDir);
}

EXPORT void CALL DllConfig ( HWND hParent )
{
    FILE *f = fopen("./config/ppsp_input.conf", "r" );
    if (!f)
	{
		config_setDefault();
		config_write("./config/ppsp_input.conf");
    }
	else
        fclose(f);

    system("mousepad ./config/ppsp_input.conf");
}
