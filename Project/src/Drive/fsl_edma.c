/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "include.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EDMA_TRANSFER_ENABLED_MASK 0x80U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/



/*!
 * @brief Push content of TCD structure into hardware TCD register.
 *
 * @param base EDMA peripheral base address.
 * @param channel EDMA channel number.
 * @param tcd Point to TCD structure.
 */
static void EDMA_InstallTCD(DMA_MemMapPtr base, uint32_t channel, edma_tcd_t *tcd);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map EDMA instance number to base pointer. */
static DMA_MemMapPtr const s_edmaBases[] = {DMA_BASE_PTR};



/*! @brief Pointers to transfer handle for each EDMA channel. */
static edma_handle_t *s_EDMAHandle[32];

/*******************************************************************************
 * Code
 ******************************************************************************/


static void EDMA_InstallTCD(DMA_MemMapPtr base, uint32_t channel, edma_tcd_t *tcd)
{

    /* Push tcd into hardware TCD register */
    base->TCD[channel].SADDR = tcd->SADDR;
    base->TCD[channel].SOFF = tcd->SOFF;
    base->TCD[channel].ATTR = tcd->ATTR;
    base->TCD[channel].NBYTES_MLNO = tcd->NBYTES;
    base->TCD[channel].SLAST = tcd->SLAST;
    base->TCD[channel].DADDR = tcd->DADDR;
    base->TCD[channel].DOFF = tcd->DOFF;
    base->TCD[channel].CITER_ELINKNO = tcd->CITER;
    base->TCD[channel].DLAST_SGA = tcd->DLAST_SGA;
    /* Clear DONE bit first, otherwise ESG cannot be set */
    base->TCD[channel].CSR = 0;
    base->TCD[channel].CSR = tcd->CSR;
    base->TCD[channel].BITER_ELINKNO = tcd->BITER;
}


void EDMA_ResetChannel(DMA_MemMapPtr base, uint32_t channel)
{


    EDMA_TcdReset((edma_tcd_t *)&base->TCD[channel]);
}

void EDMA_SetTransferConfig(DMA_MemMapPtr base, uint32_t channel, const edma_transfer_config_t *config, edma_tcd_t *nextTcd)
{

    EDMA_TcdSetTransferConfig((edma_tcd_t *)&base->TCD[channel], config, nextTcd);
}

void EDMA_SetMinorOffsetConfig(DMA_MemMapPtr base, uint32_t channel, const edma_minor_offset_config_t *config)
{
    uint32_t tmpreg;

    tmpreg = base->TCD[channel].NBYTES_MLOFFYES;
    tmpreg &= ~(DMA_NBYTES_MLOFFYES_SMLOE_MASK | DMA_NBYTES_MLOFFYES_DMLOE_MASK | DMA_NBYTES_MLOFFYES_MLOFF_MASK);
    tmpreg |=
        (DMA_NBYTES_MLOFFYES_SMLOE(config->enableSrcMinorOffset) |
         DMA_NBYTES_MLOFFYES_DMLOE(config->enableDestMinorOffset) | DMA_NBYTES_MLOFFYES_MLOFF(config->minorOffset));
    base->TCD[channel].NBYTES_MLOFFYES = tmpreg;
}

void EDMA_SetChannelLink(DMA_MemMapPtr base, uint32_t channel, edma_channel_link_type_t type, uint32_t linkedChannel)
{

    EDMA_TcdSetChannelLink((edma_tcd_t *)&base->TCD[channel], type, linkedChannel);
}

void EDMA_SetBandWidth(DMA_MemMapPtr base, uint32_t channel, edma_bandwidth_t bandWidth)
{

    base->TCD[channel].CSR = (base->TCD[channel].CSR & (~DMA_CSR_BWC_MASK)) | DMA_CSR_BWC(bandWidth);
}

void EDMA_SetModulo(DMA_MemMapPtr base, uint32_t channel, edma_modulo_t srcModulo, edma_modulo_t destModulo)
{

    uint32_t tmpreg;

    tmpreg = base->TCD[channel].ATTR & (~(DMA_ATTR_SMOD_MASK | DMA_ATTR_DMOD_MASK));
    base->TCD[channel].ATTR = tmpreg | DMA_ATTR_DMOD(destModulo) | DMA_ATTR_SMOD(srcModulo);
}

void EDMA_EnableChannelInterrupts(DMA_MemMapPtr base, uint32_t channel, uint32_t mask)
{

    /* Enable error interrupt */
    if (mask & kEDMA_ErrorInterruptEnable)
    {
        base->EEI |= (0x1U << channel);
    }

    /* Enable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable)
    {
        base->TCD[channel].CSR |= DMA_CSR_INTMAJOR_MASK;
    }

    /* Enable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable)
    {
        base->TCD[channel].CSR |= DMA_CSR_INTHALF_MASK;
    }
}

void EDMA_DisableChannelInterrupts(DMA_MemMapPtr base, uint32_t channel, uint32_t mask)
{

    /* Disable error interrupt */
    if (mask & kEDMA_ErrorInterruptEnable)
    {
        base->EEI &= ~(0x1U << channel);
    }

    /* Disable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable)
    {
        base->TCD[channel].CSR &= ~DMA_CSR_INTMAJOR_MASK;
    }

    /* Disable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable)
    {
        base->TCD[channel].CSR &= ~DMA_CSR_INTHALF_MASK;
    }
}

void EDMA_TcdReset(edma_tcd_t *tcd)
{

    /* Reset channel TCD */
    tcd->SADDR = 0U;
    tcd->SOFF = 0U;
    tcd->ATTR = 0U;
    tcd->NBYTES = 0U;
    tcd->SLAST = 0U;
    tcd->DADDR = 0U;
    tcd->DOFF = 0U;
    tcd->CITER = 0U;
    tcd->DLAST_SGA = 0U;
    /* Enable auto disable request feature */
    tcd->CSR = DMA_CSR_DREQ(true);
    tcd->BITER = 0U;
}

void EDMA_TcdSetTransferConfig(edma_tcd_t *tcd, const edma_transfer_config_t *config, edma_tcd_t *nextTcd)
{

    /* source address */
    tcd->SADDR = config->srcAddr;
    /* destination address */
    tcd->DADDR = config->destAddr;
    /* Source data and destination data transfer size */
    tcd->ATTR = DMA_ATTR_SSIZE(config->srcTransferSize) | DMA_ATTR_DSIZE(config->destTransferSize);
    /* Source address signed offset */
    tcd->SOFF = config->srcOffset;
    /* Destination address signed offset */
    tcd->DOFF = config->destOffset;
    /* Minor byte transfer count */
    tcd->NBYTES = config->minorLoopBytes;
    /* Current major iteration count */
    tcd->CITER = config->majorLoopCounts;
    /* Starting major iteration count */
    tcd->BITER = config->majorLoopCounts;
    /* Enable scatter/gather processing */
    if (nextTcd != NULL)
    {
        tcd->DLAST_SGA = (uint32_t)nextTcd;
        /*
            Before call EDMA_TcdSetTransferConfig or EDMA_SetTransferConfig,
            user must call EDMA_TcdReset or EDMA_ResetChannel which will set
            DREQ, so must use "|" or "&" rather than "=".

            Clear the DREQ bit because scatter gather has been enabled, so the
            previous transfer is not the last transfer, and channel request should
            be enabled at the next transfer(the next TCD).
        */
        tcd->CSR = (tcd->CSR | DMA_CSR_ESG_MASK) & ~DMA_CSR_DREQ_MASK;
    }
}

void EDMA_TcdSetMinorOffsetConfig(edma_tcd_t *tcd, const edma_minor_offset_config_t *config)
{

    uint32_t tmpreg;

    tmpreg = tcd->NBYTES &
             ~(DMA_NBYTES_MLOFFYES_SMLOE_MASK | DMA_NBYTES_MLOFFYES_DMLOE_MASK | DMA_NBYTES_MLOFFYES_MLOFF_MASK);
    tmpreg |=
        (DMA_NBYTES_MLOFFYES_SMLOE(config->enableSrcMinorOffset) |
         DMA_NBYTES_MLOFFYES_DMLOE(config->enableDestMinorOffset) | DMA_NBYTES_MLOFFYES_MLOFF(config->minorOffset));
    tcd->NBYTES = tmpreg;
}

void EDMA_TcdSetChannelLink(edma_tcd_t *tcd, edma_channel_link_type_t type, uint32_t linkedChannel)
{

    if (type == kEDMA_MinorLink) /* Minor link config */
    {
        uint32_t tmpreg;

        /* Enable minor link */
        tcd->CITER |= DMA_CITER_ELINKYES_ELINK_MASK;
        tcd->BITER |= DMA_BITER_ELINKYES_ELINK_MASK;
        /* Set likned channel */
        tmpreg = tcd->CITER & (~DMA_CITER_ELINKYES_LINKCH_MASK);
        tmpreg |= DMA_CITER_ELINKYES_LINKCH(linkedChannel);
        tcd->CITER = tmpreg;
        tmpreg = tcd->BITER & (~DMA_BITER_ELINKYES_LINKCH_MASK);
        tmpreg |= DMA_BITER_ELINKYES_LINKCH(linkedChannel);
        tcd->BITER = tmpreg;
    }
    else if (type == kEDMA_MajorLink) /* Major link config */
    {
        uint32_t tmpreg;

        /* Enable major link */
        tcd->CSR |= DMA_CSR_MAJORELINK_MASK;
        /* Set major linked channel */
        tmpreg = tcd->CSR & (~DMA_CSR_MAJORLINKCH_MASK);
        tcd->CSR = tmpreg | DMA_CSR_MAJORLINKCH(linkedChannel);
    }
    else /* Link none */
    {
        tcd->CITER &= ~DMA_CITER_ELINKYES_ELINK_MASK;
        tcd->BITER &= ~DMA_BITER_ELINKYES_ELINK_MASK;
        tcd->CSR &= ~DMA_CSR_MAJORELINK_MASK;
    }
}

void EDMA_TcdSetModulo(edma_tcd_t *tcd, edma_modulo_t srcModulo, edma_modulo_t destModulo)
{

    uint32_t tmpreg;

    tmpreg = tcd->ATTR & (~(DMA_ATTR_SMOD_MASK | DMA_ATTR_DMOD_MASK));
    tcd->ATTR = tmpreg | DMA_ATTR_DMOD(destModulo) | DMA_ATTR_SMOD(srcModulo);
}

void EDMA_TcdEnableInterrupts(edma_tcd_t *tcd, uint32_t mask)
{

    /* Enable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable)
    {
        tcd->CSR |= DMA_CSR_INTMAJOR_MASK;
    }

    /* Enable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable)
    {
        tcd->CSR |= DMA_CSR_INTHALF_MASK;
    }
}

void EDMA_TcdDisableInterrupts(edma_tcd_t *tcd, uint32_t mask)
{

    /* Disable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable)
    {
        tcd->CSR &= ~DMA_CSR_INTMAJOR_MASK;
    }

    /* Disable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable)
    {
        tcd->CSR &= ~DMA_CSR_INTHALF_MASK;
    }
}

uint32_t EDMA_GetRemainingMajorLoopCount(DMA_MemMapPtr base, uint32_t channel)
{

    uint32_t remainingCount = 0;

    if (DMA_CSR_DONE_MASK & base->TCD[channel].CSR)
    {
        remainingCount = 0;
    }
    else
    {
        /* Calculate the unfinished bytes */
        if (base->TCD[channel].CITER_ELINKNO & DMA_CITER_ELINKNO_ELINK_MASK)
        {
            remainingCount =
                (base->TCD[channel].CITER_ELINKYES & DMA_CITER_ELINKYES_CITER_MASK) >> DMA_CITER_ELINKYES_CITER_SHIFT;
        }
        else
        {
            remainingCount =
                (base->TCD[channel].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK) >> DMA_CITER_ELINKNO_CITER_SHIFT;
        }
    }

    return remainingCount;
}

uint32_t EDMA_GetChannelStatusFlags(DMA_MemMapPtr base, uint32_t channel)
{

    uint32_t retval = 0;

    /* Get DONE bit flag */
    retval |= ((base->TCD[channel].CSR & DMA_CSR_DONE_MASK) >> DMA_CSR_DONE_SHIFT);
    /* Get ERROR bit flag */
    retval |= (((base->ERR >> channel) & 0x1U) << 1U);
    /* Get INT bit flag */
    retval |= (((base->INT >> channel) & 0x1U) << 2U);

    return retval;
}

void EDMA_ClearChannelStatusFlags(DMA_MemMapPtr base, uint32_t channel, uint32_t mask)
{

    /* Clear DONE bit flag */
    if (mask & kEDMA_DoneFlag)
    {
        base->CDNE = channel;
    }
    /* Clear ERROR bit flag */
    if (mask & kEDMA_ErrorFlag)
    {
        base->CERR = channel;
    }
    /* Clear INT bit flag */
    if (mask & kEDMA_InterruptFlag)
    {
        base->CINT = channel;
    }
}

void EDMA_CreateHandle(edma_handle_t *handle, DMA_MemMapPtr base, uint32_t channel)
{

    uint32_t edmaInstance;
    uint32_t channelIndex;
    edma_tcd_t *tcdRegs;

    /* Zero the handle */
//    memset(handle, 0, sizeof(*handle));

    handle->base = base;
    handle->channel = channel;
    /* Get the DMA instance number */
    edmaInstance = 0;
    channelIndex = edmaInstance + channel;
    s_EDMAHandle[channelIndex] = handle;

    /* Enable NVIC interrupt */
    NVIC_EnableIRQ((IRQn_Type)channelIndex);

    /*
       Reset TCD registers to zero. Unlike the EDMA_TcdReset(DREQ will be set),
       CSR will be 0. Because in order to suit EDMA busy check mechanism in
       EDMA_SubmitTransfer, CSR must be set 0.
    */
    tcdRegs = (edma_tcd_t *)&handle->base->TCD[handle->channel];
    tcdRegs->SADDR = 0;
    tcdRegs->SOFF = 0;
    tcdRegs->ATTR = 0;
    tcdRegs->NBYTES = 0;
    tcdRegs->SLAST = 0;
    tcdRegs->DADDR = 0;
    tcdRegs->DOFF = 0;
    tcdRegs->CITER = 0;
    tcdRegs->DLAST_SGA = 0;
    tcdRegs->CSR = 0;
    tcdRegs->BITER = 0;
}

void EDMA_InstallTCDMemory(edma_handle_t *handle, edma_tcd_t *tcdPool, uint32_t tcdSize)
{

    /* Initialize tcd queue attibute. */
    handle->header = 0;
    handle->tail = 0;
    handle->tcdUsed = 0;
    handle->tcdSize = tcdSize;
    handle->flags = 0;
    handle->tcdPool = tcdPool;
}

void EDMA_SetCallback(edma_handle_t *handle, edma_callback callback, void *userData)
{

    handle->callback = callback;
    handle->userData = userData;
}

void EDMA_PrepareTransfer(edma_transfer_config_t *config,
                          void *srcAddr,
                          uint32_t srcWidth,
                          void *destAddr,
                          uint32_t destWidth,
                          uint32_t bytesEachRequest,
                          uint32_t transferBytes,
                          edma_transfer_type_t type)
{

    config->destAddr = (uint32_t)destAddr;
    config->srcAddr = (uint32_t)srcAddr;
    config->minorLoopBytes = bytesEachRequest;
    config->majorLoopCounts = transferBytes / bytesEachRequest;
    switch (srcWidth)
    {
        case 1U:
            config->srcTransferSize = kEDMA_TransferSize1Bytes;
            break;
        case 2U:
            config->srcTransferSize = kEDMA_TransferSize2Bytes;
            break;
        case 4U:
            config->srcTransferSize = kEDMA_TransferSize4Bytes;
            break;
        case 16U:
            config->srcTransferSize = kEDMA_TransferSize16Bytes;
            break;
        case 32U:
            config->srcTransferSize = kEDMA_TransferSize32Bytes;
            break;
        default:
            break;
    }
    switch (destWidth)
    {
        case 1U:
            config->destTransferSize = kEDMA_TransferSize1Bytes;
            break;
        case 2U:
            config->destTransferSize = kEDMA_TransferSize2Bytes;
            break;
        case 4U:
            config->destTransferSize = kEDMA_TransferSize4Bytes;
            break;
        case 16U:
            config->destTransferSize = kEDMA_TransferSize16Bytes;
            break;
        case 32U:
            config->destTransferSize = kEDMA_TransferSize32Bytes;
            break;
        default:
            break;
    }
    switch (type)
    {
        case kEDMA_MemoryToMemory:
            config->destOffset = destWidth;
            config->srcOffset = srcWidth;
            break;
        case kEDMA_MemoryToPeripheral:
            config->destOffset = 0U;
            config->srcOffset = srcWidth;
            break;
        case kEDMA_PeripheralToMemory:
            config->destOffset = destWidth;
            config->srcOffset = 0U;
            break;
        default:
            break;
    }
}

int EDMA_SubmitTransfer(edma_handle_t *handle, const edma_transfer_config_t *config)
{

    edma_tcd_t *tcdRegs = (edma_tcd_t *)&handle->base->TCD[handle->channel];

    if (handle->tcdPool == NULL)
    {
        /*
            Check if EDMA is busy: if the given channel started transfer, CSR will be not zero. Because
            if it is the last transfer, DREQ will be set. If not, ESG will be set. So in order to suit
            this check mechanism, EDMA_CreatHandle will clear CSR register.
        */
        if ((tcdRegs->CSR != 0) && ((tcdRegs->CSR & DMA_CSR_DONE_MASK) == 0))
        {
            return kStatus_EDMA_Busy;
        }
        else
        {
            EDMA_SetTransferConfig(handle->base, handle->channel, config, NULL);
            /* Enable auto disable request feature */
            handle->base->TCD[handle->channel].CSR |= DMA_CSR_DREQ_MASK;
            /* Enable major interrupt */
            handle->base->TCD[handle->channel].CSR |= DMA_CSR_INTMAJOR_MASK;

            return kStatus_Success;
        }
    }
    else /* Use the TCD queue. */
    {
        uint32_t primask;
        uint32_t csr;
        int8_t currentTcd;
        int8_t previousTcd;
        int8_t nextTcd;

        /* Check if tcd pool is full. */
        primask = DisableGlobalIRQ();
        if (handle->tcdUsed >= handle->tcdSize)
        {
            EnableGlobalIRQ(primask);

            return kStatus_EDMA_QueueFull;
        }
        currentTcd = handle->tail;
        handle->tcdUsed++;
        /* Calculate index of next TCD */
        nextTcd = currentTcd + 1U;
        if (nextTcd == handle->tcdSize)
        {
            nextTcd = 0U;
        }
        /* Advance queue tail index */
        handle->tail = nextTcd;
        EnableGlobalIRQ(primask);
        /* Calculate index of previous TCD */
        previousTcd = currentTcd ? currentTcd - 1U : handle->tcdSize - 1U;
        /* Configure current TCD block. */
        EDMA_TcdReset(&handle->tcdPool[currentTcd]);
        EDMA_TcdSetTransferConfig(&handle->tcdPool[currentTcd], config, NULL);
        /* Enable major interrupt */
        handle->tcdPool[currentTcd].CSR |= DMA_CSR_INTMAJOR_MASK;
        /* Link current TCD with next TCD for identification of current TCD */
        handle->tcdPool[currentTcd].DLAST_SGA = (uint32_t)&handle->tcdPool[nextTcd];
        /* Chain from previous descriptor unless tcd pool size is 1(this descriptor is its own predecessor). */
        if (currentTcd != previousTcd)
        {
            /* Enable scatter/gather feature in the previous TCD block. */
            csr = (handle->tcdPool[previousTcd].CSR | DMA_CSR_ESG_MASK) & ~DMA_CSR_DREQ_MASK;
            handle->tcdPool[previousTcd].CSR = csr;
            /*
                Check if the TCD blcok in the registers is the previous one (points to current TCD block). It
                is used to check if the previous TCD linked has been loaded in TCD register. If so, it need to
                link the TCD register in case link the current TCD with the dead chain when TCD loading occurs
                before link the previous TCD block.
            */
            if (tcdRegs->DLAST_SGA == (uint32_t)&handle->tcdPool[currentTcd])
            {
                /* Enable scatter/gather also in the TCD registers. */
                csr = (tcdRegs->CSR | DMA_CSR_ESG_MASK) & ~DMA_CSR_DREQ_MASK;
                /* Must write the CSR register one-time, because the transfer maybe finished anytime. */
                tcdRegs->CSR = csr;
                /*
                    It is very important to check the ESG bit!
                    Because this hardware design: if DONE bit is set, the ESG bit can not be set. So it can
                    be used to check if the dynamic TCD link operation is successful. If ESG bit is not set
                    and the DLAST_SGA is not the next TCD address(it means the dynamic TCD link succeed and
                    the current TCD block has been loaded into TCD registers), it means transfer finished
                    and TCD link operation fail, so must install TCD content into TCD registers and enable
                    transfer again. And if ESG is set, it means transfer has notfinished, so TCD dynamic
                    link succeed.
                */
                if (tcdRegs->CSR & DMA_CSR_ESG_MASK)
                {
                    return kStatus_Success;
                }
                /*
                    Check whether the current TCD block is already loaded in the TCD registers. It is another
                    condition when ESG bit is not set: it means the dynamic TCD link succeed and the current
                    TCD block has been loaded into TCD registers.
                */
                if (tcdRegs->DLAST_SGA == (uint32_t)&handle->tcdPool[nextTcd])
                {
                    return kStatus_Success;
                }
                /*
                    If go to this, means the previous transfer finished, and the DONE bit is set.
                    So shall configure TCD registers.
                */
            }
            else if (tcdRegs->DLAST_SGA != 0)
            {
                /* The current TCD block has been linked successfully. */
                return kStatus_Success;
            }
            else
            {
                /*
                    DLAST_SGA is 0 and it means the first submit transfer, so shall configure
                    TCD registers.
                */
            }
        }
        /* There is no live chain, TCD block need to be installed in TCD registers. */
        EDMA_InstallTCD(handle->base, handle->channel, &handle->tcdPool[currentTcd]);
        /* Enable channel request again. */
        if (handle->flags & EDMA_TRANSFER_ENABLED_MASK)
        {
            handle->base->SERQ = DMA_SERQ_SERQ(handle->channel);
        }

        return kStatus_Success;
    }
}

void EDMA_StartTransfer(edma_handle_t *handle)
{

    if (handle->tcdPool == NULL)
    {
        handle->base->SERQ = DMA_SERQ_SERQ(handle->channel);
    }
    else /* Use the TCD queue. */
    {
        uint32_t primask;
        edma_tcd_t *tcdRegs = (edma_tcd_t *)&handle->base->TCD[handle->channel];

        handle->flags |= EDMA_TRANSFER_ENABLED_MASK;

        /* Check if there was at least one descriptor submitted since reset (TCD in registers is valid) */
        if (tcdRegs->DLAST_SGA != 0U)
        {
            primask = DisableGlobalIRQ();
            /* Check if channel request is actually disable. */
            if ((handle->base->ERQ & (1U << handle->channel)) == 0U)
            {
                /* Check if transfer is paused. */
                if ((!(tcdRegs->CSR & DMA_CSR_DONE_MASK)) || (tcdRegs->CSR & DMA_CSR_ESG_MASK))
                {
                    /*
                        Re-enable channel request must be as soon as possible, so must put it into
                        critical section to avoid task switching or interrupt service routine.
                    */
                    handle->base->SERQ = DMA_SERQ_SERQ(handle->channel);
                }
            }
            EnableGlobalIRQ(primask);
        }
    }
}

void EDMA_StopTransfer(edma_handle_t *handle)
{

    handle->flags &= (~EDMA_TRANSFER_ENABLED_MASK);
    handle->base->CERQ = DMA_CERQ_CERQ(handle->channel);
}

void EDMA_AbortTransfer(edma_handle_t *handle)
{
    handle->base->CERQ = DMA_CERQ_CERQ(handle->channel);
    /*
        Clear CSR to release channel. Because if the given channel started transfer,
        CSR will be not zero. Because if it is the last transfer, DREQ will be set.
        If not, ESG will be set.
    */
    handle->base->TCD[handle->channel].CSR = 0;
    /* Cancel all next TCD transfer. */
    handle->base->TCD[handle->channel].DLAST_SGA = 0;
}

void EDMA_HandleIRQ(edma_handle_t *handle)
{

    /* Clear EDMA interrupt flag */
    handle->base->CINT = handle->channel;
    if ((handle->tcdPool == NULL) && (handle->callback != NULL))
    {
        (handle->callback)(handle, handle->userData, true, 0);
    }
    else /* Use the TCD queue. Please refer to the API descriptions in the eDMA header file for detailed information. */
    {
        uint32_t sga = handle->base->TCD[handle->channel].DLAST_SGA;
        uint32_t sga_index;
        int32_t tcds_done;
        uint8_t new_header;
        uint8_t transfer_done;

        /* Check if transfer is already finished. */
        transfer_done = ((handle->base->TCD[handle->channel].CSR & DMA_CSR_DONE_MASK) != 0);
        /* Get the offset of the next transfer TCD blcoks to be loaded into the eDMA engine. */
        sga -= (uint32_t)handle->tcdPool;
        /* Get the index of the next transfer TCD blcoks to be loaded into the eDMA engine. */
        sga_index = sga / sizeof(edma_tcd_t);
        /* Adjust header positions. */
        if (transfer_done)
        {
            /* New header shall point to the next TCD to be loaded (current one is already finished) */
            new_header = sga_index;
        }
        else
        {
            /* New header shall point to this descriptor currently loaded (not finished yet) */
            new_header = sga_index ? sga_index - 1U : handle->tcdSize - 1U;
        }
        /* Calculate the number of finished TCDs */
        if (new_header == handle->header)
        {
            if (handle->tcdUsed == handle->tcdSize)
            {
                tcds_done = handle->tcdUsed;
            }
            else
            {
                /* No TCD in the memory are going to be loaded or internal error occurs. */
                tcds_done = 0;
            }
        }
        else
        {
            tcds_done = new_header - handle->header;
            if (tcds_done < 0)
            {
                tcds_done += handle->tcdSize;
            }
        }
        /* Advance header which points to the TCD to be loaded into the eDMA engine from memory. */
        handle->header = new_header;
        /* Release TCD blocks. tcdUsed is the TCD number which can be used/loaded in the memory pool. */
        handle->tcdUsed -= tcds_done;
        /* Invoke callback function. */
        if (handle->callback)
        {
            (handle->callback)(handle, handle->userData, transfer_done, tcds_done);
        }
    }
}

void DMA10_DMA26_IRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DMA0, 10U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[10]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 26U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[26]);
    }
}

void DMA11_DMA27_IRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DMA0, 11U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[11]);
    }
    if ((EDMA_GetChannelStatusFlags(DMA0, 27U) & kEDMA_InterruptFlag) != 0U)
    {
        EDMA_HandleIRQ(s_EDMAHandle[27]);
    }
}
