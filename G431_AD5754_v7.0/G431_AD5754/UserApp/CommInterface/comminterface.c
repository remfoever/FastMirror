#include "comminterface.h"
#include "usart.h"
#include <string.h>
#include "modbus.h"
#include "log.h"





void HandleReceivedData(uint8_t *pframe, uint8_t len)
{
	    // 1. Check the minimum frame length.
    if (len < 7) {
			EMLOG(LOG_DEBUG, "The frame length is incorrect.");
			return;
		}  // Slave Address | func code | data address hi | data address lo | data len(4~N) | data |CRC16 hi | CRC16 lo | MIN = 6

		else{
			EMLOG(LOG_DEBUG, "The frame length is correct.");
		}
		
		
    // 2. Check that the data frame length is correct
    uint8_t frame_size = pframe[4];
    if (frame_size != len + MODBUS_CRC_LEN) {
			
			EMLOG(LOG_DEBUG, "The data length is incorrect.");
			return;
		}

		else {
			EMLOG(LOG_DEBUG, "Data length correct.");
		}
		
    // 3. CRC check
    unsigned short received_crc = (pframe[len - 1] | (pframe[len - 2] << 8));
    if (Modbus_Crc16(pframe, len - 2) != received_crc) {
			EMLOG(LOG_DEBUG, "CRC check error.");
			return;
		}
		else {
			EMLOG(LOG_DEBUG, "CRC check correct.");
		}

    // 4. Parse commands
    uint8_t command = pframe[1];  // Get function code
		
		//5.  Parse data
		if (frame_size > 4){
			uint8_t* data_ptr = &pframe[5];  // data pointer
		}
		else{
			uint8_t* data_ptr = NULL;  // data pointer
		}
	
		
    switch (command) {
        case 0x01:  
					EMLOG(LOG_DEBUG, "COMMAND 1.");
//				memcpy(DAC_dataUnion.bytes, data_ptr, sizeof(AD5754Value));  // 拷贝数据到联合体
//				SetDacVol(0, DAC_dataUnion.dacValues.DAC_Value_A);
//				SetDacVol(1, DAC_dataUnion.dacValues.DAC_Value_B);
//				SetDacVol(2, DAC_dataUnion.dacValues.DAC_Value_C);
//				SetDacVol(3, DAC_dataUnion.dacValues.DAC_Value_D);
//				
//				DACBack_dataUnion.dacValues.DAC_Value_A = GetDacVol(0);
//				DACBack_dataUnion.dacValues.DAC_Value_B = GetDacVol(1);
//				DACBack_dataUnion.dacValues.DAC_Value_C = GetDacVol(2);
//				DACBack_dataUnion.dacValues.DAC_Value_D = GetDacVol(3);
//				
//				uint8_t rs485_send_data[sizeof(AD5754Value)];
//				memcpy(rs485_send_data, DACBack_dataUnion.bytes, sizeof(AD5754Value));
//				sendDataCRC(sizeof(rs485_send_data), 0x01, rs485_send_data);
            break;
        case 0x02:  
            EMLOG(LOG_DEBUG, "COMMAND 1.");
            break;
        default:
						EMLOG(LOG_DEBUG, "ERROR.");
            break;
    }
    
}













// 发送数据函数
void sendDataCRC(uint8_t dataSize, uint8_t commend, uint8_t *data)
{
    int frameSize = 6 + dataSize;  //帧长度 + 命令 + 数据部分大小 + CRC
    uint8_t send_data[frameSize];  // 用于存储整个数据帧

    send_data[1] = frameSize;
    send_data[2] = commend;

    // 填充数据
    memcpy(&send_data[3], data, dataSize);

    // 计算CRC
    uint16_t crc = Modbus_Crc16(send_data, frameSize - 3);  // CRC校验码计算
    send_data[frameSize - 3] = (uint8_t)(crc & 0xFF);  // CRC低字节
    send_data[frameSize - 2] = (uint8_t)((crc >> 8) & 0xFF);  // CRC高字节


    // 发送数据
    RS485_send_data(send_data, frameSize);
		
}





















