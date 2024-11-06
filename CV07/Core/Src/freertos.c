/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "gpio.h"
#include "lis2dw12_reg.h"
#include "accel.h"
#include <stdio.h>
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
osThreadId defaultTaskHandle;
osThreadId VisualTaskHandle;
osThreadId AcceleroTaskHandle;
osMessageQId xVisualQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartVisualTask(void const * argument);
void StartAcceleroTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

  /* Create the queue(s) */
  /* definition and creation of xVisualQueue */
  osMessageQDef(xVisualQueue, 16, int16_t);
  xVisualQueueHandle = osMessageCreate(osMessageQ(xVisualQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of VisualTask */
  osThreadDef(VisualTask, StartVisualTask, osPriorityNormal, 0, 128);
  VisualTaskHandle = osThreadCreate(osThread(VisualTask), NULL);

  /* definition and creation of AcceleroTask */
  osThreadDef(AcceleroTask, StartAcceleroTask, osPriorityIdle, 0, 128);
  AcceleroTaskHandle = osThreadCreate(osThread(AcceleroTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartVisualTask */
/**
* @brief Function implementing the VisualTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartVisualTask */
void StartVisualTask(void const * argument)
{
  /* USER CODE BEGIN StartVisualTask */
  /* Infinite loop */
  for(;;)
  {


		int16_t msg;
		if (xQueueReceive(xVisualQueueHandle, &msg, portMAX_DELAY))
		{
			if(msg < -1000)
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			}
			else if (msg > 1000)
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			}
			else
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			}

		}

		osDelay(1);
  }
  /* USER CODE END StartVisualTask */
}

/* USER CODE BEGIN Header_StartAcceleroTask */
/**
* @brief Function implementing the AcceleroTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAcceleroTask */
void StartAcceleroTask(void const * argument)
{
  /* USER CODE BEGIN StartAcceleroTask */

	static stmdev_ctx_t lis2dw12 =
	{ .write_reg = platform_write, .read_reg = platform_read, .handle = &hi2c1, };

	// Check device ID
	uint8_t whoamI = 0;
	lis2dw12_device_id_get(&lis2dw12, &whoamI);
	printf("LIS2DW12_ID %s\n", (whoamI == LIS2DW12_ID) ? "OK" : "FAIL");

  /* Infinite loop */
  for(;;)
  {
		int16_t data = 0;
		osMessagePut(xVisualQueueHandle, data, 0);
		osDelay(100);

		data = -5000;
		osMessagePut(xVisualQueueHandle, data, 0);
		osDelay(100);

		data = 5000;
		osMessagePut(xVisualQueueHandle, data, 0);
		osDelay(100);
  }
  /* USER CODE END StartAcceleroTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

