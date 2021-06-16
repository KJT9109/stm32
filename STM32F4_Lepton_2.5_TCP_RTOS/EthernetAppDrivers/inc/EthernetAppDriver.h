/*
 * EthernetAppDriver.h
 *
 *  Created on: 2021. 6. 16.
 *      Author: Jitae
 */

#ifndef INC_ETHERNETAPPDRIVER_H_
#define INC_ETHERNETAPPDRIVER_H_

#include "lwip.h"
#include "lwip/opt.h"

#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"

#include "ip6_addr.h"
#include "ip.h"

#define IP_ADDRESS0   192
#define IP_ADDRESS1   168
#define IP_ADDRESS2   1
#define IP_ADDRESS3   112



uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];
ip4_addr_t ipaddr;



#endif /* INC_ETHERNETAPPDRIVER_H_ */
