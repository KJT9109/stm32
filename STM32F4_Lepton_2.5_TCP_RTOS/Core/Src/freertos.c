/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
/* Definitions for LeptonTask */
osThreadId_t LeptonTaskHandle;
const osThreadAttr_t LeptonTask_attributes = {
  .name = "LeptonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ConsoleTask */
osThreadId_t ConsoleTaskHandle;
const osThreadAttr_t ConsoleTask_attributes = {
  .name = "ConsoleTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for EthernetAppTask */
osThreadId_t EthernetAppTaskHandle;
const osThreadAttr_t EthernetAppTask_attributes = {
  .name = "EthernetAppTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TempDetectionTa */
osThreadId_t TempDetectionTaHandle;
const osThreadAttr_t TempDetectionTa_attributes = {
  .name = "TempDetectionTa",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void vLeptonTask(void *argument);
void vConsoleTask(void *argument);
void vEthernetAppTask(void *argument);
void vTempDetectionTask(void *argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LeptonTask */
  LeptonTaskHandle = osThreadNew(vLeptonTask, NULL, &LeptonTask_attributes);

  /* creation of ConsoleTask */
  ConsoleTaskHandle = osThreadNew(vConsoleTask, NULL, &ConsoleTask_attributes);

  /* creation of EthernetAppTask */
  EthernetAppTaskHandle = osThreadNew(vEthernetAppTask, NULL, &EthernetAppTask_attributes);

  /* creation of TempDetectionTa */
  TempDetectionTaHandle = osThreadNew(vTempDetectionTask, NULL, &TempDetectionTa_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_vLeptonTask */
/**
  * @brief  Function implementing the LeptonTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_vLeptonTask */
__weak void vLeptonTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN vLeptonTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vLeptonTask */
}

/* USER CODE BEGIN Header_vConsoleTask */
/**
* @brief Function implementing the ConsoleTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vConsoleTask */
__weak void vConsoleTask(void *argument)
{
  /* USER CODE BEGIN vConsoleTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vConsoleTask */
}

/* USER CODE BEGIN Header_vEthernetAppTask */
/**
* @brief Function implementing the EthernetAppTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vEthernetAppTask */
__weak void vEthernetAppTask(void *argument)
{
  /* USER CODE BEGIN vEthernetAppTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vEthernetAppTask */
}

/* USER CODE BEGIN Header_vTempDetectionTask */
/**
* @brief Function implementing the TempDetectionTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTempDetectionTask */
__weak void vTempDetectionTask(void *argument)
{
  /* USER CODE BEGIN vTempDetectionTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vTempDetectionTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
