///
/// @file artemis_spi.c
///

#include "artemis_spi.h"

///
///
///
bool artemis_spi_send(artemis_spi_t *spi, artemis_iom_t *iom)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32TxBuffer = (uint32_t *)artemis_stream_getbuffer(spi->txstream);
    transfer.ui32NumBytes = artemis_stream_written(spi->txstream);
    transfer.eDirection = AM_HAL_IOM_TX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(iom->handle, &transfer)) {
        return(false);
    }

    return(true);
}

///
///
///
bool artemis_spi_receive(artemis_spi_t *spi, artemis_iom_t *iom)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32RxBuffer = (uint32_t *)artemis_stream_getbuffer(spi->rxstream);
    transfer.ui32NumBytes = artemis_stream_length(spi->rxstream);
    transfer.eDirection = AM_HAL_IOM_RX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(iom->handle, &transfer)) {
        return(false);
    }

    return(true);
}

///
///
///
bool artemis_spi_transfer(artemis_spi_t *spi, artemis_iom_t *iom)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32TxBuffer = (uint32_t *)artemis_stream_getbuffer(spi->txstream);
	transfer.pui32RxBuffer = (uint32_t *)artemis_stream_getbuffer(spi->rxstream);
    transfer.ui32NumBytes = artemis_stream_written(spi->txstream);
    transfer.eDirection = AM_HAL_IOM_FULLDUPLEX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_spi_blocking_fullduplex(iom->handle, &transfer)) {
        return(false);
    }

    return(true);
}
