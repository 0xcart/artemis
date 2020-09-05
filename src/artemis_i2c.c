///
/// @file artemis_i2c.c
///

#include "artemis_i2c.h"

///
///
///
bool artemis_i2c_send(artemis_i2c_t *i2c, artemis_iom_t *iom)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32I2CDevAddr = i2c->address;
    transfer.bContinue = !i2c->stop;
	transfer.pui32TxBuffer = (uint32_t *)artemis_stream_getbuffer(i2c->txstream);
    transfer.ui32NumBytes = artemis_stream_written(i2c->txstream);
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
bool artemis_i2c_request(artemis_i2c_t *i2c, artemis_iom_t *iom)
{
    am_hal_iom_transfer_t transfer = {0};

    transfer.uPeerInfo.ui32I2CDevAddr = i2c->address;
    transfer.bContinue = !i2c->stop;
	transfer.pui32RxBuffer = (uint32_t *)artemis_stream_getbuffer(i2c->rxstream);
    transfer.ui32NumBytes = artemis_stream_length(i2c->rxstream);
    transfer.eDirection = AM_HAL_IOM_RX;
	transfer.ui8Priority = 1;

    if (AM_HAL_STATUS_SUCCESS != am_hal_iom_blocking_transfer(iom->handle, &transfer)) {
        return(false);
    }

    return(true);
}
