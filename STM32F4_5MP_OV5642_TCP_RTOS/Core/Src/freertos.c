/* USER CODE BEGIN Header */
/**
******************************************************************************
* File Name          : freertos.c
* Description        : Code for freertos applications
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0044
*
******************************************************************************
*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"

#include <string.h>

#include "Camera_motion.h"

#include "FreeRTOS2.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

struct TCP_str TCP_arg;
struct TCP_str CMRA_arg;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TcpTask */
osThreadId_t TcpTaskHandle;
const osThreadAttr_t TcpTask_attributes = {
  .name = "TcpTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for CameraTask */
osThreadId_t CameraTaskHandle;
const osThreadAttr_t CameraTask_attributes = {
  .name = "CameraTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TCPQUEUE */
osMessageQueueId_t TCPQUEUEHandle;
const osMessageQueueAttr_t TCPQUEUE_attributes = {
  .name = "TCPQUEUE"
};
/* Definitions for TCPLinkSemaphore */
osSemaphoreId_t TCPLinkSemaphoreHandle;
const osSemaphoreAttr_t TCPLinkSemaphore_attributes = {
  .name = "TCPLinkSemaphore"
};
/* Definitions for CmraSemaphore */
osSemaphoreId_t CmraSemaphoreHandle;
const osSemaphoreAttr_t CmraSemaphore_attributes = {
  .name = "CmraSemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
struct netconn *tcpconn;

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Tcp_Task(void *argument);
void Camera_Task(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of TCPLinkSemaphore */
  TCPLinkSemaphoreHandle = osSemaphoreNew(1, 1, &TCPLinkSemaphore_attributes);

  /* creation of CmraSemaphore */
  CmraSemaphoreHandle = osSemaphoreNew(1, 1, &CmraSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of TCPQUEUE */
  TCPQUEUEHandle = osMessageQueueNew (16, sizeof(uint16_t), &TCPQUEUE_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TcpTask */
 // TcpTaskHandle = osThreadNew(Tcp_Task, NULL, &TcpTask_attributes);

  /* creation of CameraTask */
 // CameraTaskHandle = osThreadNew(Camera_Task, NULL, &CameraTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  
  TCP_arg.netconn = tcpconn;
  TCP_arg.semaphore = TCPLinkSemaphoreHandle; 
  
  CMRA_arg.netconn = tcpconn;
  CMRA_arg.semaphore = CmraSemaphoreHandle;
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief  Function implementing the defaultTask thread.
* @param  argument: Not used 
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  set_format(JPEG);	
  initCAM();
  set_bit(0x03,0x02);
  clear_fifo_flag();
  write_reg(0x01,0x00);
  OV5642_set_JPEG_size(OV5642_1600x1200);
  
  TcpTaskHandle = osThreadNew(Tcp_Task, NULL, &TcpTask_attributes);
  CameraTaskHandle = osThreadNew(Camera_Task, NULL, &CameraTask_attributes);
  /* Infinite loop */
  for(;;)
  {
   osThreadTerminate(defaultTaskHandle);
      
  
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Tcp_Task */
/**
* @brief Function implementing the TcpTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tcp_Task */

void Tcp_Task(void *argument)
{

  /* USER CODE BEGIN Tcp_Task */
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  void *data;
  u16_t len;
  
  LWIP_UNUSED_ARG(argument);
  conn = netconn_new(NETCONN_TCP);
  if (conn!=NULL)
  {  
    err = netconn_bind(conn, NULL, 1460);
    
    if (err == ERR_OK)
    {
      netconn_listen(conn);
      
      while (1) 
      {
        accept_err = netconn_accept(conn, &newconn);
        if (accept_err == ERR_OK) 
        {
              CMRA_arg.netconn  =  newconn;
              while(netconn_recv(newconn, &buf) == ERR_OK) 
              {
                
                netbuf_data(buf, &data, &len);   
                
                if(strcmp(data,"CMRA") == 0)
                {
                  Camera_Capture(newconn);
                }
 
                else if(strcmp(data,"CMIT") == 0)
                {
                  Camera_interface_test(newconn);
                }
                else
                {
                  netconn_write(newconn, "ERROR", 5, NETCONN_COPY);
                  netconn_write(newconn, "ERROR", 5, NETCONN_NOFLAG);
                  osDelay(100);
                }
                netbuf_delete(buf);
         
                         
              }
              
#ifdef DEBUG          
            printf("LWIP_TASK\n\r");
#endif           
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
      
    }
    else
    {
      netconn_delete(newconn);
    }
    
    
  }
  
  
  
  /* Infinite loop */
  
  /* USER CODE END Tcp_Task */
}

/* USER CODE BEGIN Header_Camera_Task */
/**
* @brief Function implementing the CameraTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Camera_Task */
void Camera_Task(void *argument)
{
  /* USER CODE BEGIN Camera_Task */
  int tast = 0;
  /* Infinite loop */
 
  /* Infinite loop */
  for(;;)
  {
#ifdef DEBUG
    tast++;
    printf(" hello Camera Task: %d \n\r",tast);
#endif     
    osDelay(100);
   
    
  }
  /* USER CODE END Camera_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
