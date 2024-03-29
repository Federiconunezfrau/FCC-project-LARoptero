#ifdef __cplusplus
extern "C" {
#endif

#include "circularBuffer.h"

void circularBuffer_constructor(circularBuffer_t *me, uint8_t *buf, uint32_t size)
{
    me->mBuf_ = buf;
    me->mSize_ = size;
    me->mWrite_ = 0;
    me->mRead_ = 0;
}

/**
 * @Brief Toma como parametro el buffer cicular y devuelve el tamanio util para escribir
 * @param Buffer
 * @return 
 */
uint16_t circularBuffer_length_data(circularBuffer_t* me)
{
	return ((me->mWrite_ - me->mRead_) & (me->mSize_ - 1));
}

/**
 * @Brief Escribe en el buffer "Buffer" el dato "data"
 * @param Buffer
 * @param data
 * @return 
 */
uint8_t circularBuffer_write(circularBuffer_t* me, uint8_t data)
{
    if( circularBuffer_length_data(me) == (me->mSize_ - 1) )
    {
        return 1;
    }
    
    me->mBuf_[me->mWrite_] = data;
    me->mWrite_ = (me->mWrite_ +1) & (me->mSize_ -1);

    return 0;
}

/**
 * @Brief Lee del buffer "Buffer" el dato "data"
 * @param Buffer
 * @param data
 * @return 
 */
uint8_t circularBuffer_read(circularBuffer_t* me, uint8_t *data)
{
    if(circularBuffer_length_data(me) == 0 )
    {
        return 1;
    }
    *data = me->mBuf_[me->mRead_];
    me->mRead_ = (me->mRead_ + 1) & (me->mSize_ -1);
    return 0;
}

void circularBuffer_flush(circularBuffer_t *me)
{
	me->mRead_ = me->mWrite_;
}

#ifdef __cplusplus
}
#endif
