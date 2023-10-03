// RDKRL78_spi.c
#include <stdbool.h>
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "spi.h"

extern volatile uint8_t G_CSI21_SendingData; //spi busy flag
extern volatile uint8_t G_CSI21_ReceivingData; //spi busy flag

uint8_t *SPI_CS_Port[] = {
    (uint8_t *)&P8,   // Left_Lever Tmag5170        P80
    (uint8_t *)&P14,  // Right_Lever Tmag5170       P142
};

uint8_t SPI_CS_Pin[] = {
    0,   // EINK-CS#       P80
    1,   // SD-CS          P142
};

void IO_Reset()
{
    int i = 0;
    
    //#warning RESET-IO must be inverted for actual HW
    P13 |= (1<<0); // Assert P130 (#RESET-IO)
    for (i=0;i<12000;i++);
    P13 &= ~(1<<0);  // Deassert P130 (#RESET-IO)
    for (i=0;i<12000;i++);
}

void SPI_Init()
{
    IO_Reset();
    
    R_CSI21_Start();
}

void SPI_CS_Start(uint8_t aDevice)
{
	*SPI_CS_Port[aDevice] &= ~(1<<SPI_CS_Pin[aDevice]);
}

void SPI_CS_End(uint8_t aDevice)
{
	*SPI_CS_Port[aDevice] |= (1<<SPI_CS_Pin[aDevice]);
}

void SPI_Send(uint8_t aDevice, uint8_t *aData, uint32_t aLength)
{
    uint8_t noRXData;
	G_CSI21_SendingData = 1;
	G_CSI21_ReceivingData = 0;
	
	SPI_CS_Start(aDevice);

    R_CSI21_Send_Receive(aData, aLength, &noRXData);
    while(G_CSI21_SendingData);
    
    SPI_CS_End(aDevice);
}

uint8_t SPI_SendReceive(uint8_t aDevice, uint8_t *aTXData, uint32_t aTXLength, uint8_t *aRXData)
{
    uint16_t timeout;
	G_CSI21_SendingData = 1;
	G_CSI21_ReceivingData = 1;
	
	SPI_CS_Start(aDevice);
	
    if(aDevice == SPI_SD)
        delay_ms(1);
    
    R_CSI21_Send_Receive(aTXData, aTXLength, aRXData);
    
    timeout = 0;
    while(timeout < 1000)
    {
        if(!(G_CSI21_SendingData || G_CSI21_ReceivingData))
            break;
        
        timeout++;
    }
    
	SPI_CS_End(aDevice);
    
    if(timeout >= 1000)
        return 0;
    else
        return 1;
}

