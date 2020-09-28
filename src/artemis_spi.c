///
/// @file artemis_spi.c
///

#include "artemis_debug.h"
#include "artemis_spi.h"

///
///
///
void artemis_spi_send(artemis_spi_t *spi, bool stop, artemis_stream_t *txstream)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32SpiChipSelect = spi->chipselect;
    transfer.bContinue = !stop;
    transfer.pui32TxBuffer = (uint32_t *)txstream->buffer;
    transfer.ui32NumBytes = txstream->written;
    transfer.eDirection = AM_HAL_IOM_TX;
    transfer.ui8Priority = 1;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_blocking_transfer(spi->iom.handle, &transfer));

    // update the number of bytes read from the txstream
    txstream->read = txstream->written;
}

///
///
///
void artemis_spi_receive(artemis_spi_t *spi, bool stop, artemis_stream_t *rxstream, uint32_t rxnumber)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32SpiChipSelect = spi->chipselect;
    transfer.bContinue = !stop;
    transfer.pui32RxBuffer = (uint32_t *)rxstream->buffer;
    transfer.ui32NumBytes = rxnumber;
    transfer.eDirection = AM_HAL_IOM_RX;
    transfer.ui8Priority = 1;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_blocking_transfer(spi->iom.handle, &transfer));

    // update the number of bytes written to the rxstream
    rxstream->written = rxnumber;
}

///
///
///
void artemis_spi_transfer(artemis_spi_t *spi, bool stop, artemis_stream_t *txstream, artemis_stream_t *rxstream)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32SpiChipSelect = spi->chipselect;
    transfer.bContinue = !stop;
    transfer.pui32TxBuffer = (uint32_t *)txstream->buffer;
    transfer.pui32RxBuffer = (uint32_t *)rxstream->buffer;
    transfer.ui32NumBytes = txstream->written;
    transfer.eDirection = AM_HAL_IOM_FULLDUPLEX;
    transfer.ui8Priority = 1;

    ARTEMIS_DEBUG_HALSTATUS(am_hal_iom_spi_blocking_fullduplex(spi->iom.handle, &transfer));

    // update the number of bytes read from the txstream
    txstream->read = txstream->written;

    // update the number of bytes written to the rxstream
    rxstream->written = txstream->written;
}
