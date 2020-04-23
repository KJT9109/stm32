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
#include "bool.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"

#include "cmsis_os.h"

#include "spi.h"
#include "Camera_function.h"
#include "gpio.h"




void Camera_Capture(struct netconn *cmraconn);
void first_picture_data();
void second_picture_data(struct netconn *cmraconn);
void Camera_interface_test(struct netconn *cmraconn);
#endif /* CAMERA_MOTION_H_ */
