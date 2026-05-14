#include "bsp.h"
#include "../../Core/Inc/main.h"
/* 板级全局设备对象在此集中定义，供应用层和中断层共享。 */
/* Global board-level objects are defined here for shared access. */
/*led初始化-----------------------------------------*/
/* LED initialization storage ------------------------------------*/
LED_DRIVES user_led[6] = {0};
LED_DRIVES white_led = {0};
HC_SR04_Drives  user_hc_sr04 = {0};

//初始化全局计时器
// Initialize the global time counter.
uint64_t time_counyer = 0;

/* 默认处于第 1 档计时配置。 */
/* Default timing configuration starts at level 1. */
uint8_t user_time_mode = 1;

Keystroke_Recognition_data key_1 = {0}; /* 按键 1 实例初始化 / Initialize key 1 instance */

/**
 * @brief 初始化按键 / Initialize the key
 * @param key       按键数据结构指针 / Pointer to the key data structure
 * @param GPIOx     GPIO 端口 / GPIO port
 * @param GPIO_Pin  GPIO 引脚 / GPIO pin
*/
void Key_Init(Keystroke_Recognition_data* key ,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    key->GPIO_Pin =GPIO_Pin;
    key->GPIOx = GPIOx;
}

/**
 * @brief 按键识别 / Keystroke recognition
 * @param key  按键数据结构指针 / Pointer to the key data structure
 * @param num  目标按下次数 / Target number of presses
 * @return uint8_t 1 表示识别成功, 0 表示未识别 / 1 if recognized, 0 otherwise
*/
uint8_t Keystroke_Recognition (Keystroke_Recognition_data* key, uint8_t num ) {

    /* 按键按下识别 (初始状态) / Key press recognition (Initial state) */
    if (HAL_GPIO_ReadPin(key->GPIOx,key->GPIO_Pin) == GPIO_PIN_SET && key->key_down_num_s == 0 && key->key_read_mode == 0) {
        key->time_flash = time_counyer;
        key->key_down_num_s = 1;
        key->key_read_mode = 1;
    }

    /* 按键按下识别 / Key press recognition */

    /* 检测到按下后用状态机消除电压抖动防止误读 / Use state machine to debounce key press */
    if (HAL_GPIO_ReadPin(key->GPIOx,key->GPIO_Pin) == GPIO_PIN_SET && key->key_down_num_s != 0 && key->key_read_mode == 0) {
        key->key_time_flash = time_counyer;
        key->key_down_num_s ++;
        key->key_read_mode = 1 ;
    }

    /* 状态机复位 / State machine reset */

    if (time_counyer - key->key_time_flash > 50 && key->key_read_mode == 1 &&HAL_GPIO_ReadPin(key->GPIOx,key->GPIO_Pin) == GPIO_PIN_RESET) {
        key->key_read_mode = 0;
        key->key_time_flash = 0;
    }

    if ((time_counyer - key->time_flash) > 1000 ) {
        key->key_down_num_s = 0;
    }

    if (key->key_down_num_s == num) {
        key->key_down_num_s = 0;
        return 1;
    }else {
        return 0;
    }
}

/**
 * @brief 初始化超声波测距仪 / Initialize the ultrasonic rangefinder
 * @param hc_sr04     驱动结构体指针 / Pointer to the driver structure
 * @param tim         定时器句柄 / Timer handle
 * @param outGPIOx    Trig 端口 / Trig port
 * @param outGPIO_Pin Trig 引脚 / Trig pin
 * @param inGPIOx     Echo 端口 / Echo port
 * @param inGPIO_Pin  Echo 引脚 / Echo pin
*/
void HC_SR04_INIT(HC_SR04_Drives* hc_sr04, TIM_HandleTypeDef* tim , GPIO_TypeDef* outGPIOx, uint16_t outGPIO_Pin , GPIO_TypeDef* inGPIOx, uint16_t inGPIO_Pin) {
    hc_sr04->tim = tim ;
    hc_sr04->outGPIOx = outGPIOx ;
    hc_sr04->outGPIO_Pin = outGPIO_Pin ;
    hc_sr04->inGPIOx = inGPIOx ;
    hc_sr04->inGPIO_Pin = inGPIO_Pin ;
}

/**
 * @brief 获取测量距离 / Get measured distance
 * @param hc_sr04 驱动结构体指针 / Pointer to the driver structure
*/
void HC_SR04_getlength(HC_SR04_Drives* hc_sr04 ) {

    /* 触发测量 / Trigger measurement */
    if (hc_sr04->run_mode == 0) {
        HAL_GPIO_WritePin(hc_sr04->outGPIOx,hc_sr04->outGPIO_Pin,GPIO_PIN_SET);
        hc_sr04->run_mode = 1;
        hc_sr04->tem_time_flash = time_counyer;
    }

    /* 维持 Trig 高电平 10us 以上 / Keep Trig high for at least 10us */
    if (hc_sr04->run_mode == 1 && hc_sr04->tem_time_flash - time_counyer > 1) {
        HAL_GPIO_WritePin(hc_sr04->outGPIOx,hc_sr04->outGPIO_Pin,GPIO_PIN_RESET);
        __HAL_TIM_SET_COUNTER(hc_sr04->tim, 0);
        hc_sr04->run_mode = 2;
    }

    /* 等待 Echo 高电平并开始计时 / Wait for Echo high and start timer */
    if (hc_sr04->run_mode == 2 && HAL_GPIO_ReadPin(hc_sr04->inGPIOx,hc_sr04->inGPIO_Pin) == GPIO_PIN_SET) {
        __HAL_TIM_ENABLE(hc_sr04->tim);
        hc_sr04->run_mode = 3;
    }

    /* 等待 Echo 低电平并停止计时 / Wait for Echo low and stop timer */
    if (hc_sr04->run_mode == 3 && HAL_GPIO_ReadPin(hc_sr04->inGPIOx,hc_sr04->inGPIO_Pin) == GPIO_PIN_RESET) {
        __HAL_TIM_DISABLE(hc_sr04->tim);
        hc_sr04->length = __HAL_TIM_GET_COUNTER(hc_sr04->tim) * 0.17 ; /* 换算成距离 (mm) / Convert to distance (mm) */
        hc_sr04->run_mode = 0;
    }

    /* 超时处理 / Timeout handling */
    if (__HAL_TIM_GET_COUNTER(hc_sr04->tim) > 23300 && hc_sr04->run_mode == 3) {
        __HAL_TIM_DISABLE(hc_sr04->tim);
        hc_sr04->length = 400;
        hc_sr04->run_mode = 0;
    }
}

