
#include "../modules/spi.h"

static const uint16_t speeds[] = {
    [Spi::Speed::SLOW] = SPI_BaudRatePrescaler_64,
    [Spi::Speed::MEDIUM] = SPI_BaudRatePrescaler_8,
    [Spi::Speed::FAST] = SPI_BaudRatePrescaler_2
};

Spi::Spi(SPI_TypeDef* spix)
  : SPIx(spix)
{
  // PORT Clock should be enabled!
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  if (SPIx == SPI1)
  {
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
  else if (SPIx == SPI2)
  {
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  }
  else
  {
    return;
  }

  SPI_InitTypeDef SPI_InitStructure;
  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = speeds[Spi::Speed::SLOW];
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStructure);

  SPI_Cmd(SPIx, ENABLE);
}

Spi::~Spi()
{
}

int Spi::ReadWrite(uint8_t* rbuf, const uint8_t* tbuf,
                 int cnt, Spi::Speed speed)
{
  //SPI_RxFIFOThresholdConfig (SPIx, SPI_RxFIFOThreshold_QF);
  int i;
  SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | speeds[speed];

  //while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  for (i = 0; i < cnt; ++i)
  {
    if (tbuf)
    {
      SPI_I2S_SendData(SPIx, *tbuf++);
    }
    else
    {
      SPI_I2S_SendData(SPIx, 0xff);
    }
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    if (rbuf)
    {
      *rbuf++ = SPI_I2S_ReceiveData(SPIx);
    }
    else
    {
      SPI_I2S_ReceiveData(SPIx);
    }
  }
}

int Spi::ReadWrite16(uint16_t* rbuf, const uint16_t* tbuf,
                   int cnt, Spi::Speed speed)
{
  SPI_DataSizeConfig(SPIx, SPI_DataSize_16b);
  int i;
  SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | speeds[speed];

  for (i = 0; i < cnt; ++i)
  {
    if (tbuf)
    {
      SPI_I2S_SendData(SPIx, *tbuf++);
    }
    else
    {
      SPI_I2S_SendData(SPIx, 0xff);
    }
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    if (rbuf)
    {
      uint16_t data = SPI_I2S_ReceiveData(SPIx);
      *rbuf++ = data;
    }
    else
    {
      SPI_I2S_ReceiveData(SPIx);
    }
  }

  SPI_DataSizeConfig(SPIx, SPI_DataSize_8b);
}

void Spi::Test()
{
  uint8_t txbuf[4], rxbuf[4];
  uint16_t txbuf16[4], rxbuf16[4];

  int i = 0, j = 0;

  for (i = 0; i < 8; ++i)
   {
     for (j = 0; j < 4; ++j)
     {
       txbuf16[j] = i*4 + j + (i << 8);
       rxbuf16[j] = 0;
     }
     ReadWrite16(rxbuf16, txbuf16, 4, Spi::Speed::SLOW);
     for (j = 0; j < 4; ++j)
     {
       if (rxbuf16[j] != txbuf16[j])
       {
         //assert_failed((unsigned char*)(__FILE__), __LINE__);
       }
     }
   }

  for (i = 0; i < 8; ++i)
  {
    for (j = 0; j < 4; ++j)
      txbuf[j] = i*4 + j;
    ReadWrite(rxbuf, txbuf, 4, Spi::Speed::SLOW);
    for (j = 0; j < 4; ++j)
    {
      if (rxbuf[j] != txbuf[j])
      {
        //assert_failed((unsigned char*)(__FILE__), __LINE__);
      }
    }
  }
}
