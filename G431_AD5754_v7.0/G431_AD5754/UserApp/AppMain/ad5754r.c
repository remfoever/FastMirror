/********************************************************************************
 Author        :

 Date          :

 Hardware      :ad5754R
********************************************************************************/
#include "ad5754r.h"
#include "tim.h"
#include "spi.h"


#define DLY_TIM_Handle  (&htim4)


void delay_us(uint16_t nus)
{
	__HAL_TIM_SET_COUNTER(DLY_TIM_Handle, 0);
	__HAL_TIM_ENABLE(DLY_TIM_Handle);
	while (__HAL_TIM_GET_COUNTER(DLY_TIM_Handle) < nus)
	{
	}
	__HAL_TIM_DISABLE(DLY_TIM_Handle);
}



void AD5754_Init()
{
	AD5754_CS(1);
	AD5754_CLR(1);
	AD5754_LDAC(0);
}


void WriteToAD5754RViaSpi(uint32_t *RegisterData)
{
    uint8_t TxData[3];  
    uint32_t ValueToWrite = *RegisterData;
    
    TxData[0] = (ValueToWrite >> 16) & 0xFF;  
    TxData[1] = (ValueToWrite >> 8) & 0xFF;   
    TxData[2] = ValueToWrite & 0xFF;          

    AD5754_CS(0);  
    delay_us(20); 

    HAL_SPI_Transmit(&hspi2, TxData, 3, HAL_MAX_DELAY);  // 传输3个字节

    AD5754_CS(1);  
    delay_us(20);
}



/********************************************************************************
Function that read from the AD5754R via the SPI port.
********************************************************************************/

void ReadFromAD5754RViaSpi(uint32_t *RegisterData)
{
    uint8_t TxData[3]; 
    uint8_t RxData[3];  
		uint32_t RotateData = 0;
		uint32_t Noop = Nop;
	
	  // Nop(0x180000)
	  TxData[0] = (uint8_t)((Noop >> 16) & 0xFF);  // High
    TxData[1] = (uint8_t)((Noop >> 8) & 0xFF);   // Mid
    TxData[2] = (uint8_t)(Noop & 0xFF);          // Low

    // SPI start
    AD5754_CS(0);  
    delay_us(20); 

    HAL_SPI_TransmitReceive(&hspi2, TxData, RxData, 3, HAL_MAX_DELAY);

    RotateData = ((uint32_t)RxData[0] << 16) | ((uint32_t)RxData[1] << 8) | (uint32_t)RxData[2];
  
		Noop <<= 1;
	
	  delay_us(20); 
    // SPI end
    AD5754_CS(1);  
    delay_us(20); 

    *RegisterData = RotateData;
}


//---------------------
//Configure AD5754
//---------------------

void ConfigAD5754R(void)
{
		int i;
		uint32_t *p;
		uint32_t ins[2] = {0, 0};
		ins[0] = Power_Control_Register | PowerUp_ALL;
		ins[1] = Output_Range_Select_Register | Range2_Select | DAC_Channel_ALL;
		p = ins;
		for(i=0; i<2; i++)
		{ 
			WriteToAD5754RViaSpi(p);
			delay_us(200);
			p++;
		}
}


void SetDacVol(uint8_t ch, uint16_t val)
{
		uint32_t cstr = 0;

		if(val > 0xFFFF)
		{
			return;
		}	

		switch(ch)
		{
			case 0:
					cstr = DAC_Register | DAC_Channel_A | val;
			break;
			
			case 1:
					cstr = DAC_Register | DAC_Channel_B | val;
			break;
			
			case 2:
					cstr = DAC_Register | DAC_Channel_C | val;
			break;
			
			case 3:
					cstr = DAC_Register | DAC_Channel_D | val;
			break;
			
			case 4:
					cstr = DAC_Register | DAC_Channel_ALL | val;				
			break;
			default:
				break;
		}
		WriteToAD5754RViaSpi(&cstr);
		
		cstr = Load;
		WriteToAD5754RViaSpi(&cstr);
		return;
}




uint16_t GetDacVol(uint8_t ch) 
{
    uint32_t readCommand;
    uint32_t registerData = 0;
    uint16_t voltage;
    // 根据通道选择对应的寄存器地址
    switch (ch) 
    {
        case 0:
            readCommand = DAC_Register | DAC_Channel_A |  Read;  // R/W位设置为1以启用回读
            break;
        case 1:
            readCommand = DAC_Register | DAC_Channel_B |  Read;
            break;
        case 2:
            readCommand = DAC_Register | DAC_Channel_C |  Read;
            break;
        case 3:
            readCommand = DAC_Register | DAC_Channel_D |  Read;
            break;
        default:
            return 0;
    }
    WriteToAD5754RViaSpi(&readCommand);

		delay_us(20);
		
    ReadFromAD5754RViaSpi(&registerData);

		voltage = (uint16_t)registerData;
		
    return voltage;
}





