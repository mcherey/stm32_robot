#include <stm32f10x_spi.h>

#pragma once

class Spi
{
public:
  enum Speed {SLOW, MEDIUM, FAST};
public:
  Spi(SPI_TypeDef* spix);
  virtual ~Spi();

  int ReadWrite(uint8_t* rbuf, const uint8_t* tbuf,
                int cnt, Spi::Speed speed);
  int ReadWrite16(uint16_t* rbuf, const uint16_t* tbuf,
                  int cnt, Spi::Speed speed);

  void Test();
private:
  SPI_TypeDef* SPIx;
};
