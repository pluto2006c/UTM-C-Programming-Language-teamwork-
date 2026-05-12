#ifndef USER_LED_H
#define USER_LED_H
#include "main.h"
#ifdef HAL_GPIO_MODULE_ENABLED

/* LED 轻量驱动接口，封装 GPIO 电平与逻辑反相关系。 */
/* Lightweight LED driver interface wrapping GPIO polarity handling. */
/* 包含头文件 ----------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/

/* 类型定义 ------------------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/**
* @brief LED 状态枚举 / LED state enumeration
*/
typedef enum {
    LED_OFF = 0,  /* LED 熄灭状态 / LED off state */
    LED_ON = 1,   /* LED 点亮状态 / LED on state */
} LED_State;

/**
* @brief LED 驱动结构体 / LED driver structure
*/
typedef struct {
    GPIO_TypeDef* GPIO;  /* 存储 GPIO 端口 / Stores the GPIO port */
    uint16_t pin;        /* 存储 GPIO 引脚 / Stores the GPIO pin */
    uint8_t is_reversal; /* 存储是否反转逻辑，1 为反转，0 为正常 / Polarity inversion flag */
} LED_DRIVES;

/* 函数声明 ------------------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/

/**
 * @brief 初始化 LED / Initialize the LED
 * @param user_led    LED 驱动结构体指针 / Pointer to the LED driver structure
 * @param GPIO        GPIO 端口 / GPIO port
 * @param pin         GPIO 引脚 / GPIO pin
 * @param is_reversal 是否反转逻辑 / Polarity inversion flag
*/
void LED_Init(LED_DRIVES* user_led, GPIO_TypeDef* GPIO, uint16_t pin, uint8_t is_reversal);

/**
 * @brief 设置 LED 状态 / Set the LED state
 * @param user_led    LED 驱动结构体指针 / Pointer to the LED driver structure
 * @param state       目标 LED 状态 / Target LED state
*/
void LED_Set_State(const LED_DRIVES* user_led, LED_State state);

/**
 * @brief 点亮 LED / Turn the LED on
 * @param user_led    LED 驱动结构体指针 / Pointer to the LED driver structure
*/
void LED_On(const LED_DRIVES* user_led);

/**
 * @brief 熄灭 LED / Turn the LED off
 * @param user_led    LED 驱动结构体指针 / Pointer to the LED driver structure
*/
void LED_Off(const LED_DRIVES* user_led);

/**
 * @brief 翻转 LED 状态 / Toggle the LED state
 * @param user_led    LED 驱动结构体指针 / Pointer to the LED driver structure
*/
void LED_Toggle(const LED_DRIVES* user_led);

/**
 * @brief 获取 LED 状态 / Get the LED state
 * @param user_led    LED 驱动结构体指针 / Pointer to the LED driver structure
 * @return LED_State  逻辑 LED 状态 / Logical LED state
*/
LED_State LED_Get_State(const LED_DRIVES* user_led);

#endif /* HAL_GPIO_MODULE_ENABLED */
#endif // USER_LED_H
