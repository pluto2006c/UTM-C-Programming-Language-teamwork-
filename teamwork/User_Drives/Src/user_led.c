#include "../../Core/Inc/bsp.h"
#ifdef HAL_GPIO_MODULE_ENABLED
/* 本文件实现 LED 结构体与 GPIO 操作之间的映射。 */
/* This file maps the LED structure to GPIO operations. */
/* 包含头文件 ----------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "../../User_Drives/user_led.h"
#include <string.h>
/* 函数体 --------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * @brief 初始化 LED / Initialize the LED
 * @param user_led         LED 驱动结构体指针 / Pointer to the LED driver structure
 * @param GPIO        GPIO 端口 / GPIO port
 * @param pin         GPIO 引脚 / GPIO pin
 * @param is_reversal 是否反转逻辑, 1 为反转, 0 为正常 / Inverted logic flag
*/
void LED_Init(LED_DRIVES* user_led, GPIO_TypeDef* GPIO, const uint16_t pin, const uint8_t is_reversal){
    memset(user_led, 0, sizeof(LED_DRIVES));

    /* 记录硬件连接信息，后续控制函数只依赖该结构体。 */
    /* Store the hardware mapping used by the later control APIs. */
    user_led->GPIO = GPIO;
    user_led->pin = pin;
    user_led->is_reversal = is_reversal;
}

/**
 * @brief 设置 LED 状态 / Set the LED state
 * @param user_led   LED 驱动结构体指针 / Pointer to the LED driver structure
 * @param state LED 状态 / Target LED state
*/
void LED_Set_State(const LED_DRIVES* user_led, const LED_State state){
    /* 兼容高电平点亮和低电平点亮两种接法。 */
    /* Support both active-high and active-low wiring. */
    if (user_led->is_reversal) {
        switch (state) {
            case LED_ON:
                HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_RESET);  /*LED 点亮 / LED on*/
                break;
            case LED_OFF:
                HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_SET);    /*LED 熄灭 / LED off*/
                break;
        }
    } else {
        switch (state) {
            case LED_ON:
                HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_SET);    /*LED 点亮 / LED on*/
                break;
            case LED_OFF:
                HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_RESET);  /*LED 熄灭 / LED off*/
                break;
        }
    }
}

/**
 * @brief 点亮 LED / Turn the LED on
 * @param user_led LED 驱动结构体指针 / Pointer to the LED driver structure
*/
void LED_On(const LED_DRIVES* user_led){
    if (user_led->is_reversal)
        HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_RESET);  /*LED 点亮 / LED on*/
    else
        HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_SET);    /*LED 点亮 / LED on*/
}

/**
 * @brief 熄灭 LED / Turn the LED off
 * @param user_led LED 驱动结构体指针 / Pointer to the LED driver structure
*/
void LED_Off(const LED_DRIVES* user_led){
    if (user_led->is_reversal)
        HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_SET);    /*LED 熄灭 / LED off*/
    else
        HAL_GPIO_WritePin(user_led->GPIO, user_led->pin, GPIO_PIN_RESET);  /*LED 熄灭 / LED off*/
}

/**
 * @brief 翻转 LED 状态 / Toggle the LED state
 * @param user_led LED 驱动结构体指针 / Pointer to the LED driver structure
*/
void LED_Toggle(const LED_DRIVES* user_led){
    HAL_GPIO_TogglePin(user_led->GPIO, user_led->pin);
}

/**
 * @brief 获取 LED 状态 / Get the LED state
 * @param user_led LED 驱动结构体指针 / Pointer to the LED driver structure
 * @return LED 状态 / Logical LED state
*/
LED_State LED_Get_State(const LED_DRIVES* user_led){
    const GPIO_PinState pinState = HAL_GPIO_ReadPin(user_led->GPIO, user_led->pin);

    /* 把实际管脚电平还原成统一的 LED 逻辑状态。 */
    /* Convert the physical pin level back into the logical LED state. */
    if (user_led->is_reversal)
        switch (pinState) {
            case GPIO_PIN_RESET:
                return LED_ON;
            case GPIO_PIN_SET:
                return LED_OFF;
        }
    else
        switch (pinState) {
            case GPIO_PIN_SET:
                return LED_ON;
            case GPIO_PIN_RESET:
                return LED_OFF;
        }

    return LED_OFF;
}

#endif /* HAL_GPIO_MODULE_ENABLED */
