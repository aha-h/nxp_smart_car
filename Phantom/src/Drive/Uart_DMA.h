#ifndef __UART_DMA_H
#define __UART_DMA_H

#include "fsl_edma.h"

typedef struct 
{
    volatile uint8_t txOnGoing         :        1;    //串口TX正在发送标志位
    volatile uint8_t rxOnGoing         :        1;    //串口RX正在接收标志位
    volatile uint8_t txBufferFull      :        1;    //串口TX发送寄存器满标志位
    volatile uint8_t rxBufferEmpty     :        1;    //串口RX接收寄存器空标志位
    
    volatile uint8_t get_pid_group1    :        1;    //上位机请求，为1时，单片机需要发送pid参数给上位机
}volatile status_t;

extern status_t status;


/* Forward declaration of the handle typedef. */
typedef struct _uart_edma_handle uart_edma_handle_t;

/*! @brief UART transfer callback function. */
typedef void (*uart_edma_transfer_callback_t)(UART_MemMapPtr base,
                                              uart_edma_handle_t *handle,
                                              int status,
                                              void *userData);



/*!
* @brief UART eDMA handle
*/
struct _uart_edma_handle
{
    uart_edma_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                         /*!< UART callback function parameter.*/
    size_t rxDataSizeAll;                   /*!< Size of the data to receive. */
    size_t txDataSizeAll;                   /*!< Size of the data to send out. */

    edma_handle_t *txEdmaHandle; /*!< The eDMA TX channel used. */
    edma_handle_t *rxEdmaHandle; /*!< The eDMA RX channel used. */

    uint8_t nbytes; /*!< eDMA minor byte transfer count initially configured. */

    volatile uint8_t txState; /*!< TX transfer state. */
    volatile uint8_t rxState; /*!< RX transfer state */
};


/*! @brief UART transfer structure. */
typedef struct _uart_transfer
{
    uint8_t *data;   /*!< The buffer of data to be transfer.*/
    size_t dataSize; /*!< The byte count to be transfer. */
} uart_transfer_t;

/*!
 * @brief Gets the UART data register address.
 *
 * This function returns the UART data register address, which is mainly used by DMA/eDMA.
 *
 * @param base UART peripheral base address.
 * @return UART data register addresses which are used both by the transmitter and the receiver.
 */
static inline uint32_t UART_GetDataRegisterAddress(UART_MemMapPtr base)
{
    return (uint32_t) & (base->D);
}

/**
  * @brief    ����4 + DMA����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/14 ���ڶ�
  */
void Uart4_DMA_Init(void);

void EDMA_PrepareTransfer(edma_transfer_config_t *config,
                          void *srcAddr,
                          uint32_t srcWidth,
                          void *destAddr,
                          uint32_t destWidth,
                          uint32_t bytesEachRequest,
                          uint32_t transferBytes,
                          edma_transfer_type_t type);

int UART_SendEDMA(UART_MemMapPtr base, uart_edma_handle_t *handle, uart_transfer_t *xfer);

int UART_ReceiveEDMA(UART_MemMapPtr base, uart_edma_handle_t *handle, uart_transfer_t *xfer);

void UART_TransferAbortSendEDMA(UART_MemMapPtr base, uart_edma_handle_t *handle);

static inline void UART_EnableTxDMA(UART_MemMapPtr base, uint8_t enable);

static inline void UART_EnableRxDMA(UART_MemMapPtr base, uint8_t enable);

void UART_TransferAbortReceiveEDMA(UART_MemMapPtr base, uart_edma_handle_t *handle);

int UART_TransferGetSendCountEDMA(UART_MemMapPtr base, uart_edma_handle_t *handle, uint32_t *count);

int UART_TransferGetReceiveCountEDMA(UART_MemMapPtr base, uart_edma_handle_t *handle, uint32_t *count);

extern uart_edma_handle_t g_uartEdmaHandle;

/**
  * @brief    ���Դ���+DMA
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/14 ���ڶ�
  */
void Test_UART_DMA(void);


#endif
