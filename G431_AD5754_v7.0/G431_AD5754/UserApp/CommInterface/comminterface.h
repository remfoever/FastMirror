#ifndef __COMMINTERFACE_H_
#define __COMMINTERFACE_H_

#include <main.h>

#define MODBUS_CRC_LEN 2



void HandleReceivedData(uint8_t *pframe,uint8_t len);

void sendDataCRC(uint8_t dataSize, uint8_t commend, uint8_t *data);

#endif // __COMMINTERFACE_H_
