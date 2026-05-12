#ifndef DOOR_SENSOR_BSP_H
#define DOOR_SENSOR_BSP_H

/* 板级支持包汇总了本项目会跨模块共享的设备实例与运行状态。 */
/* Board support package declarations shared across modules. */
/*led注册--------------------------------------------------*/
/* LED registration -----------------------------------------------*/
#include "../../User_Drives/user_led.h"



/* 6 个用户指示灯与 1 个红色告警灯实例。 */
/* Six user indicator LEDs and one red alarm LED instance. */
extern LED_DRIVES user_led[6];
extern LED_DRIVES white_led;

/* 当前设置的计时挡位，对应点亮的用户 LED 数量。 */
/* Current timing level, reflected by the number of lit user LEDs. */
extern uint8_t user_time_mode;



typedef enum {
    use_mode = 0,        /* 使用模式 / Usage mode */
    time_set_mode = 1,   /* 计时设置模式 / Time setting mode */
    length_set_mode = 2, /* 长度设置模式 / Length setting mode */
}RUN_MODE ;

/**
 * @brief 按键识别数据结构 / Keystroke recognition data structure
*/
typedef struct {
    uint64_t time_flash ;       /* 刷新时间 / Refresh time */
    uint64_t key_time_flash ;   /* 按键刷新时间 / Key refresh time */
    uint8_t key_down_num_s ;    /* 按键按下次数 / Key press count */
    uint8_t key_read_mode ;     /* 按键读取模式 / Key read mode */
    GPIO_TypeDef* GPIOx;        /* GPIO 端口 / GPIO port */
    uint16_t GPIO_Pin;          /* GPIO 引脚 / GPIO pin */
}Keystroke_Recognition_data;

/**
 * @brief 初始化按键 / Initialize the key
 * @param key       按键数据结构指针 / Pointer to the key data structure
 * @param GPIOx     GPIO 端口 / GPIO port
 * @param GPIO_Pin  GPIO 引脚 / GPIO pin
*/
void Key_Init(Keystroke_Recognition_data* key ,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief 按键识别 / Keystroke recognition
 * @param key  按键数据结构指针 / Pointer to the key data structure
 * @param num  目标按下次数 / Target number of presses
 * @return uint8_t 1 表示识别成功, 0 表示未识别 / 1 if recognized, 0 otherwise
*/
uint8_t Keystroke_Recognition (Keystroke_Recognition_data* key, uint8_t num );

extern Keystroke_Recognition_data key_1; /* 按键 1 实例 / Key 1 instance */

#endif //DOOR_SENSOR_BSP_H
