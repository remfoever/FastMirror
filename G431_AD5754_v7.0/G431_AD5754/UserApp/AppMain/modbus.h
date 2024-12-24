#ifndef __MODBUS_H_
#define __MODBUS_H_

#include <stdint.h>

uint16_t Modbus_Crc16( uint8_t * pFrame, uint16_t len );
	

#endif	//__MODBUS_H_

