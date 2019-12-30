/*
 * Camera_function.c
 *
 *  Created on: Dec 29, 2019
 *      Author: jitae.kim
 *  Reference : https://github.com/ArduCAM/Arduino
 *  License :https://github.com/ArduCAM/Arduino/blob/master/LICENSE
 *
 *  This code was written according to the MIT license
 *  designated by the Lead author.
 */

#include "bool.h"
#include "stdint.h"
#include "Arducam.h"
#include "stm32f4xx_hal.h"
#include "Camera_function.h"
#include "ov5642_reg.h"
#include "spi.h"
#include "i2c.h"
#include "gpio.h"

#include "debug_mode.h"

void
set_format (uint16_t fmt)
{
  if (fmt == BMP)
    {
      m_fmt = BMP;

    }
  else
    {
      m_fmt = JPEG;

    }

}

uint32_t
DWT_Delay_Init (void)
{
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

  /* 3 NO OPERATION instructions */
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
  if (DWT->CYCCNT)
    {
      return 0; /*clock cycle counter started*/
    }
  else
    {
      return 1; /*clock cycle counter not started*/
    }
}

void
DWT_Delay_us (volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq () / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds)
    ;
}

void
wrSensorReg16_8 (uint16_t tx_addr, unsigned char rx_buf)
{

  if(HAL_I2C_Mem_Write (&hi2c1, 0x78, tx_addr, 2, &rx_buf, 1, 1000) != HAL_OK)
    {
	#ifdef DEBUG
	printf("I2c Write Problem \n\r");
	#endif

    }
  HAL_Delay (1);

}

void
rdSensorReg16_8 (uint16_t tx_addr, uint8_t* rx_buf)
{

  if((HAL_I2C_Mem_Read(&hi2c1, 0x78, tx_addr, 2, rx_buf, 1, 1000))!= HAL_OK)
    {
	#ifdef DEBUG
	printf("I2c Read Problem \n\r");
	#endif
    }
  HAL_Delay (1);

}

int
wrSensorRegs16_8 (const struct sensor_reg reglist[])
{

  unsigned int reg_addr = 0;
  unsigned char reg_val = 0;
  const struct sensor_reg *next = reglist;

  while ((reg_addr != 0xffff) | (reg_val != 0xff))
    {
      reg_addr = next->reg;
      reg_val = next->val;

      if((HAL_I2C_Mem_Write(&hi2c1, 0x78, reg_addr, 2, &reg_val, 1, 1000))!= HAL_OK)
	{
	    #ifdef DEBUG
	    printf("I2c Write Problem \n\r");
	    #endif
	}
	next++;

      HAL_Delay (1);

    }
  return 1;

}

void
OV5642_set_JPEG_size (uint8_t size)
{

  switch (size)
    {
    case 0:
      wrSensorRegs16_8 (ov5642_320x240);
      break;
    case 1:
      wrSensorRegs16_8 (ov5642_640x480);
      break;
    case 2:
      wrSensorRegs16_8 (ov5642_1024x768);
      break;
    case 3:
      wrSensorRegs16_8 (ov5642_1280x960);
      break;
    case 4:
      wrSensorRegs16_8 (ov5642_1600x1200);
      break;
    case 5:
      wrSensorRegs16_8 (ov5642_2048x1536);
      break;
    case 6:
      wrSensorRegs16_8 (ov5642_2592x1944);
      break;
    default:
      wrSensorRegs16_8 (ov5642_320x240);
      break;
    }

}

void
initCAM ()
{

  uint16_t reg_val;
  wrSensorReg16_8 (0x3008, 0x80);
  wrSensorRegs16_8 (OV5642_QVGA_Preview);

  HAL_Delay (100);

  if (m_fmt == JPEG)
    {
      wrSensorRegs16_8 (OV5642_JPEG_Capture_QSXGA);
      wrSensorRegs16_8 (ov5642_320x240);
      HAL_Delay (100);
      wrSensorReg16_8 (0x3818, 0xa8);
      wrSensorReg16_8 (0x3621, 0x10);
      wrSensorReg16_8 (0x3801, 0xb0);
      wrSensorReg16_8 (0x4407, 0x04);
    }
  else
    {
      wrSensorReg16_8 (0x4740, 0x21);
      wrSensorReg16_8 (0x501e, 0x2a);
      wrSensorReg16_8 (0x5002, 0xf8);
      wrSensorReg16_8 (0x501f, 0x01);
      wrSensorReg16_8 (0x4300, 0x61);
      rdSensorReg16_8 (0x3818, (uint8_t*) &reg_val);
      wrSensorReg16_8 (0x3818, (reg_val | 0x60) & 0xff);
      rdSensorReg16_8 (0x3621, (uint8_t*) &reg_val);
      wrSensorReg16_8 (0x3621, reg_val & 0xdf);
    }

}

void
set_fifo_burst ()
{
  uint8_t burst = 0x3C;
  uint8_t value;
  HAL_SPI_TransmitReceive (&hspi3, (uint8_t*) &burst, &value, 1, 1000);

}
uint8_t
bus_write (int address, int value)
{

  GPIOC->BSRR = CS_Pin << 16;

  HAL_SPI_Transmit (&hspi3, (uint8_t*) &address, 1, 1000);
  HAL_SPI_Transmit (&hspi3, (uint8_t*) &value, 1, 1000);

  GPIOC->BSRR = CS_Pin;

  return 1;

}

void
write_reg (uint8_t addr, uint8_t data)
{

  bus_write (addr | 0x80, data);

}

uint8_t
bus_read (int address)
{
  uint8_t value;
  int dummy = 0x00;

  GPIOC->BSRR = CS_Pin << 16;

  HAL_SPI_Transmit (&hspi3, (uint8_t*) &address, 1, 1000);

  HAL_SPI_TransmitReceive (&hspi3, (uint8_t*) &dummy, &value, 1, 1000);

  GPIOC->BSRR = CS_Pin;

  return value;
}

uint8_t
read_reg (uint8_t addr)
{
  uint8_t data;

  data = bus_read (addr & 0x7F);

  return data;
}

void
set_bit (uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = read_reg (addr);
  write_reg (addr, temp | bit);

}

uint8_t
get_bit (uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = read_reg (addr);

  temp = temp & bit;

  return temp;

}
uint32_t
read_fifo_length ()
{
  uint8_t len1, len2, len3 = 0;

  uint32_t length = 0;
  len1 = read_reg (0x42);
  len2 = read_reg (0x43);
  len3 = read_reg (0x44) & 0x7f;
  length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
  return length;

}

void
flush_fifo (void)
{

  write_reg (0x04, 0x01);

}

void
fstart_capture (void)
{

  write_reg (0x04, 0x02);

}

void
clear_fifo_flag (void)
{

  write_reg (0x04, 0x01);

}

void
clear_bit (uint8_t addr, uint8_t bit)
{

  uint8_t temp;

  temp = read_reg (addr);

  write_reg (addr, temp & (~bit));

}

