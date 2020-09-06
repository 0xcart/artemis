///
/// @file artemis_spi.c
///

#include "artemis_spi.h"

///
///
///
bool artemis_spi_send(artemis_spi_t *spi)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32TxBuffer = (uint32_t *)spi->iom.txstream.buffer;
    transfer.ui32NumBytes = spi->iom.txstream.written;
    transfer.eDirection = AM_HAL_IOM_TX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(spi->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes read from the txstream
    spi->iom.txstream.read = spi->iom.txstream.written;

    return(true);
}

///
///
///
bool artemis_spi_receive(artemis_spi_t *spi)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32RxBuffer = (uint32_t *)spi->iom.rxstream.buffer;
    transfer.ui32NumBytes = spi->iom.rxnumber;
    transfer.eDirection = AM_HAL_IOM_RX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(spi->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes written to the rxstream
    spi->iom.rxstream.written = spi->iom.rxnumber;

    return(true);
}

///
///
///
bool artemis_spi_transfer(artemis_spi_t *spi)
{
    am_hal_iom_transfer_t transfer = {0};

	transfer.uPeerInfo.ui32SpiChipSelect = 0;
    transfer.bContinue = false;
	transfer.pui32TxBuffer = (uint32_t *)spi->iom.txstream.buffer;
	transfer.pui32RxBuffer = (uint32_t *)spi->iom.rxstream.buffer;
    transfer.ui32NumBytes = spi->iom.txstream.written;
    transfer.eDirection = AM_HAL_IOM_FULLDUPLEX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_spi_blocking_fullduplex(spi->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes read from the txstream
    spi->iom.txstream.read = spi->iom.txstream.written;

    // update the number of bytes written to the rxstream assuming the
    // number of bytes received equals the number of bytes sent based
    // on the implementation of the above hal fullduplex function
    spi->iom.rxstream.written = spi->iom.txstream.written; 

    return(true);
}
