

#ifndef INC_CIRCULARBUFFER_H_
#define INC_CIRCULARBUFFER_H_

#include <stdint.h>

typedef struct
{
    volatile uint8_t *mBuf_;
    uint32_t mSize_;
    volatile uint32_t mRead_;
    volatile uint32_t mWrite_;
} circularBuffer_t;

void     circularBuffer_constructor ( circularBuffer_t*, uint8_t*, uint32_t );
uint16_t circularBuffer_length_data ( circularBuffer_t* );
uint8_t  circularBuffer_write       ( circularBuffer_t*, uint8_t );
uint8_t  circularBuffer_read        ( circularBuffer_t*, uint8_t* );
void     circularBuffer_flush       ( circularBuffer_t * );

#endif	/* BUFFER_CIRCULAR_H */