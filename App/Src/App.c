/*
 * App.c
 *
 *  Created on: May 10, 2024
 *      Author: matio
 */

#include "App.h"
#include "main.h"
#include "usbd_hid.h"


void App_Init()
{
	display_init();

	while(1)
	{
		Buttons_Check();
		LED_Update();
	}
}

