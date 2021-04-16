///
/// @file artemis_stream.c
///

#include "artemis_stream.h"

///
/// 
/// 
void artemis_stream_setbuffer(artemis_stream_t *stream, uint8_t *buffer, size_t length)
{
    stream->buffer = buffer;
    stream->length = length;
    stream->written  = 0;
    stream->read   = 0;
}

///
///
///
uint8_t *artemis_stream_getbuffer(artemis_stream_t *stream)
{
    return(stream->buffer);
}

///
///
///
size_t artemis_stream_length(artemis_stream_t *stream)
{
    return(stream->length);
}

///
///
///
size_t artemis_stream_written(artemis_stream_t *stream)
{
    return(stream->written);
}

///
///
///
size_t artemis_stream_available(artemis_stream_t *stream)
{
    return(stream->written - stream->read);
}

///
///
///
size_t artemis_stream_put(artemis_stream_t *stream, uint8_t data)
{
    if (stream->written < stream->length) {
        stream->buffer[stream->written++] = data;
        return(1);
    }

    return(0);
}

///
///
///
size_t artemis_stream_write(artemis_stream_t *stream, uint8_t *data, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        if (!artemis_stream_put(stream, data[i])) {
            return(i);
        }
    }

    return(length);
}

///
///
///
size_t artemis_stream_peek(artemis_stream_t *stream, uint8_t *data)
{
    if (stream->read < stream->written) {
        *data = stream->buffer[stream->read];
        return(1);
    }

    return(0);
}

///
///
///
size_t artemis_stream_get(artemis_stream_t *stream, uint8_t *data)
{
    if (stream->read < stream->written) {
        *data = stream->buffer[stream->read++];
        return(1);
    }

    return(0);
}

///
///
///
size_t artemis_stream_read(artemis_stream_t *stream, uint8_t *data, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        if (!artemis_stream_get(stream, &data[i])) {
            return(i);
        }
    }

    return(length);
}

///
///
///
void artemis_stream_reset(artemis_stream_t *stream)
{
    stream->written = 0;
    stream->read = 0;
}
