/*
 * tcp_server.c
 *
 *  Created on: Dec 29, 2019
 *      Author: jitae.kim
 */

#include "tcp_server.h"
#include "Camera_motion.h"

#include "debug_mode.h"

void tcp_server_init(void)
{

	tcp_server_pcb = tcp_new();

	if (tcp_server_pcb != NULL)
	{
		err_t err;
		err = tcp_bind(tcp_server_pcb, IP_ADDR_ANY, 1460);

		if (err == ERR_OK)
		{
			/* TCP Listening */
			tcp_server_pcb = tcp_listen(tcp_server_pcb);
			/* Define Callback Function if a client is Connected */
			 tcp_accept(tcp_server_pcb, tcp_server_accept);


 		}
		else
		{
			/* deallocate the pcb */
			memp_free(MEMP_TCP_PCB, tcp_server_pcb);
		}
	}
}

err_t tcp_server_accept(void *arg, struct tcp_pcb *accept_pcb, err_t err)
{

	err_t ret_err;

	/*tcp_echoserver_struct형 구조체 변수 es를 선언함 */
	struct tcp_server_struct *es;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	/* set priority for the newly accepted tcp connection accept_pcb */
	tcp_setprio(accept_pcb, TCP_PRIO_MIN);

	/* allocate structure es to maintain tcp connection informations */
	es = (struct tcp_server_struct *) mem_malloc(
			sizeof(struct tcp_server_struct));

	if (es != NULL)
	{
		es->state = ES_RECEIVED;
		es->pcb = accept_pcb;
		es->p = NULL;

		/* pass newly allocated es structure as argument to accept_pcb */
		tcp_nagle_disable(accept_pcb);
		tcp_arg(accept_pcb, es);

		tcp_sent(accept_pcb, tcp_server_sent);

		tcp_recv(accept_pcb, tcp_server_recv);

		/* initialize lwip tcp_err callback function for accept_pcb  */
		tcp_err(accept_pcb, tcp_server_error);

		tcp_poll(accept_pcb, tcp_server_poll, 1);

		/* send data */
		tcp_server_send(accept_pcb, es);

		ret_err = ERR_OK;
	}
}

err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
		err_t err)
{

	struct tcp_server_struct *es;

	err_t ret_err;
	char cp[50];
	char *recv_buf = '\0';
	uint8_t len;

	LWIP_ASSERT("arg != NULL", arg != NULL);

	es = (struct tcp_server_struct *) arg;

	/* if we receive an empty tcp frame from client => close connection */
	if (p == NULL)
	{
		/* remote host closed connection */
		es->state = ES_CLOSING;
		if (es->p == NULL)
		{
			/* we're done sending, close connection */
			tcp_server_connection_close(tpcb, es);
		}
		else
		{
			/* we're not done yet */
			/* acknowledge received packet */
			tcp_sent(tpcb, tcp_server_sent);

			/* send remaining data*/
			tcp_server_send(tpcb, es);
		}
		ret_err = ERR_OK;
	}

	/* else : a non empty frame was received from client but for some reason err != ERR_OK */
	else if (err != ERR_OK)
	{
		/* free received pbuf*/
		if (p != NULL)
		{
			es->p = NULL;
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if (es->state == ES_ACCEPTED)
	{
		es->state = ES_RECEIVED;
		/* store reference to incoming pbuf (chain) */
		es->p = p;

		/* initialize LwIP tcp_sent callback function */
		tcp_sent(tpcb, tcp_server_sent);

	}

	else if (es->state == ES_RECEIVED)
	{
		/* more data received from client and previous data has been already sent*/
		if (es->p == NULL)
		{
			recv_buf = p->payload;
			len = p->len;
			strncpy(cp, recv_buf, p->len);
			cp[len] = '\0';

			if (strcmp(cp, "cmt1") == 0)
			{
				Camera_interface_test(tpcb);
			}
			else if (strcmp(cp, "capture1") == 0)
			{

				Camera_Capture(tpcb);
			}

			else
			{
				tcp_write(tpcb, "check Command", sizeof("check Command"), 1);
			}

			pbuf_free(p);
		}

		else
		{

			struct pbuf *ptr;
			/* chain pbufs to the end of what we recv'ed previously  */
			ptr = es->p;
			pbuf_chain(ptr, p);
		}
		ret_err = ERR_OK;
	}

	/* data received when connection already closed */
	else
	{
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);

		/* free pbuf and do nothing */
		es->p = NULL;
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	return ret_err;
}

void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{

	struct pbuf *ptr;
	err_t wr_err = ERR_OK;

	while ((wr_err == ERR_OK) && (es->p != NULL)
			&& (es->p->len <= tcp_sndbuf(tpcb)))
	{

		/* get pointer on pbuf from es structure */
		ptr = es->p;

		/* enqueue data for transmission */

		wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

		if (wr_err == ERR_OK)
		{
			u16_t plen;

			plen = ptr->len;

			/* continue with next pbuf in chain (if any) */

			es->p = ptr->next;

			if (es->p != NULL)
			{
				/* increment reference count for es->p */
				pbuf_ref(es->p);
			}

			/* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
			pbuf_free(ptr);

			/* Update tcp window size to be advertized : should be called when received
			 data (with the amount plen) has been processed by the application layer */
			tcp_recved(tpcb, plen);

		}
		else if (wr_err == ERR_MEM)
		{
			/* we are low on memory, try later / harder, defer to poll */
			es->p = ptr;
		}
		else
		{
			/* other problem ?? */
		}
	}
}
err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	struct tcp_server_struct *es;

	LWIP_UNUSED_ARG(len);

	es = (struct tcp_server_struct *) arg;

	if (es->p != NULL)
	{
		/* still got pbufs to send */
		tcp_server_send(tpcb, es);
	}
	else
	{


		/* if no more data to send and client closed connection*/
		if (es->state == ES_CLOSING)
			tcp_server_connection_close(tpcb, es);
	}
	return ERR_OK;
}

err_t tcp_camera_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	struct tcp_server_struct *es;

	LWIP_UNUSED_ARG(len);

	es = (struct tcp_server_struct *) arg;

	if (es->p != NULL)
	{
		/* still got pbufs to send */
		tcp_server_send(tpcb, es);
	}
	else
	{
		second_picture_data(tpcb);

		/* if no more data to send and client closed connection*/
		if (es->state == ES_CLOSING)
			tcp_server_connection_close(tpcb, es);
	}
	return ERR_OK;
}

err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_server_struct *es;

	es = (struct tcp_server_struct *) arg;

	if (es != NULL)
	{

		if (es->p != NULL)
		{
			/* there is a remaining pbuf (chain) , try to send data */
			tcp_server_send(tpcb, es);
		}
		else
		{

			/* no remaining pbuf (chain)  */
			if (es->state == ES_CLOSING)
			{
				/*  close tcp connection */
				tcp_server_connection_close(tpcb, es);
			}
		}
		ret_err = ERR_OK;
	}

	else
	{
		/* nothing to be done */
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
	return ret_err;

}

/**
 *   This functions closes the tcp connection
 * param  	tcp_pcb	: pointer on the tcp connection
 * param  	es		: pointer on echo_state structure
 * retval 	None
 */
void tcp_server_connection_close(struct tcp_pcb *tpcb,
		struct tcp_server_struct *es)
{

	/* remove all callbacks */
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_recv(tpcb, NULL);
	tcp_err(tpcb, NULL);
	tcp_poll(tpcb, NULL, 0);

	/* delete es structure */
	if (es != NULL)
	{
		mem_free(es);
	}

	/* close tcp connection */
	tcp_close(tpcb);
}

/**
 *   This function implements the tcp_err callback function (called
 *         when a fatal tcp_connection error occurs.
 * param  	arg	: pointer on argument parameter
 * param  	err	: not used
 * retval 	None
 */

void tcp_server_error(void *arg, err_t err)
{
	struct tcp_server_struct *es;

	LWIP_UNUSED_ARG(err);

	es = (struct tcp_server_struct *) arg;
	if (es != NULL)
	{
		/*  free es structure */
		mem_free(es);
	}
}

