///
/// @file artemis_spi.c
///

#include "artemis_spi.h"

///
///
///
bool artemis_spi_send(artemis_spi_t *spi, artemis_stream_t *txstream)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32TxBuffer = (uint32_t *)txstream->buffer;
    transfer.ui32NumBytes = txstream->written;
    transfer.eDirection = AM_HAL_IOM_TX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(spi->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes read from the txstream
    txstream->read = txstream->written;

    return(true);
}

///
///
///
bool artemis_spi_receive(artemis_spi_t *spi, artemis_stream_t *rxstream, uint32_t rxnumber)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32RxBuffer = (uint32_t *)rxstream->buffer;
    transfer.ui32NumBytes = rxnumber;
    transfer.eDirection = AM_HAL_IOM_RX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(spi->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes written to the rxstream
    rxstream->written = rxnumber;

    return(true);
}

///
///
///
bool artemis_spi_transfer(artemis_spi_t *spi, artemis_stream_t *txstream, artemis_stream_t *rxstream)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32TxBuffer = (uint32_t *)txstream->buffer;
	transfer.pui32RxBuffer = (uint32_t *)rxstream->buffer;
    transfer.ui32NumBytes = txstream->written;
    transfer.eDirection = AM_HAL_IOM_FULLDUPLEX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_spi_blocking_fullduplex(spi->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes read from the txstream
    txstream->read = txstream->written;

    // update the number of bytes written to the rxstream
    rxstream->written = txstream->written;

    return(true);
}
