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

#define LEPTON_SIGNAL           0x55
#define VIDEO_REQ_SIGNAL        ( LEPTON_SIGNAL << 0 )
#define LEPTON_CONFIG_SIGNAL    ( LEPTON_SIGNAL << 1 )

#endif /* INC_LEPTONAPPDRIVER_H_ */
