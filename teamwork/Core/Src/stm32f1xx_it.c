/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* 引入板级对象，供中断里访问 LED、测距结果和运行模式。 */
/* Bring in board-level objects used by interrupts and callbacks. */
#include "../../Core/Inc/bsp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* 1 ms 软计时器，作为本项目的统一时间基准。 / 1 ms software timer used as the common project time base. */
  if (time_counyer >= 10000000) {
    time_counyer = 0;
  }else {
    time_counyer++;
  }

  /* 运行期参数:
   * user_length: 报警距离阈值，单位 mm
   * user_time:   超时报警阈值，单位 ms
   * key_mode:    按键按下累计时长，用于区分长按阶段
   * Runtime parameters:
   * user_length: alarm distance threshold in mm
   * user_time:   timeout alarm threshold in ms
   * key_mode:    accumulated key press duration for long-press detection
   */
  static uint8_t user_length = 30;
  static uint32_t user_time = 30000;
  static uint32_t key_mode = 0;
  static uint32_t open_time = 0;
  static uint8_t door_State = 0;

  /* 使用状态机切换工作模式，避免按键逻辑相互干扰。 */
  /* Use a state machine to switch modes and keep key logic isolated. */
  static RUN_MODE user_run_mode = 0;

  for (uint8_t i = 0; i < 6; i++) {
    LED_Off(&user_led[i]);
  }

  /* 模式切换：每秒双击两次按键切换到下一模式。 / Mode switch: Double-click within 1s to switch to the next mode. */
  if (Keystroke_Recognition(&key_1,2) == 1) {
    if (user_run_mode < 2) {
      user_run_mode ++;
    }else {
      user_run_mode = 0;
    }
  }

  /* 正常使用模式:
   * Normal operation mode:
   */
  if (user_run_mode == use_mode) {

    if (time_counyer % 1000 == 900) {
      LED_On(&white_led);
    }
    if (time_counyer % 1000 == 0) {
      LED_Off(&white_led);
    }

    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET) {
      key_mode++ ;
    }else {
      key_mode = 0;
    }

  }

  /* 时间设置模式:
   * 每累计约 1 s 增加 30 s 报警时间，并点亮更多用户 LED 表示当前挡位。
   * Time-setting mode:
   * Every ~1 s of key hold adds 30 s to the alarm time and lights more user LEDs.
   */
  if (user_run_mode == time_set_mode) {

    if (time_counyer % 1000 == 700 || time_counyer % 1000 == 950) {
      LED_On(&white_led);
    }
    if (time_counyer % 1000 == 750 || time_counyer % 1000 == 0) {
      LED_Off(&white_led);
    }

    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET) {
      key_mode++ ;
    }else {
      key_mode = 0;
    }
    if (key_mode % 700 == 0 && key_mode != 0 ) {
      user_time += 30000;
      if(user_time_mode < 6) {
        user_time_mode++;
      }else {
        user_time_mode = 1 ;
      }
      if (user_time > 180000) {
        user_time = 30000 ;
      }
    }
  }

  /* 距离设置模式:
   * 每累计约 1 s 读取当前测距值并写入新的报警阈值。
   * Distance-setting mode:
   * Every ~1 s of key hold samples the current distance as the new alarm threshold.
   */
  if (user_run_mode == length_set_mode) {

    if (time_counyer % 1000  == 450 || time_counyer % 1000 == 700 || time_counyer % 1000 == 950) {
      LED_On(&white_led);
    }
    if (time_counyer % 1000 == 500 || time_counyer % 1000 == 750 || time_counyer % 1000 == 0) {
      LED_Off(&white_led);
    }

    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET) {
      key_mode++ ;
    }else {
      key_mode = 0;
    }

    if (key_mode % 1000 == 0) {
      user_length = user_hc_sr04.length;
    }
  }

  if (user_hc_sr04.length > user_length ) {
    if (door_State == 0) {
      open_time = time_counyer;
    }
    door_State = 1;
    if (time_counyer - open_time > user_time ) {
      if (time_counyer % 1000 == 500){
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
      }else if (time_counyer % 1000 == 0) {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
      }
    }
  }else{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
    door_State = 0;
  }

  for (uint8_t i = 0; i < user_time_mode; i++) {
    LED_On(&user_led[i]);
  }





  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

  /* USER CODE END DMA1_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}

/* USER CODE BEGIN 1 */



/* USER CODE END 1 */
