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
#include "Arducam.h"
#include "spi.h"
#include "Camera_function.h"
#include "gpio.h"
#include "tcp_server.h"
#include "lwip/tcp.h"

#include "debug_mode.h"

void Camera_Capture(struct tcp_pcb *tpcb)
{
	tcp_sent(tpcb, tcp_camera_sent);
	flush_fifo();
	clear_fifo_flag();
	HAL_Delay(1);
	fstart_capture();
	camera_flag = true;
	/****** Camera_flag***********/
	//while문을 통해 camera가 get_bit return 값이 1인지 계속 체크한다.
	while (camera_flag)
	{
		if (get_bit(0x41, 0x08))
		{
			#ifdef DEBUG
			printf("get_bit is 1 \n\r");
                        #endif
			Camera_Module_length = read_fifo_length();
			GPIOC->BSRR = CS_Pin << 16;
			first_picture_data();
			second_picture_data(tpcb);

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

void second_picture_data(struct tcp_pcb *tpcb)
{

	while (picture_flag)
	{

		temp_last = temp;

		HAL_SPI_TransmitReceive(&hspi3, (uint8_t*) &spi_dummy, &temp, 1, 1000);

		cmra_buf[cmra_flag++] = temp;
		Real_length++;

		if ((Camera_Module_length - 50 < Real_length) && (temp == 0xD9)
				&& (temp_last == 0xFF))
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
			tcp_write(tpcb, cmra_buf, cmra_flag, 1);
			cmra_flag = 0;
			tcp_sent(tpcb, tcp_server_sent);

		}

		/* 데이터가  측정 길이보다 짧게 나왔을 때 재 전송을 위한 구문
		 * 앞에 0xd9 0xff가 출력이 되지 않을 경우 0xAA가 출력 됨.
		 *
		 */
		else if ((Camera_Module_length - 50 < Real_length) && (temp == 0xAA)
				&& (temp_last == 0xAA))
		{
			flush_fifo();
			clear_fifo_flag();
			HAL_Delay(10);

			fstart_capture();
			/****** Camera_flag***********/
			//while문을 통해 camera가 get_bit return 값이 1인지 계속 체크한다.
			while (camera_flag)
			{
				if (get_bit(0x41, 0x08))
				{
					Camera_Module_length = read_fifo_length();
					GPIOC->BSRR = CS_Pin << 16;
					first_picture_data();
					camera_flag = false;

				}

			}
			camera_flag = true;

		}

		if (cmra_flag == 2910)
		{
			cmra_flag = 0;
			tcp_write(tpcb, cmra_buf, 2910, 1);
			break;

		}

	}

}

void Camera_interface_test(struct tcp_pcb *tpcb)
{

	uint8_t Camera_pin_check, stats, stats2, stats3[2];

	/********** SPI INTERFACE test ***************/

	write_reg(0x00, 0x88);
	Camera_pin_check = read_reg(0x00);

	if (Camera_pin_check == 0x88)
	{
		stats = 0x41;    // 연결 되어있을시 ascii =A

	}
	else
	{
		stats = 0x46;    //연결 실패시 ascii =F

	}

	/********** SPI INTERFACE test END ***************/

	/********** I2C INTERFACE test ******************/

	rdSensorReg16_8(0x300a, &Camera_pin_check);
	if (Camera_pin_check == 0x56)
	{
		stats2 = 0x41;     // 연결 되어있을시 ascii =ACamera_Capture(tpcb);

	}
	else
	{
		stats2 = 0x46;     // 연결 되어있을시 ascii =F

	}

	stats3[0] = stats;
	stats3[1] = stats2;

	tcp_write(tpcb, &stats3, 2, 1);

	//최종 출력 =AA  이면 I2C, SPI  OK
	//최종 출력 =FF   이면 I2C, SPI  FAIL

	/********** I2C INTERFACE test END ******************/

}

