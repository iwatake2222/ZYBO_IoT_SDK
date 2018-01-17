/*
 * DevUIOPWM.h
 *
 *  Created on: 2018/01/16
 *      Author: tak
 */

#ifndef SRC_DEVUIOPWM_H_
#define SRC_DEVUIOPWM_H_

#include "DevUIO.h"

class DevUIOPWM : public DevUIO
{
private:
	static const uint32_t REG_OFFSET_LED0_R = 0x0000;
	static const uint32_t REG_OFFSET_LED0_G = 0x0004;
	static const uint32_t REG_OFFSET_LED0_B = 0x0008;
	static const uint32_t REG_OFFSET_LED1_R = 0x000C;
	static const uint32_t REG_OFFSET_LED1_G = 0x0010;
	static const uint32_t REG_OFFSET_LED1_B = 0x0014;
	static const uint32_t DUTY_MAX = 255;	// duty ratio is 0/256 ~ 255/256

public:
	DevUIOPWM(const char* dev);
	virtual ~DevUIOPWM();

	// r,g,b is duty ratio (0 ~ 100)
	void setLed0(uint32_t r, uint32_t g, uint32_t b);
	void setLed1(uint32_t r, uint32_t g, uint32_t b);
};


#endif /* SRC_DEVUIOPWM_H_ */
