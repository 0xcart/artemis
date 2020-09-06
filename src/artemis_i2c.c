///
/// @file artemis_i2c.c
///

#include "artemis_i2c.h"

///
///
///
bool artemis_i2c_send(artemis_i2c_t *i2c)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32I2CDevAddr = i2c->address;
    transfer.bContinue = !i2c->stop;
	transfer.pui32TxBuffer = (uint32_t *)i2c->iom.txstream.buffer;
    transfer.ui32NumBytes = i2c->iom.txstream.written;
    transfer.eDirection = AM_HAL_IOM_TX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(i2c->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes read from the txstream
    i2c->iom.txstream.read = i2c->iom.txstream.written;

    return(true);
}

///
///
///
bool artemis_i2c_receive(artemis_i2c_t *i2c)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32I2CDevAddr = i2c->address;
    transfer.bContinue = !i2c->stop;
	transfer.pui32RxBuffer = (uint32_t *)i2c->iom.rxstream.buffer;
    transfer.ui32NumBytes = i2c->iom.rxnumber;
    transfer.eDirection = AM_HAL_IOM_RX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(i2c->iom.handle, &transfer)) {
        return(false);
    }

    // update the number of bytes written to the rxstream
    i2c->iom.rxstream.written = i2c->iom.rxnumber;

    return(true);
}
