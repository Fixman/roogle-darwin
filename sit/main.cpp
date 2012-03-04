/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include "Camera.h"
#include "Point.h"
#include "mjpg_streamer.h"
#include "minIni.h"
#include "LinuxCamera.h"
#include "ColorFinder.h"

#include "Action.h"
#include "Head.h"
#include "Walking.h"
#include "MX28.h"
#include "MotionManager.h"
#include "LinuxMotionTimer.h"
#include "LinuxCM730.h"
#include "LinuxActionScript.h"

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../../../Data/motion_1024.bin"
#else
#define MOTION_FILE_PATH    "../../../../../Data/motion_4096.bin"
#endif

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

int main(void)
{
    printf( "\n===== Action script Tutorial for DARwIn =====\n\n");

    change_current_dir();

    Action::GetInstance()->LoadFile(MOTION_FILE_PATH);

    LinuxCM730 linux_cm730("/dev/ttyUSB0");
    CM730 cm730(&linux_cm730);
    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        printf("Fail to initialize Motion Manager!\n");
            return 0;
    }
    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    LinuxMotionTimer::Initialize(MotionManager::GetInstance());

    MotionManager::GetInstance()->SetEnable(true);

    while ( true )
    {
	    printf ("Number? ");
	    int g; scanf("%d", &g);
	    Action::GetInstance()->Start(g);
	    while(Action::GetInstance()->IsRunning()) usleep(8*1000);
    }

    return 0;
}
