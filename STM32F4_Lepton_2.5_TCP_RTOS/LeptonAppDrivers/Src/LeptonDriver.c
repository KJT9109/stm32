/*
 * LeptonDriver.c
 *
 *  Created on: 2021. 6. 21.
 *      Author: Jitae
 */

#include "LeptonAppDriver.h"



HAL_StatusTypeDef LeptonCommand(unsigned int moduleID, unsigned int commandID, unsigned int command)
{
  uint8_t write_data[2];
  uint16_t command_reg;

  command_reg = 0x00 << 8;
  command_reg |= 0x04;

  if (moduleID == 0x08)
  {
    write_data[0] = 0x48;
  }
  else
  {
    write_data[0] = moduleID & 0x0f;
  }

  write_data[1] = (((commandID << 2) & 0xfc) | (command & 0x3));

  return HAL_I2C_Mem_Write(&hi2c1, ADDRESS << 1, command_reg, (uint16_t) 2, write_data, 2, 0xffff);

}

uint16_t readReg(uint16_t command_reg)
{
  uint8_t read_data[2];
  uint16_t return_data;

  if (HAL_I2C_Mem_Read(&hi2c1, ADDRESS << 1, command_reg, 2, read_data, 2, 1000) != HAL_OK)
  {
    printf("I2c Error #0 \r\n");
  }

  return_data = read_data[0] << 8 | read_data[1];

  return return_data;
}

uint16_t readData(uint8_t *read_buf)
{
  int payload_length;

  while (readReg(0x02) & 0x01)
  {
    printf("Busy \r\n");
  }

  payload_length = readReg(0x06);
  printf("Payload_length is %d \r\n", payload_length);

  if (HAL_I2C_Master_Receive(&hi2c1, ADDRESS << 1, read_buf, payload_length, 1000) != HAL_OK)
  {
    printf("I2c Error #1 \r\n");
  }

  return 0;

}

void AGCEnable()
{
  uint16_t command_reg = 0x0105;
  uint8_t write_data[2];
  write_data[0] = 0x00;
  write_data[1] = 0x01;

  if (HAL_I2C_Mem_Write(&hi2c1, ADDRESS << 1, command_reg, (uint16_t) 2, write_data, 2, 1000)
      != HAL_OK)
  {
    printf("I2c Error #2 \r\n");
  }

}

/*TODO: Test용 Buffer */
uint8_t testRead_Buff[256];

void Lepton_init()
{
  readReg(0x02);

  /* SYS Camera Customer Serial Number */

  if (LeptonCommand(SYS, 0x28 >> 2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* SYS Flir Serial Number */
  if (LeptonCommand(SYS, 0x2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* SYS Camera Uptime */
  if (LeptonCommand(SYS, 0xC >> 2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* SYS Fpa Temperature Kelvin */
  if (LeptonCommand(SYS, 0x14 >> 2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* SYS Aux Temperature Kelvin */
  if (LeptonCommand(SYS, 0x10 >> 2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* OEM Chip Mask Revision */
  if (LeptonCommand(SYS, 0x14 >> 2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* OEM Camera Software Revision */
  if (LeptonCommand(OEM, 0x14 >> 2, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);

  /* AGC Enable */
  AGCEnable();

  /* AGC READ */
  if (LeptonCommand(AGC, 0x00, GET) != HAL_OK)
  {
    printf("ERROR \r\n");
  }

  readData(testRead_Buff);
}
