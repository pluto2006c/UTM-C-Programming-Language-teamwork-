/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint64_t time_counyer ;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/**
 * @brief 不可屏蔽中断处理 / Non-maskable interrupt handler
*/
void NMI_Handler(void);

/**
 * @brief 硬件故障处理 / Hard fault handler
*/
void HardFault_Handler(void);

/**
 * @brief 内存管理故障处理 / Memory management fault handler
*/
void MemManage_Handler(void);

/**
 * @brief 总线故障处理 / Bus fault handler
*/
void BusFault_Handler(void);

/**
 * @brief 使用故障处理 / Usage fault handler
*/
void UsageFault_Handler(void);

/**
 * @brief 系统调用处理 / SVC handler
*/
void SVC_Handler(void);

/**
 * @brief 调试监视器处理 / Debug monitor handler
*/
void DebugMon_Handler(void);

/**
 * @brief 可挂起系统服务处理 / PendSV handler
*/
void PendSV_Handler(void);

/**
 * @brief 系统滴答定时器处理 / SysTick handler
*/
void SysTick_Handler(void);

/**
 * @brief DMA1 通道 6 中断处理 / DMA1 Channel 6 interrupt handler
*/
void DMA1_Channel6_IRQHandler(void);

/**
 * @brief DMA1 通道 7 中断处理 / DMA1 Channel 7 interrupt handler
*/
void DMA1_Channel7_IRQHandler(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */
