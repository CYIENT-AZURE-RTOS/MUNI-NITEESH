/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "app_threadx.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include <string.h>
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
/* USER CODE BEGIN PV */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_THREAD Threadthree;
TX_EVENT_FLAGS_GROUP event_pointer;
extern UART_HandleTypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void Threadthree_Entry(ULONG thread_input);
void App_Delay(uint32_t Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

   /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer,*pointer2,*pointer3;

    /* Allocate the stack for ThreadOne.  */
    if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
      ret = TX_POOL_ERROR;
    }

    /* Create ThreadOne.  */
    if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0,
                         pointer, APP_STACK_SIZE,
                         THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                         1, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }

    /* Allocate the stack for ThreadTwo.  */
    if (tx_byte_allocate(byte_pool, (VOID **) &pointer2,
                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
      ret = TX_POOL_ERROR;
    }
    /* Create ThreadTwo.  */
    if (tx_thread_create(&ThreadTwo, "Thread Two", ThreadTwo_Entry, 0,
                         pointer2, APP_STACK_SIZE,
                         THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
                         1, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }
    /* Allocate the stack for Threadthree.  */
    if (tx_byte_allocate(byte_pool, (VOID **) &pointer3,
                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
      ret = TX_POOL_ERROR;
    }

    /* Create Threadthree.  */
    if (tx_thread_create(&Threadthree, "Thread three", Threadthree_Entry, 0,
                         pointer3, APP_STACK_SIZE,
                         THREAD_THREE_PRIO, THREAD_THREE_PREEMPTION_THRESHOLD,
                         1, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }
  ret=tx_event_flags_create(&event_pointer,"event pointer");
  if(TX_SUCCESS!=ret)
  {
	  HAL_UART_Transmit(&huart1, "\n\rcould not create the event\n\r", 34, 100);
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing the ThreadOne thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadOne_Entry(ULONG thread_input)
{
  uint8_t status;
  ULONG actual_events;
  while(1)									/* Infinite loop */
  {
   status=tx_event_flags_get(&event_pointer,0x111,TX_AND_CLEAR,&actual_events,20);
	if(TX_SUCCESS==status)
	{
		HAL_UART_Transmit(&huart1, "\n\r event done\n\r", 18, 100);
	}
//	t1_count++;
//	   if(t1_count==10)
//	   {
//		   t1_count=0;
//		   tx_thread_sleep(100);
//	   }
  }
}
void ThreadTwo_Entry(ULONG thread_input)
{
	uint8_t t1_count=0;
  uint8_t status;
   while(1)									/* Infinite loop */
   {
	   t1_count++;
	   tx_thread_sleep(100);
	   	   if(t1_count==2)
	   	   {
	   		   t1_count=0;
	   		    status=tx_event_flags_set(&event_pointer,0x11,TX_OR);
	   		 	if(TX_SUCCESS==status)
	   		 	{
	   		 		HAL_UART_Transmit(&huart1, "\n\r set event 0 4\n\r", 22, 100);
	   		 	}
	   	   }

   }
}
void Threadthree_Entry(ULONG thread_input)
{
  uint8_t t2_count=0;
  uint8_t status;
   while(1)									/* Infinite loop */
   {
	   t2_count++;
	  	   tx_thread_sleep(100);
	  	   	   if(t2_count==2)
	  	   	   {
	  	   		   t2_count=0;
	  	   		    status=tx_event_flags_set(&event_pointer,0x100,TX_OR);
	  	   		 	if(TX_SUCCESS==status)
	  	   		 	{
	  	   		 		HAL_UART_Transmit(&huart1, "\n\r set event 8\n\r", 20, 100);
	  	   		 	}
	  	   	   }
   }
}

/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
/* USER CODE END 1 */
