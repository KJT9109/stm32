/*
 * LeptonAppDriver.h
 *
 *  Created on: 2021. 6. 21.
 *      Author: Jitae
 */

#ifndef INC_LEPTONAPPDRIVER_H_
#define INC_LEPTONAPPDRIVER_H_

#include "cmsis_os2.h"
#include "stdio.h"

#include "i2c.h"
#include "spi.h"

#define ADDRESS  (0x2A)

#define AGC      (0x01)
#define SYS      (0x02)
#define VID      (0x03)
#define OEM      (0x08)
#define GET      (0x00)
#define SET      (0x01)
#define RUN      (0x02)



#define LEPTON_SIGNAL           0x55
#define VIDEO_REQ_SIGNAL        ( LEPTON_SIGNAL << 0 )
#define LEPTON_CONFIG_SIGNAL    ( LEPTON_SIGNAL << 1 )

#endif /* INC_LEPTONAPPDRIVER_H_ */
