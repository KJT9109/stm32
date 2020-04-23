
#ifndef INC_FREERTOS2_H
#define INC_FREERTOS2_H

struct TCP_str
{
  struct netconn *netconn;
  osSemaphoreId semaphore;
};


#endif