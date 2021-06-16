/*
 * EthernetAppTask.c
 *
 *  Created on: 2021. 6. 16.
 *      Author: Jitae
 */


#include "EthernetAppDriver.h"

extern struct netif gnetif;



void vEthernetAppTask(void *argument)
{
  LWIP_UNUSED_ARG(argument);
  MX_LWIP_Init();

  struct netconn *netPtr;
  struct netbuf *netBuf;

  void *data;

  uint16_t len;


  for(;;)
  {
	if (gnetif.ip_addr.addr == 0 || gnetif.netmask.addr == 0 || gnetif.gw.addr == 0) //system has no valid ip address
	{
	  osDelay(1000);
	  continue;
	}

	netPtr = netconn_new(NETCONN_TCP);

	if(netPtr == NULL)
	{
	    //TODO: netPtr Error 처리
	}

	IP4_ADDR(&ipaddr, IP_ADDRESS0, IP_ADDRESS1, IP_ADDRESS2, IP_ADDRESS3);

	netconn_connect(netPtr, &ipaddr, 1460);

	while (netconn_recv(netPtr, &netBuf) == ERR_OK)
	{
        do
        {
          netbuf_data(netBuf, &data, &len);
          netconn_write(netPtr, data, len, NETCONN_COPY);

        } while (netbuf_next(netBuf) >= 0);

        netbuf_delete(netBuf);
	}

    netconn_close(netPtr);
    netconn_delete(netPtr);

    osDelay(1000);

  }



}

