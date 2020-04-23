/*
 * Arducam.h
 *
 *  Created on: Dec 29, 2019
 *      Author: jitae.kim
 *  Reference : https://github.com/ArduCAM/Arduino
 *  License :https://github.com/ArduCAM/Arduino/blob/master/LICENSE
 *
 *  This code was written according to the MIT license
 *  designated by the Lead author.
 */

#ifndef ARDUCAM_H_
#define ARDUCAM_H_



#define ARDUCHIP_TEST1   0x00


#define BMP 	0
#define JPEG	1



struct sensor_reg 
{
  uint16_t reg;
  uint16_t val;


};

#define OV5642_320x240 		0	//320x240
#define OV5642_640x480		1	//640x480
#define OV5642_1024x768		2	//1024x768
#define OV5642_1280x960 	3	//1280x960
#define OV5642_1600x1200	4	//1600x1200
#define OV5642_2048x1536	5	//2048x1536
#define OV5642_2592x1944	6	//2592x1944






#endif /* ARDUCAM_H_ */
