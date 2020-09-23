///
/// @file artemis_i2c.c
///

#include "artemis_i2c.h"

///
///
///
bool artemis_i2c_send(artemis_i2c_t *i2c, bool stop, artemis_stream_t *txstream)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32I2CDevAddr = i2c->address;
    transfer.bContinue = !stop;
    transfer.pui32TxBuffer = (uint32_t *)txstream->buffer;
    transfer.ui32NumBytes = txstream->written;
    transfer.eDirection = AM_HAL_IOM_TX;
    transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(i2c->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes read from the txstream
    txstream->read = txstream->written;

    return(true);
}

///
///
///
bool artemis_i2c_receive(artemis_i2c_t *i2c, bool stop, artemis_stream_t *rxstream, uint32_t rxnumber)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32I2CDevAddr = i2c->address;
    transfer.bContinue = !stop;
    transfer.pui32RxBuffer = (uint32_t *)rxstream->buffer;
    transfer.ui32NumBytes = rxnumber;
    transfer.eDirection = AM_HAL_IOM_RX;
    transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(i2c->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes written to the rxstream
    rxstream->written = rxnumber;

    return(true);
}
