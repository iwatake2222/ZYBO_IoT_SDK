/*
 * DevUIO.h
 *
 *  Created on: 2018/01/16
 *      Author: tak
 */

#ifndef SRC_DEVUIO_H_
#define SRC_DEVUIO_H_

#include <cstdint>

class DevUIO
{
private:
	void* m_address;    /*/dev/uioXへの仮想アドレス(ユーザ空間) */
	int   m_fd;

public:
	DevUIO(const char* dev);
	virtual ~DevUIO();
	bool isValid();
	void writeReg(uint32_t offset, uint32_t data);
	uint32_t readReg(uint32_t offset);
};

#endif /* SRC_DEVUIO_H_ */
