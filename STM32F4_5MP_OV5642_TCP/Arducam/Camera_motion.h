/*
 * Camera_motion.h
 *
 *  Created on: Dec 29, 2019
 *      Author: jitae.kim
 *  Reference : https://github.com/ArduCAM/Arduino
 *  License :https://github.com/ArduCAM/Arduino/blob/master/LICENSE
 *
 *  This code was written according to the MIT license
 *  designated by the Lead author.
 */

#ifndef CAMERA_MOTION_H_
#define CAMERA_MOTION_H_

#include <stdio.h>
#include <string.h>
#include "lwip/tcp.h"
#include "bool.h"


char cmra_buf[2920];
bool picture_flag;
bool camera_flag;
uint8_t temp, temp_last,spi_dummy;
uint32_t Camera_Module_length, Real_length;
uint16_t cmra_flag;



void Camera_Capture(struct tcp_pcb *tpcb);
void first_picture_data();
void second_picture_data(struct tcp_pcb *tpcb);
void Camera_interface_test(struct tcp_pcb *tpcb);
#endif /* CAMERA_MOTION_H_ */
