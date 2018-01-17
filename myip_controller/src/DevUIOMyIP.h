/*
 * DevUIOMyIP.h
 *
 *  Created on: 2018/01/16
 *      Author: tak
 */

#ifndef SRC_DEVUIOMYIP_H_
#define SRC_DEVUIOMYIP_H_

#include "DevUIO.h"

class DevUIOMyIP : public DevUIO
{
private:
	static const uint32_t REG_OFFSET_LED = 0x0000;
	static const uint32_t REG_OFFSET_BTN = 0x0004;
	static const uint32_t REG_OFFSET_SW  = 0x0008;

public:
	DevUIOMyIP(const char* dev);
	virtual ~DevUIOMyIP();

	void setLed(uint32_t led);
	void setLedBit(uint32_t bit);
	void clrLedBit(uint32_t bit);
	uint32_t getBtn();
	uint32_t getSw();
};



#endif /* SRC_DEVUIOMYIP_H_ */
