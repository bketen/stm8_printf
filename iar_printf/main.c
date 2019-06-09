/**
  ******************************************************************************
  * @file       main.c
  * @author     Burak Keten
  * @date       06-Jun-2019
  * @compiler   Iar
  * @brief      Printf
  ******************************************************************************
**/

#include <iostm8l152c6.h>
#include <intrinsics.h>
#include "stdio.h"

void Clock_Init(void);
void Usart_Init(void);
int putchar (int c);
void _delay(void);

void main(void)
{
  __disable_interrupt();
  Clock_Init();
  Usart_Init();
  __enable_interrupt();
  
  for(;;)
  {
    printf("Hello World\n");
    _delay();
  }
}

void Clock_Init(void)
{
   CLK_ICKCR = 0x01;            //High-speed internal RC oscillator ON
   while(!(CLK_ICKCR & 0x02));  //Wait until the HSI clock is ready
   CLK_SWR = 0x01;              //HSI selected as systemclock source
   while(CLK_SCSR!=0x01);       //Wait until stabilization for HSI Systemclock
   CLK_CKDIVR = 0x00;           //System clock source/1
}

void Usart_Init(void)
{
  CLK_PCKENR1 |= 0x20;     //Enabling clock for Usart1
  
  PC_DDR |= (1 << 3) | (0 << 2) ;       //3: Output     TX
                                        //2: Input      RX
  
  PC_CR1 |= (1 << 3) | (1 << 2) ;       //3: Push-pull
                                        //2: Input with pull-up
  
  PC_CR2 |= (1 << 3) | (0 << 2) ;       //3: Output speed up to 10 MHz
                                        //2: External interrupt disabled
  
  USART1_CR1 &= (0 << 2);       //Parity control disabled
  USART1_CR1 &= (0 << 4);       //Word Length: 1 Start bit, 8 Data bits, ‘n’ STOP bit
                                //(n depending on STOP[1:0] bits in the USART_CR3 register)
  
  USART1_CR3 &= (0 << 4) | (0 << 5);    //Bit [5:4] STOP: 1 STOP bit

  //Set Baudrate 57600
  USART1_BRR2 = 0x05;  //Set the MSB mantissa of USARTDIV
  USART1_BRR1 = 0x11;  //Set the LSB mantissa of USARTDIV 
  
  USART1_CR2 |= (1 << 2);       //Set the Receiver Enable bit
  USART1_CR2 |= (1 << 3);       //Set the Transmitter  Enable bit
  
  USART1_CR1 &= (0 << 5);       //USART Enable
}

int putchar (int c)
{
  USART1_DR = c;
  while(!(USART1_SR & 0x80));
  
  return (c);
}

void _delay(void)
{
  unsigned long int j=150000;
  while(j--);
}