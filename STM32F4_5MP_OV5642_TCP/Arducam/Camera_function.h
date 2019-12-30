/*
 * Camera_fucntion.h
 *
 *  Created on: Dec 29, 2019
 *      Author: jitae.kim
 *  Reference : https://github.com/ArduCAM/Arduino
 *  License :https://github.com/ArduCAM/Arduino/blob/master/LICENSE
 *
 *  This code was written according to the MIT license
 *  designated by the Lead author.
 */

#ifndef CAMERA_FUNCTION_H_
#define CAMERA_FUNCTION_H_

#include "Arducam.h"

void set_format(uint16_t fmt);
uint32_t DWT_Delay_Init(void);
void DWT_Delay_us(volatile uint32_t microseconds);
void wrSensorReg16_8(uint16_t tx_addr, unsigned char rx_buf);
void rdSensorReg16_8(uint16_t tx_addr, uint8_t* rx_buf);
int wrSensorRegs16_8(const struct sensor_reg reglist[]);
void OV5642_set_JPEG_size(uint8_t size);
void initCAM();
void set_fifo_burst();
uint8_t bus_write(int address, int value);
void write_reg(uint8_t addr, uint8_t data);
uint8_t bus_read(int address);
uint8_t read_reg(uint8_t addr);
void set_bit(uint8_t addr, uint8_t bit);
uint8_t get_bit(uint8_t addr, uint8_t bit);
uint32_t read_fifo_length();
void flush_fifo(void);
void fstart_capture(void);
void clear_fifo_flag(void);
void clear_bit(uint8_t addr, uint8_t bit);



#endif /* CAMERA_FUNCTION_H_ */
