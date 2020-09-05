///
/// @file artemis_stream.h
///

#ifndef ARTEMIS_STREAM_H
#define ARTEMIS_STREAM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_artemis_stream_t
{
    uint8_t *buffer;
    size_t length;
    size_t written;
    size_t read;
} artemis_stream_t;

void artemis_stream_setbuffer(artemis_stream_t *stream, uint8_t *buffer, size_t length);
uint8_t *artemis_stream_getbuffer(artemis_stream_t *stream);
size_t artemis_stream_length(artemis_stream_t *stream);
size_t artemis_stream_written(artemis_stream_t *stream);
size_t artemis_stream_available(artemis_stream_t *stream);
size_t artemis_stream_put(artemis_stream_t *stream, uint8_t data);
size_t artemis_stream_write(artemis_stream_t *stream, uint8_t *data, size_t length);
size_t artemis_stream_peek(artemis_stream_t *stream, uint8_t *data);
size_t artemis_stream_get(artemis_stream_t *stream, uint8_t *data);
size_t artemis_stream_read(artemis_stream_t *stream, uint8_t *data, size_t length);
void artemis_stream_reset(artemis_stream_t *stream);

#ifdef __cplusplus
}
#endif

#endif
