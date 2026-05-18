#include "../../Core/Inc/bsp.h"

#ifdef HAL_SPI_MODULE_ENABLED

#include "../user_spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 私有变量 ------------------------------------------------------------------ */
static SPI_DRIVES* spi_drives[SPI_NUM];
static uint8_t spi_num = 0;

/* 私有函数 -------------------------------------------------------------------*/

static void SPI_Queue_Free(SPI_QUEUE* queue ,uint8_t free_num);
void SPI_Queue_Push(SPI_QUEUE* queue, uint8_t* data ,uint8_t size);
static void SPI_Queue_Init(SPI_QUEUE* queue);

/* 函数实现 -------------------------------------------------------------------*/


void SPI_Init(SPI_DRIVES* user_spi, SPI_HandleTypeDef* hspi){
    spi_drives[spi_num] = user_spi;
    user_spi->hspi = hspi;
    user_spi->queue_tx.queue_write_num = 0;
    user_spi->queue_tx.queue_read_num = 0;
    user_spi->status = SPI_IDLE;
    SPI_Queue_Init(&user_spi->queue_tx);
    spi_num ++;
}

void SPI_Queue_Init(SPI_QUEUE* queue) {
    for (int i = 0; i < SPI_QUEUE_MEMBER_NUM; i++) {
        queue->user_spi_queue_members[i].data_ptr = NULL;
        queue->user_spi_queue_members[i].data_size = 0;
    }
    queue->queue_write_num = 0;
    queue->queue_read_num = 0;
}

void SPI_Queue_Push(SPI_QUEUE* queue, uint8_t* data ,uint8_t size){
    if (queue->user_spi_queue_members[queue->queue_write_num].data_ptr == NULL) {
        queue->user_spi_queue_members[queue->queue_write_num].data_size = size;
        queue->user_spi_queue_members[queue->queue_write_num].data_ptr = malloc(size);
        memcpy(queue->user_spi_queue_members[queue->queue_write_num].data_ptr, data, size);
    }else {
        SPI_Queue_Free(queue, queue->queue_write_num);
        queue->user_spi_queue_members[queue->queue_write_num].data_size = size;
        queue->user_spi_queue_members[queue->queue_write_num].data_ptr = malloc(size);
        memcpy(queue->user_spi_queue_members[queue->queue_write_num].data_ptr, data, size);
    }
    queue->queue_write_num++;
    if (queue->queue_write_num > SPI_QUEUE_MEMBER_NUM - 1) {
        queue->queue_write_num = 0;
    }
}

void SPI_Queue_Free(SPI_QUEUE* queue ,uint8_t free_num) {
    free(queue->user_spi_queue_members[free_num].data_ptr);
    queue->user_spi_queue_members[free_num].data_ptr = NULL;
    queue->user_spi_queue_members[free_num].data_size = 0;
}

uint8_t SPI_Send(SPI_DRIVES* user_spi, uint8_t* data, uint8_t size) {
    user_spi->queue_tx.user_spi_queue_members[user_spi->queue_tx.queue_write_num].data_size = size;
    SPI_Queue_Push(&user_spi->queue_tx, data,user_spi->queue_tx.user_spi_queue_members[user_spi->queue_tx.queue_write_num].data_size);
    if (user_spi->queue_tx.user_spi_queue_members[user_spi->queue_tx.queue_read_num].data_ptr != NULL) {
        if (user_spi->status == SPI_IDLE) {
            user_spi->status = SPI_TX_WORKING;
            HAL_SPI_Transmit_DMA(user_spi->hspi, user_spi->queue_tx.user_spi_queue_members[user_spi->queue_tx.queue_read_num].data_ptr, user_spi->queue_tx.user_spi_queue_members[user_spi->queue_tx.queue_read_num].data_size);
            return SPI_OK;
        }
        if (user_spi->status == SPI_TX_WORKING) {
            return SPI_BUSY;
        }
    }
    return SPI_QUEUE_VOID;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    for (uint8_t user_spi_num = 0; user_spi_num < spi_num; user_spi_num++) {
        if (spi_drives[user_spi_num]->hspi == hspi) {
            SPI_Queue_Free(&spi_drives[user_spi_num]->queue_tx ,spi_drives[user_spi_num]->queue_tx.queue_read_num);
            spi_drives[user_spi_num]->queue_tx.queue_read_num++;
            if (spi_drives[user_spi_num]->queue_tx.queue_read_num > SPI_QUEUE_MEMBER_NUM - 1) {
                spi_drives[user_spi_num]->queue_tx.queue_read_num = 0;
            }
            if (spi_drives[user_spi_num]->callbacks_num > 0) {
                for (int j = 0; j < spi_drives[user_spi_num]->callbacks_num; j++) {
                    spi_drives[user_spi_num]->callbacks[j](spi_drives[user_spi_num]);
                }
            }
            spi_drives[user_spi_num]->status = SPI_IDLE;
        }
    }
}

uint8_t SPI_Data_Get(SPI_DRIVES* user_spi, uint8_t* data, uint8_t size) {
    if (user_spi->status != SPI_IDLE) {
        return SPI_BUSY;
    }
    user_spi->rx_data.rx_data_ptr = data;
    user_spi->rx_data.rx_data_size = size;
    HAL_SPI_Receive_DMA(user_spi->hspi, user_spi->rx_data.rx_data_ptr, user_spi->rx_data.rx_data_size);
    user_spi->status = SPI_RX_WORKING;
    return SPI_OK;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    for (uint8_t user_spi_num = 0; user_spi_num < spi_num; user_spi_num++) {
        if (spi_drives[user_spi_num]->hspi == hspi) {
            spi_drives[user_spi_num]->status = SPI_IDLE;
        }
    }
}

void User_SPI_Callback_Init(SPI_DRIVES* user_spi, SPI_Callback callback) {
    if (user_spi->callbacks_num < SPI_CALLBACK_NUM) {
        user_spi->callbacks[user_spi->callbacks_num] = callback;
        user_spi->callbacks_num++;
    }
}

void User_SPI_QueueHandle() {
    for (uint8_t user_spi_num = 0; user_spi_num < spi_num; user_spi_num++) {
        for (uint8_t user_callbacks_num = 0; user_callbacks_num < spi_drives[user_spi_num]->callbacks_num; user_callbacks_num++) {
             spi_drives[user_spi_num]->callbacks[user_callbacks_num](spi_drives[user_spi_num]);
        }
    }
}

void User_SPI_Peripheral_Init(SPI_DRIVES* user_spi, SPI_HandleTypeDef* hspi) {
    user_spi->hspi = hspi;
}



#endif
