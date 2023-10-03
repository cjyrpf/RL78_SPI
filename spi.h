// RDKRL78_spi.h
#ifndef _RDKRL78_SPI_H
#define _RDKRL78_SPI_H

#define Left_Lever Tmag5170         0
#define Right_Lever Tmag5170        1

void SPI_Init();
void SPI_Send(uint8_t aDevice, uint8_t *aData, uint32_t aLength);
uint8_t SPI_SendReceive(uint8_t aDevice, uint8_t *aTXData, uint32_t aTXLength, uint8_t *aRXData);

#endif // _RSPI_H