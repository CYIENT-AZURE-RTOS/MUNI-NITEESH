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
#define RATE1 500
#define RATE2 600
#define RATE3 700
#define RATE4 800

#define PER1 2
#define PER2 2
#define PER3 1
#define PER4 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_THREAD Threadthree;
TX_THREAD Threadfour;

//TX_MUTEX uart_pointer;
TX_SEMAPHORE sem_pointer;
extern UART_HandleTypeDef huart1;
uint8_t tx_success=0;

uint8_t status;
uint8_t REDflag;
uint8_t GREENflag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void Threadthree_Entry(ULONG thread_input);
void Threadfour_Entry(ULONG thread_input);
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
  CHAR *pointer,*pointer2,*pointer3,*pointer4;

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
    /* Allocate the stack for ThreadThree.  */
       if (tx_byte_allocate(byte_pool, (VOID **) &pointer3,
                            APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
       {
         ret = TX_POOL_ERROR;
       }

       /* Create ThreadThree.  */
       if (tx_thread_create(&Threadthree, "Thread three", Threadthree_Entry, 0,
                            pointer3, APP_STACK_SIZE,
                            THREAD_THREE_PRIO, THREAD_THREE_PREEMPTION_THRESHOLD,
                            1, TX_AUTO_START) != TX_SUCCESS)
       {
         ret = TX_THREAD_ERROR;
       }
       /* Allocate the stack for ThreadFour.  */
          if (tx_byte_allocate(byte_pool, (VOID **) &pointer4,
                               APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
          {
            ret = TX_POOL_ERROR;
          }

          /* Create ThreadFour.  */
          if (tx_thread_create(&Threadfour, "Thread four", Threadfour_Entry, 0,
                               pointer4, APP_STACK_SIZE,
                               THREAD_FOUR_PRIO, THREAD_FOUR_PREEMPTION_THRESHOLD,
                               1, TX_AUTO_START) != TX_SUCCESS)
          {
            ret = TX_THREAD_ERROR;
          }
   // tx_mutex_create(&uart_pointer,"uart pointer",TX_NO_INHERIT);
    tx_semaphore_create(&sem_pointer,"sem pointer",2);
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

  uint8_t pin;

  while(1)									/* Infinite loop */
  {
	 // tx_mutex_get(&uart_pointer,TX_WAIT_FOREVER);
	  status=tx_semaphore_get(&sem_pointer,TX_WAIT_FOREVER);
	  if(tx_success==status)
	  {
	  HAL_UART_Transmit(&huart1,"\n\r*******In thread one function*****\n\r",40, 100);
	  if(0==REDflag)
	  {
		  REDflag=1;
		  pin=GPIO_PIN_6;
	  }
	  else if(0==GREENflag)
	  {
		  GREENflag=1;
		 pin=GPIO_PIN_7;
	  }
	  for(int i=0;i<PER1;i++)
	  {
		  HAL_GPIO_TogglePin(GPIOH, pin);	/* Toggle LED */
		  App_Delay(RATE1);							/* Delay for 100=1000ms */
	  }
	  if(0==GPIO_PIN_6)
	  {
		  REDflag=0;
	  }
	  else if(0==GPIO_PIN_7)
	  {
		  GREENflag=0;
	  }

	  //tx_mutex_put(&uart_pointer);

	  HAL_UART_Transmit(&huart1,"\n\r*******released thread one function*****\n\r",40, 100);
	  tx_semaphore_put(&sem_pointer);
    }
  }
}
void ThreadTwo_Entry(ULONG thread_input)
{

  uint8_t pin;
  while(1)									/* Infinite loop */
  {
	  status=tx_semaphore_get(&sem_pointer,TX_WAIT_FOREVER);
		  if(tx_success==status)
		  {
		  HAL_UART_Transmit(&huart1,"\n\r*******In thread two function*****\n\r" ,40, 100);
		  if(0==REDflag)
		  {
			  REDflag=1;
			  pin=GPIO_PIN_6;
		  }
		  else if(0==GREENflag)
		  {
			  GREENflag=1;
			 pin=GPIO_PIN_7;
		  }
		  for(int i=0;i<PER2;i++)
		  {
			  HAL_GPIO_TogglePin(GPIOH, pin);	/* Toggle LED */
			  App_Delay(RATE2);							/* Delay for 100=1000ms */
		  }
		  if(0==GPIO_PIN_6)
		  {
			  REDflag=0;
		  }
		  else if(0==GPIO_PIN_7)
		  {
			  GREENflag=0;
		  }
		  //tx_mutex_put(&uart_pointer);

		  HAL_UART_Transmit(&huart1,"\n\r*******released thread two function*****\n\r",40, 100);
		  tx_semaphore_put(&sem_pointer);
	    }
  }
}
void Threadthree_Entry(ULONG thread_input)
{

  uint8_t pin;
  while(1)									/* Infinite loop */
  {
	  status=tx_semaphore_get(&sem_pointer,TX_WAIT_FOREVER);
		  if(tx_success==status)
		  {
		  HAL_UART_Transmit(&huart1,"\n\r*******In thread three function*****\n\r",40, 100);
		  if(0==REDflag)
		  {
			  REDflag=1;
			  pin=GPIO_PIN_6;
		  }
		  else if(0==GREENflag)
		  {
			  GREENflag=1;
			 pin=GPIO_PIN_7;
		  }
		  for(int i=0;i<PER3;i++)
		  {
			  HAL_GPIO_TogglePin(GPIOH, pin);	/* Toggle LED */
			  App_Delay(RATE3);							/* Delay for 100=1000ms */
		  }
		  if(0==GPIO_PIN_6)
		  {
			  REDflag=0;
		  }
		  else if(0==GPIO_PIN_7)
		  {
			  GREENflag=0;
		  }
		  //tx_mutex_put(&uart_pointer);

		  HAL_UART_Transmit(&huart1,"\n\r*******released thread three function*****\n\r",42, 100);
		  tx_semaphore_put(&sem_pointer);
	    }
  }
}
void Threadfour_Entry(ULONG thread_input)
{
  uint8_t pin;
  while(1)									/* Infinite loop */
  {
	  status=tx_semaphore_get(&sem_pointer,TX_WAIT_FOREVER);
		  if(tx_success==status)
		  {
		  HAL_UART_Transmit(&huart1,"\n\r*******In thread four function*****\n\r" ,40, 100);
		  if(0==REDflag)
		  {
			  REDflag=1;
			  pin=GPIO_PIN_6;
		  }
		  else if(0==GREENflag)
		  {
			  GREENflag=1;
			 pin=GPIO_PIN_7;
		  }
		  for(int i=0;i<PER4;i++)
		  {
			  HAL_GPIO_TogglePin(GPIOH, pin);	/* Toggle LED */
			  App_Delay(RATE4);							/* Delay for 100=1000ms */
		  }
		  if(0==GPIO_PIN_6)
		  {
			  REDflag=0;
		  }
		  else if(0==GPIO_PIN_7)
		  {
			  GREENflag=0;
		  }
		  //tx_mutex_put(&uart_pointer);

		  HAL_UART_Transmit(&huart1,"\n\r*******released thread four function*****\n\r",41, 100);
		  tx_semaphore_put(&sem_pointer);
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
