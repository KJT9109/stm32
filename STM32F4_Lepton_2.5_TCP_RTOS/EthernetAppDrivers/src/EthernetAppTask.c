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

  uint16_t len;
  void *data;


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

	netPtr->recv_timeout = 1000;

	while(1)
	{
	  while (netconn_recv(netPtr, &netBuf) == ERR_OK)
	  {

		netbuf_data(netBuf, &data, &len);

		if(strncmp(data,"LEPTON", len) == 0)
	 	{
	      netconn_write(netPtr, (char *)"Lepton Recive !!! \r\n", sizeof("Lepton Recive !!! \r\n"), NETCONN_COPY);
		}
		else
		{
	      netconn_write(netPtr, (char *)" Not User Command \r\n", sizeof("Not User Command \r\n"), NETCONN_COPY);
		}

		netbuf_delete(netBuf);
	  }

	  netconn_write(netPtr,(char *)"Test Non Blocking Recv\r\n", sizeof("Test Non Blocking Recv\r\n"),NETCONN_COPY);

    }
  }

   netconn_close(netPtr);
   netconn_delete(netPtr);

}

