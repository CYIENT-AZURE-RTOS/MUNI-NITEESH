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
TX_TIMER tim1_pointer;
TX_TIMER tim2_pointer;
TX_TIMER tim3_pointer;
extern UART_HandleTypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void Timer1_function_Entry(ULONG thread_input);
void Timer2_function_Entry(ULONG thread_input);
void Timer3_function_Entry(ULONG thread_input);
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
                         1, TX_DONT_START) != TX_SUCCESS)
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

    tx_mutex_create(&uart_pointer,"uart pointer",TX_NO_INHERIT);
    tx_timer_create(&tim1_pointer,"timer1 pointer",Timer1_function_Entry,0x66,50,25,TX_AUTO_ACTIVATE);
    tx_timer_create(&tim2_pointer,"timer2 pointer",Timer2_function_Entry,0x66,75,50,TX_AUTO_ACTIVATE);
    tx_timer_create(&tim3_pointer,"timer3 pointer",Timer3_function_Entry,0x66,100,75,TX_AUTO_ACTIVATE);
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
  uint8_t t1_count=0;
  char c[]="\n\r THREAD ONE IS CREATED BY TIMERS\n\r";

  while(1)									/* Infinite loop */
  {
	  tx_mutex_get(&uart_pointer,TX_WAIT_FOREVER);
	  HAL_UART_Transmit(&huart1,c ,strlen(c), 100);
	  tx_mutex_put(&uart_pointer);
      App_Delay(5);							/* Delay for 100=1000ms */
  }
}
void ThreadTwo_Entry(ULONG thread_input)
{
  uint8_t t2_count=0;
  char s[]="\n\rTHREAD TWO IS CREATED BY TIMERS\n\r";

  while(1)									/* Infinite loop */
  {
	  tx_mutex_get(&uart_pointer,TX_WAIT_FOREVER);
	  HAL_UART_Transmit(&huart1, s, strlen(s), 100);
	  tx_mutex_put(&uart_pointer);
 	  App_Delay(5);                        /* Delay for 100=1000ms */
  }
}
void Timer1_function_Entry(ULONG thread_input)
{
	  HAL_UART_Transmit(&huart1, "\n\rWELCOME TO TIMERS 1\n\r",27, 100);
	  //tx_mutex_delete(&uart_pointer);
	 tx_thread_resume(&ThreadOne);
}
void Timer2_function_Entry(ULONG thread_input)
{
	  HAL_UART_Transmit(&huart1, "\n\rWELCOME TO TIMERS 2\n\r",27, 100);
}
void Timer3_function_Entry(ULONG thread_input)
{
	  HAL_UART_Transmit(&huart1, "\n\rWELCOME TO TIMERS 3\n\r",27, 100);
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
