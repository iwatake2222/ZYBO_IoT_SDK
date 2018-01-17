/*
 * DevUIOPWM.cpp
 *
 *  Created on: 2018/01/16
 *      Author: tak
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "DevUIOPWM.h"

DevUIOPWM::DevUIOPWM(const char* dev) : DevUIO(dev)
{
	writeReg(REG_OFFSET_LED0_R, 0);
	writeReg(REG_OFFSET_LED0_G, 0);
	writeReg(REG_OFFSET_LED0_B, 0);
	writeReg(REG_OFFSET_LED1_R, 0);
	writeReg(REG_OFFSET_LED1_G, 0);
	writeReg(REG_OFFSET_LED1_B, 0);
}

DevUIOPWM::~DevUIOPWM()
{
}

void DevUIOPWM::setLed0(uint32_t r, uint32_t g, uint32_t b)
{
	r = (r * DUTY_MAX) / 100 / 10;	// limit duty ratio to 10%
	g = (g * DUTY_MAX) / 100 / 10;	// limit duty ratio to 10%
	b = (b * DUTY_MAX) / 100 / 10;	// limit duty ratio to 10%
	writeReg(REG_OFFSET_LED0_R, r);
	writeReg(REG_OFFSET_LED0_G, g);
	writeReg(REG_OFFSET_LED0_B, b);
	printf("%d %d %d\n", r, g, b);
}

void DevUIOPWM::setLed1(uint32_t r, uint32_t g, uint32_t b)
{
	r = (r * DUTY_MAX) / 100 / 10;	// limit duty ratio to 10%
	g = (g * DUTY_MAX) / 100 / 10;	// limit duty ratio to 10%
	b = (b * DUTY_MAX) / 100 / 10;	// limit duty ratio to 10%
	writeReg(REG_OFFSET_LED1_R, r);
	writeReg(REG_OFFSET_LED1_G, g);
	writeReg(REG_OFFSET_LED1_B, b);
}


