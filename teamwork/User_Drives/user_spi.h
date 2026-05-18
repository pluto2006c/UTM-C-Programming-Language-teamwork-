#ifndef USER_SPI_H
#define USER_SPI_H
#include "main.h"
#ifdef HAL_SPI_MODULE_ENABLED

/* 宏定义 --------------------------------------------------------------------*/
#define SPI_NUM                 (2)         /* SPI 通道数量 */
#define SPI_CALLBACK_NUM       (8)         /* 最大回调函数数量 */
#define SPI_QUEUE_MEMBER_NUM          (20)        /* SPI 发送/接收队列大小 */

/* 类型定义 ------------------------------------------------------------------*/

typedef void (*SPI_Callback)(void* user_spi);

typedef enum {
    SPI_BUSY,
    SPI_OK,
    SPI_FAULT,
}SPI_WORKING_STATUS;

typedef enum {
    SPI_IDLE,
    SPI_TX_WORKING,
    SPI_RX_WORKING,
    SPI_QUEUE_VOID,
}SPI_Status;

typedef struct {
    void* data_ptr;
    uint8_t data_size;
}SPI_QUEUE_MEMBERs;

typedef struct {
    SPI_QUEUE_MEMBERs user_spi_queue_members[SPI_QUEUE_MEMBER_NUM];
    uint8_t queue_write_num;
    uint8_t queue_read_num;
}SPI_QUEUE;

typedef struct {
    uint8_t* rx_data_ptr;
    uint8_t rx_data_size;
}SPI_Rx_Data;


typedef struct {
    SPI_HandleTypeDef* hspi;
    SPI_Status status;
    SPI_QUEUE queue_tx;
    uint8_t tx_data_len;
    SPI_Rx_Data rx_data;
    SPI_Callback callbacks[SPI_CALLBACK_NUM];
    uint8_t callbacks_num;
} SPI_DRIVES;

void SPI_Init(SPI_DRIVES* spi_drives, SPI_HandleTypeDef* hspi);

#endif
#endif //USER_SPI_H
