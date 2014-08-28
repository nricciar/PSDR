/* spi.c
 *
 * high level spi implementation
 *
 * Copyright 2013 RPH Engineering, VPI Engineering
 */

#include "spi.h"
#include <hal.h>
#include <stm32f4xx_hal_spi.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal.h>
#include "stm32f4xx.h"

void new_spi_init(void)
{
    GPIO_InitTypeDef gpioInitStructure;
    DMA_HandleTypeDef dma_is;

    HAL_SPI_MspInit(&SpiHandle);

    __SPI1_CLK_ENABLE();
    __DMA2_CLK_ENABLE();

    // SPI2 SCK and MOSI
    gpioInitStructure.Pin   = SPI1_SCK.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_AF_PP;
    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
    gpioInitStructure.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(SPI1_SCK.port, &gpioInitStructure);

    gpioInitStructure.Pin   = SPI1_MOSI.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_AF_PP;
    gpioInitStructure.Pull  = GPIO_NOPULL;
    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI1_MOSI.port, &gpioInitStructure);

    // SPI2 MISO
    gpioInitStructure.Pin   = SPI1_MISO.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_INPUT;
    gpioInitStructure.Pull = GPIO_PULLUP;
    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI1_MISO.port, &gpioInitStructure);

    // RFID NSS
    gpioInitStructure.Pin   = LCD_NSS.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInitStructure.Pull  = GPIO_NOPULL;
    gpioInitStructure.Alternate = 0;
    HAL_GPIO_Init(LCD_NSS.port, &gpioInitStructure);
    HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);       // TBD - should this be before init?

    SpiHandle.Instance = SPI1;
    SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
    SpiHandle.Init.Mode = SPI_MODE_MASTER;
    SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
    SpiHandle.Init.NSS = SPI_NSS_SOFT;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    SpiHandle.Init.CLKPhase = SPI_PHASE_2EDGE;
    SpiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
    SpiHandle.Init.CRCPolynomial = 7;
    HAL_SPI_Init(&SpiHandle);


    HAL_DMA_DeInit(&dma_is);
    dma_is.Instance = DMA2_Stream3;
	dma_is.Init.Channel = DMA_CHANNEL_3;
	dma_is.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dma_is.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dma_is.Init.MemInc = DMA_MINC_ENABLE;
	dma_is.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dma_is.Init.PeriphInc = DMA_PINC_DISABLE;
	dma_is.Init.Mode = DMA_NORMAL;
    dma_is.Init.Priority = DMA_PRIORITY_HIGH;
    dma_is.Init.MemBurst = DMA_MBURST_SINGLE;
    dma_is.Init.PeriphBurst = DMA_PBURST_SINGLE;
	dma_is.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	HAL_DMA_Init(&dma_is);

	// FIXME:  hdmatx is not correct, but im not sure what is either
	__HAL_LINKDMA(SpiHandle, hdmatx, dma_is);
}
void spi_init(void)
{
    SPI_InitTypeDef  spiInitStructure;
    GPIO_InitTypeDef gpioInitStructure;

    HAL_SPI_MspInit(&SpiHandle);

    __SPI1_CLK_ENABLE();

    // SPI2 SCK and MOSI
    gpioInitStructure.Pin   = SPI1_SCK.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_AF_PP;
    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
    gpioInitStructure.Pull  = GPIO_NOPULL;
    //gpioInitStructure.Alternate = 1;
    HAL_GPIO_Init(SPI1_SCK.port, &gpioInitStructure);

    gpioInitStructure.Pin   = SPI1_MOSI.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_AF_PP;
    gpioInitStructure.Pull  = GPIO_NOPULL;
    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI1_MOSI.port, &gpioInitStructure);

    // SPI2 MISO
    gpioInitStructure.Pin   = SPI1_MISO.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_INPUT;
    gpioInitStructure.Pull = GPIO_PULLUP;
    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI1_MISO.port, &gpioInitStructure);

    // RFID NSS
    gpioInitStructure.Pin   = LCD_NSS.pin;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    gpioInitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInitStructure.Pull  = GPIO_NOPULL;
    gpioInitStructure.Alternate = 0;
    HAL_GPIO_Init(LCD_NSS.port, &gpioInitStructure);
    HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);       // TBD - should this be before init?

//    gpioInitStructure.Pin   = LCD_NSS.pin;
//    gpioInitStructure.Speed = GPIO_SPEED_FAST;
//    gpioInitStructure.Mode  = GPIO_MODE_AF_PP;
//    gpioInitStructure.Pull  = GPIO_NOPULL;
//    gpioInitStructure.Alternate = GPIO_AF5_SPI1;
//    HAL_GPIO_Init(LCD_NSS.port, &gpioInitStructure);
//    //HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);       // TBD - should this be before init?

    SpiHandle.Instance 				  = SPI1;
    SpiHandle.Init.Direction 		  = SPI_DIRECTION_2LINES;
    SpiHandle.Init.Mode               = SPI_MODE_MASTER;
    SpiHandle.Init.DataSize           = SPI_DATASIZE_8BIT;
    SpiHandle.Init.CLKPolarity 		  = SPI_POLARITY_HIGH;
    SpiHandle.Init.CLKPhase			  = SPI_PHASE_2EDGE;
    SpiHandle.Init.NSS                = SPI_NSS_SOFT; //SPI_NSS_SOFT;
    SpiHandle.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_2;
    SpiHandle.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.CRCCalculation 	  = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.TIMode 			  = SPI_TIMODE_DISABLED;
    SpiHandle.Init.CRCPolynomial      = 7;

    if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
    {
      /* Initialization Error */
      //Error_Handler();
    }
}



//int spi_getSpi2Semaphore(void)
//{
//    // tries to take the spi2 semaphore
//    // returns 0 if semaphore successfully taken, -1 if not
//
//    int status  = 0;
//
//    if( spi2Semaphore > 0 )
//        spi2Semaphore = 0;
//    else
//        status = -1;
//
//    return status;
//}



//void spi_postSpi2Semaphore(void)
//{
//    // post (make available) spi2 semaphore
//
//    spi2Semaphore = 1;
//}
void spi_readWrite(SPI_HandleTypeDef SpiH, uint8_t* rxBuf, uint8_t* txBuf, int cnt)
{
	//HAL_SPI_TransmitReceive(&SpiHandle, txBuf, rxBuf, cnt, 1000);
	//HAL_SPI_Transmit(&SpiHandle, txBuf, cnt, 1);
	HAL_SPI_Transmit_DMA(&SpiHandle, txBuf, cnt);

//	//High, second edge
//	//We're going to bitbang it for now, I guess
//	HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 0);
//	HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);
//	int i, j;
//	for(i = 0; i < cnt; i++)
//	{
//		for(j = 0; j < 8; j++)
//		{
//			HAL_GPIO_WritePin(SPI1_MOSI.port, SPI1_MOSI.pin, (txBuf[i] >> (7 - j)) & 1);
//			HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 0);
//			HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 1);
//		}
//	}
//	HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);


}

//int spi_readWrite(SPI_TypeDef* SPIx, uint8_t *rxBuf, const uint8_t *txBuf, int cnt, uint16_t speed)
//{
//    // send data to the spi bus, read data in from the spi bus
//    // returns the number of bytes written/read
//
//    enum { maxTries = 5000 };
//
//    int      i;
//    uint32_t tries;
//
//    // since everything currently runs at the same speed, we will not set the speed each time
//    // we will leave the set speed line here in case that changes
//    //SPIx->CR1 = (SPIx->CR1 & -SPI_BaudRatePrescaler_256) | speed;
//
//    if( cnt >= 0 )
//    {
//        for( i = 0; i < cnt; ++i )
//        {
//            if( txBuf )
//            {
//                SPI_I2S_SendData(SPIx, *txBuf++);
//                HAL_spi
//            }
//            else
//            {
//                SPI_I2S_SendData(SPIx, 0xff);
//            }
//
//            tries = 0;
//            while( SPI_I2S_GetFlagStatus(SPIx, SPI_FLAG_RXNE /*SPI_I2S_FLAG_RXNE*/) == RESET  &&  tries < maxTries )
//                ++tries;
//
//            if( tries >= maxTries )
//            {
//                // did not get rx data in indication, re-init spi
//                spi_init();
//            }
//
//            if( rxBuf )
//            {
//                *rxBuf++ = SPI_I2S_ReceiveData(SPIx);
//            }
//            else
//            {
//                SPI_I2S_ReceiveData(SPIx);
//            }
//        }
//    }
//    else
//    {
//        i = -1;
//    }
//
//    return i;
//}
