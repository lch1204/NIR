/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
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
 bool button_flag = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void Power_State_Machine(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//struct states{
//static const uint8_t STATE_1; // default state, all peripheral is off except thrusters controller
//static const uint8_t STATE_2; // 5V and 22V ENABLE
//static const uint8_t STATE_3; // 12V ENABLE
//static const uint8_t STATE_4; // 22V ENABLE
//static const uint8_t STATE_5; // RESET (go to STATE_1 for 5s then return to previous state
//};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	 // HAL_GPIO_WritePin(GPIOA,EN_VMA_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA,EN_5V_Pin, GPIO_PIN_SET);


    /* USER CODE BEGIN 3 */
	  HAL_UART_Receive(&huart2, (uint8_t*)bufferRx, 6, 10); //принимаем сообщение
	        if((bufferRx[0] == 0xff) && (bufferRx[1] == 0xfD))
	       	  rxFlag = 1;
	        while(bufferRx[0]!= 0xff || bufferRx[1]!= 0xfD){ //проверяем соответсвует ли посылка
	        			HAL_UART_Receive(&huart2, (uint8_t*)bufferRx, 6, 10);//принимаем сообщение еще раз
	        		}

	  	//  if (huart2.RxXferCount == 0){
	  	  if(rxFlag){

	  		  rxFlag = 0;
	  		  if(check_crc(bufferRx, MAX_SIZE)){ //если crc совпало

	  			  data[0] =  (bufferRx[3] << 8) | (bufferRx[2] & 0xff);
	  			  switch(data[0]){
	  			  case 0:
	  				  break;
	  			  case 1:
	  				  HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_SET);
	  				  break;
	  			  case 2:
	  				  HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_SET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_12V_Pin, GPIO_PIN_SET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_12V_ISO_Pin, GPIO_PIN_SET);
	  				  break;
	  			  case 3:
	  				  HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_SET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_12V_Pin, GPIO_PIN_SET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_12V_ISO_Pin, GPIO_PIN_SET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_VMA_Pin, GPIO_PIN_SET);
	  				  break;
	  			  case 4:
	  				  HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_RESET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_12V_Pin, GPIO_PIN_RESET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_12V_ISO_Pin, GPIO_PIN_RESET);
	  				  HAL_GPIO_WritePin(GPIOA, EN_VMA_Pin, GPIO_PIN_RESET);
	  				  break;
	  			  }

	  		  }
	  		  else{
	  			//  HAL_GPIO_WritePin(LD8_GPIO_Port, LD8_Pin, GPIO_PIN_RESET); //индикация о ошибке приема
	  		  }

	  	  }

	    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EN_5V_Pin|EN_VMA_Pin|EN_12V_Pin|EN_12V_ISO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUTTON_Pin RP0_Pin RP1_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin|RP0_Pin|RP1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : EN_5V_Pin EN_VMA_Pin EN_12V_Pin EN_12V_ISO_Pin */
  GPIO_InitStruct.Pin = EN_5V_Pin|EN_VMA_Pin|EN_12V_Pin|EN_12V_ISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

}

/* USER CODE BEGIN 4 */
void Power_State_Machine(void){
	 if(button_flag){
		 HAL_GPIO_WritePin(GPIOA,EN_VMA_Pin, GPIO_PIN_SET);
		 button_flag = false;
	 }
	 else
		 HAL_GPIO_WritePin(GPIOA,EN_VMA_Pin, GPIO_PIN_RESET);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   if(GPIO_Pin == BUTTON_Pin)
   {
      button_flag = true;
   }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
