/*
* Camera_motion.c
*
*  Created on: Dec 29, 2019
*      Author: jitae.kim
*  Reference : https://github.com/ArduCAM/Arduino
*  License :https://github.com/ArduCAM/Arduino/blob/master/LICENSE
*
*  This code was written according to the MIT license
*  designated by the Lead author.
*/
#include "Camera_motion.h"

#define SEND_SIZE 1460

char cmra_buf[2920];
bool picture_flag;
bool camera_flag;
uint8_t temp, temp_last,spi_dummy;
uint32_t Camera_Module_length, Real_length;
uint16_t cmra_flag;


void Camera_Capture(struct netconn *cmraconn)
{
  flush_fifo();
  clear_fifo_flag();
  HAL_Delay(1);
  fstart_capture();
  camera_flag = true;
  /****** Camera_flag***********/
  while (camera_flag)
  {
    if (get_bit(0x41, 0x08))
    {
#ifdef DEBUG
      printf("get_bit is true \n\r");
#endif
      Camera_Module_length = read_fifo_length();
      first_picture_data();
      second_picture_data(cmraconn);
      
      camera_flag = false;
      
    }
    
  }
  
  
}

void first_picture_data()
{
  
  uint8_t spi_dummy;
  
  cmra_flag = 0;
  Real_length = 0;
  cmra_buf[cmra_flag++] = 'T';
  cmra_buf[cmra_flag++] = 'E';
  cmra_buf[cmra_flag++] = 'S';
  cmra_buf[cmra_flag++] = 'T';
  
  set_fifo_burst();
  
  
  while (1)
  {
    temp_last = temp;
    
    HAL_SPI_TransmitReceive(&hspi3, (uint8_t*) &spi_dummy, &temp, 1, 1000);
    
    if ((temp == 0xD8) && (temp_last == 0xFF))
    {
      cmra_buf[cmra_flag++] = temp_last;
      Real_length++;
      cmra_buf[cmra_flag++] = temp;
      Real_length++;
      picture_flag = true;
      break;
      
    }
    
  }
}

void second_picture_data(struct netconn *cmraconn)
{
  while (picture_flag)
  {
    
    temp_last = temp;
    
    HAL_SPI_TransmitReceive(&hspi3, (uint8_t*) &spi_dummy, &temp, 1, 1000);
    
    cmra_buf[cmra_flag++] = temp;
    Real_length++;
    
    if ((temp == 0xD9) && (temp_last == 0xFF))
    {
      picture_flag = false;
      cmra_buf[cmra_flag++] = 0xFF;
      cmra_buf[cmra_flag++] = 0xFF;
      cmra_buf[cmra_flag++] = 0xFF;
      cmra_buf[cmra_flag++] = 0xFF;
      cmra_buf[cmra_flag++] = Real_length >> 24;
      cmra_buf[cmra_flag++] = Real_length >> 16;
      cmra_buf[cmra_flag++] = Real_length >> 8;
      cmra_buf[cmra_flag++] = Real_length;
      
      clear_fifo_flag();
      netconn_write(cmraconn,cmra_buf,cmra_flag,NETCONN_COPY);
      cmra_flag = 0;
      
      
    }
    
    
    if (cmra_flag == SEND_SIZE)
    {
      netconn_write(cmraconn,cmra_buf,cmra_flag,NETCONN_COPY);
      cmra_flag = 0;
      osDelay(1);
    }
    
  }
  
}

void Camera_interface_test(struct netconn *cmraconn)
{
  
  uint8_t Camera_pin_check;


  
  /********** SPI INTERFACE test ***************/
  
  write_reg(0x00, 0x88);
  Camera_pin_check = read_reg(0x00);
  
  if (Camera_pin_check == 0x88)
  {
    char *data = "Camera SPI Connected";
    netconn_write(cmraconn,data,sizeof("Camera SPI Connected"),NETCONN_COPY);
  }
  else
  {
    char *data = "Camera SPI Disconnected";
    netconn_write(cmraconn,data,sizeof("Camera SPI DisConnected"),NETCONN_COPY);
  }
    
  
  
  /********** SPI INTERFACE test END ***************/
  
  /********** I2C INTERFACE test ******************/
  
  rdSensorReg16_8(0x300a, &Camera_pin_check);
  if (Camera_pin_check == 0x56)
  {
    char *data = "Camera i2C Connected";
    netconn_write(cmraconn,data,sizeof("Camera i2C Connected"),NETCONN_NOFLAG);
    
  }
  else
  {
    char *data = "Camera i2C Disconnected";
    netconn_write(cmraconn,data,sizeof("Camera i2C Disconnected"),NETCONN_NOFLAG);   
    
  }
  osDelay(100);
  
  
  /********** I2C INTERFACE test END ******************/
  
}

