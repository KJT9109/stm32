/*
 * tcp_server.h
 *
 *  Created on: Dec 29, 2019
 *      Author: jitae.kim
 */

#ifndef TARGET_TCP_SERVER_H_
#define TARGET_TCP_SERVER_H_


#include "stm32f4xx_hal.h"
#include "lwip.h"
#include <stdio.h>
#include <string.h>

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
/*
 *  tcp_pcb define
 *  use to tcp socket function
 *
 */
struct tcp_pcb *tcp_server_pcb;

/*
 * 	tcp server state
 * 	use to tcp_server.c
 *
 */

enum tcp_server_states
{
	ES_NONE = 0, ES_ACCEPTED, ES_RECEIVED, ES_CLOSING
};

/*
 * structure for maintaing connection infos to be passed as argument
 * to LwIP callbacks
 *
 */
struct tcp_server_struct
{
	u8_t state; /* current connection state */
	u8_t retries;
	struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
	struct pbuf *p; /* pointer on the received/to be transmitted pbuf */
};


/*
 * 	function
 * 	use to main.c, tcp_server.c
 *
 */
void tcp_server_init(void);
err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
		err_t err);
void tcp_server_error(void *arg, err_t err);
err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
void tcp_server_connection_close(struct tcp_pcb *tpcb,
		struct tcp_server_struct *es);
err_t tcp_camera_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

/*
 * parameter
 * use to tcp_server.c
 */





#endif /* TARGET_TCP_SERVER_H_ */
