/*
 * DevUIOMyIP.cpp
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
#include "DevUIOMyIP.h"

DevUIOMyIP::DevUIOMyIP(const char* dev) : DevUIO(dev)
{
	writeReg(REG_OFFSET_LED, 0x00);
}

DevUIOMyIP::~DevUIOMyIP()
{
}


void DevUIOMyIP::setLed(uint32_t led)
{
	writeReg(REG_OFFSET_LED, led & 0x0F);
}

void DevUIOMyIP::setLedBit(uint32_t bit)
{
	uint32_t reg = readReg(REG_OFFSET_LED);
	writeReg(REG_OFFSET_LED, reg | (1 << bit));
}
void DevUIOMyIP::clrLedBit(uint32_t bit)
{
	uint32_t reg = readReg(REG_OFFSET_LED);
	writeReg(REG_OFFSET_LED, reg & ~(1 << bit));
}


uint32_t DevUIOMyIP::getBtn()
{
	return readReg(REG_OFFSET_BTN) & 0x0F;
}

uint32_t DevUIOMyIP::getSw()
{
	return readReg(REG_OFFSET_SW) & 0x0F;
}
