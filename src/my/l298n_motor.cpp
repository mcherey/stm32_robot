#include "l298n_motor.h"

L298NMotor::L298NMotor(GPIO_TypeDef* gpio1, uint16_t enA, uint16_t inA1, uint16_t inA2,
                       uint16_t enB, uint16_t inB1, uint16_t inB2)
  : GPIOX(gpio1)
  , EnA(enA)
  , InA1(inA1)
  , InA2(inA2)
  , EnB(enB)
  , InB1(inB1)
  , InB2(inB2)
{
  GPIO_InitTypeDef gpioStruct;
  GPIO_StructInit(&gpioStruct);
  gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioStruct.GPIO_Pin = EnA;
  gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOX, &gpioStruct);

  GPIO_StructInit(&gpioStruct);
  gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioStruct.GPIO_Pin = InA1;
  gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOX, &gpioStruct);

  GPIO_StructInit(&gpioStruct);
  gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioStruct.GPIO_Pin = InA2;
  gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOX, &gpioStruct);

  GPIO_StructInit(&gpioStruct);
  gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioStruct.GPIO_Pin = EnB;
  gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOX, &gpioStruct);

  GPIO_StructInit(&gpioStruct);
  gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioStruct.GPIO_Pin = InB1;
  gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOX, &gpioStruct);

  GPIO_StructInit(&gpioStruct);
  gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioStruct.GPIO_Pin = InB2;
  gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOX, &gpioStruct);
}

L298NMotor::~L298NMotor()
{

}

void L298NMotor::LeftMotorsForward()
{
  GPIO_WriteBit(GPIOX, EnA, Bit_SET);
  GPIO_WriteBit(GPIOX, InA1, Bit_SET);
  GPIO_WriteBit(GPIOX, InA2, Bit_RESET);
}

void L298NMotor::LeftMotorsBack()
{
  GPIO_WriteBit(GPIOX, EnA, Bit_SET);
  GPIO_WriteBit(GPIOX, InA1, Bit_RESET);
  GPIO_WriteBit(GPIOX, InA2, Bit_SET);
}

void L298NMotor::RightMotorsForward()
{
  GPIO_WriteBit(GPIOX, EnB, Bit_SET);
  GPIO_WriteBit(GPIOX, InB1, Bit_SET);
  GPIO_WriteBit(GPIOX, InB2, Bit_RESET);
}

void L298NMotor::RightMotorsBack()
{
  GPIO_WriteBit(GPIOX, EnB, Bit_SET);
  GPIO_WriteBit(GPIOX, InB1, Bit_RESET);
  GPIO_WriteBit(GPIOX, InB2, Bit_SET);
}

void L298NMotor::Forward()
{
  LeftMotorsForward();
  RightMotorsForward();
}

void L298NMotor::Back()
{
  LeftMotorsBack();
  RightMotorsBack();
}

void L298NMotor::Right()
{
  LeftMotorsForward();
  RightMotorsBack();
}

void L298NMotor::Left()
{
  LeftMotorsBack();
  RightMotorsForward();
}

void L298NMotor::SoftStop()
{
  GPIO_WriteBit(GPIOX, EnA, Bit_RESET);
  GPIO_WriteBit(GPIOX, EnB, Bit_RESET);
}

void L298NMotor::HardStop()
{
  GPIO_WriteBit(GPIOX, InA1, Bit_RESET);
  GPIO_WriteBit(GPIOX, InA2, Bit_RESET);

  GPIO_WriteBit(GPIOX, InB1, Bit_RESET);
  GPIO_WriteBit(GPIOX, InB2, Bit_RESET);
}

