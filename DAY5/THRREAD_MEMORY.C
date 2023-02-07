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

TX_MUTEX uart_pointer;
extern UART_HandleTypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void Mainthread_Entry(ULONG thread_input);
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
  CHAR *pointer,*pointer2;

    /* Allocate the stack for ThreadOne.  */
  ret=tx_byte_allocate(byte_pool, (VOID **) &pointer,
		                         APP_STACK_SIZE1, TX_NO_WAIT);
//    if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
//                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
//    {
//      ret = TX_POOL_ERROR;
//    }
//  ret=tx_block_pool_create(&block_pool,"my_block_pool",50, (VOID **) &pointer,
//		                         APP_STACK_SIZE1, TX_NO_WAIT);
    /* Create ThreadOne.  */
    if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0,
                         pointer, APP_STACK_SIZE1,
                         THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                         1, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }

    /* Allocate the stack for ThreadTwo.  */
    ret=tx_byte_allocate(byte_pool, (VOID **) &pointer2,
    		                         APP_STACK_SIZE2, TX_NO_WAIT);
//    if (tx_byte_allocate(byte_pool, (VOID **) &pointer2,
//                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
//    {
//      ret = TX_POOL_ERROR;
//    }
    /* Create ThreadTwo.  */
    if (tx_thread_create(&ThreadTwo, "Thread Two", ThreadTwo_Entry, 0,
                         pointer2, APP_STACK_SIZE2,
                         THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
                         1, TX_AUTO_START) != TX_SUCCESS)
    {
      ret = TX_THREAD_ERROR;
    }

    //tx_mutex_create(&uart_pointer,"uart pointer",TX_NO_INHERIT);
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
  char c[]="IAM IN CYIENT OFFICE\n\r";

  while(1)									/* Infinite loop */
  {
	  //tx_mutex_get(&uart_pointer,TX_WAIT_FOREVER);
	  HAL_UART_Transmit(&huart1,c ,strlen(c), 100);
	  //tx_mutex_put(&uart_pointer);
	HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_7);	/* Toggle Green LED */
    App_Delay(5);							/* Delay for 100=1000ms */
  }
}
void ThreadTwo_Entry(ULONG thread_input)
{
   char s[]="WILL WORK UNTIL I GET OUTPUT\n\r";
  while(1)									/* Infinite loop */
  {
	 // tx_mutex_get(&uart_pointer,TX_WAIT_FOREVER);
	  HAL_UART_Transmit(&huart1, s, strlen(s), 100);
	 // tx_mutex_put(&uart_pointer);
//	  App_Delay(5);
	HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_6);	/* Toggle Red LED */
	App_Delay(10);						/* Delay for 100=1000ms */
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
