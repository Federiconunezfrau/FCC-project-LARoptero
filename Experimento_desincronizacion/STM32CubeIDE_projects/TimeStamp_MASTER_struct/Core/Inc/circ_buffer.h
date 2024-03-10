/**
 * @file dataStructures.h
 * @author Franco Nastasi (fnastasi@fi.uba.ar)
 * @brief En este archivo se implementan estructuras como buffers circulares y otros
 * @version 0.1
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INC_CIRC_BUFFER_H_
#define INC_CIRC_BUFFER_H_

#include "stdint.h"

constexpr bool EXIT_OK = true;
constexpr bool EXIT_ERROR = false;

template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType = uint16_t >
class circ_buffer
{
	public:

		//bool bufferOverloaded;

		circ_buffer();
		bool write(bufferDataType data);
		bool read(bufferDataType* pData);

		sizeDataType copy_to(bufferDataType* pMemDest, sizeDataType size);
		sizeDataType copy_from(bufferDataType* pMemSrc, sizeDataType size);



		bool is_empty(void);
		bool is_full(void);
		uint32_t get_size(void);


		// void buffer_cpy(void);
		// uint32_t get_remaining(void);
		// uint32_t get_distance_to_end_of_buffer(void);
		// bufferDataType* get_head_ptr(void);
		// bufferDataType* get_tail_ptr(void);

	private:
		static const sizeDataType sCapacity_ = bufferSize;
		sizeDataType mSize_;
		bufferDataType mBuffer_[bufferSize];
		sizeDataType mHead_;
		sizeDataType mTail_;
		bool mFull_;
		bool mEmpty_;

};

template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
circ_buffer<bufferDataType, bufferSize, sizeDataType>::circ_buffer()
{
	mFull_ = false;
	mEmpty_ = true;
	mHead_ = 0;
	mTail_ = 0;
	mSize_ = 0;

}

template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
bool circ_buffer<bufferDataType, bufferSize, sizeDataType>::write(bufferDataType data)
{
	//if (mFull_){
	//    bufferOverloaded = true;
	//    return EXIT_ERROR;
	//}

	mBuffer_[mHead_] = data;
	mEmpty_ = false;


	if(++mHead_ == sCapacity_){
		mHead_ = 0;
	}

	// In case buffer was full, tail object must be updated so that old data is overwritten, ow check is buffer is full
	if(mFull_ == true){
		mTail_ = mTail_ + 1 == sCapacity_ ? 0 : mTail_ + 1; // mTail_ = ++mTail_ == sCapacity_ ? 0 : mTail_;
	}else if(++mSize_ == sCapacity_){
		mFull_ = true;
	}
	return EXIT_OK;
}


template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
bool circ_buffer<bufferDataType, bufferSize, sizeDataType>::read(bufferDataType* pData)
{
	// bufferDataType data = 0;

	if (mEmpty_){
		return EXIT_ERROR;
		// return data;
	}

	*pData = mBuffer_[mTail_];
	mFull_ = false;

	if(--mSize_ == 0){
		mEmpty_ = true;
	}

	if(++mTail_ == sCapacity_){
		mTail_ = 0;
	}

	return EXIT_OK;
}


template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
uint32_t circ_buffer<bufferDataType, bufferSize, sizeDataType>::get_size()
{
	return mSize_;
}


// template<typename bufferDataType, uint32_t N, typename sizeDataType >
// uint32_t circ_buffer<bufferDataType, N, sizeDataType>::get_remaining(){
//     return (sCapacity_ - mSize_);
// }

// template<typename bufferDataType, uint32_t N, typename sizeDataType >
// uint32_t circ_buffer<bufferDataType, N, sizeDataType>::get_distance_to_end_of_buffer(){
//     return (sCapacity_ - mHead_);
// }

// template<typename bufferDataType, uint32_t N, typename sizeDataType >
// bufferDataType* circ_buffer<bufferDataType, N, sizeDataType>::get_head_ptr(){
//     return &mBuffer_[mHead_];
// }

// template<typename bufferDataType, uint32_t N, typename sizeDataType >
// bufferDataType* circ_buffer<bufferDataType, N, sizeDataType>::get_tail_ptr(){
//     return &mBuffer_[mTail_];
// }

template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
bool circ_buffer<bufferDataType, bufferSize, sizeDataType>::is_empty(){
	return mEmpty_;
}

template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
bool circ_buffer<bufferDataType, bufferSize, sizeDataType>::is_full(){
	return mFull_;
}


template<typename bufferDataType, uint32_t bufferSize, typename sizeDataType >
sizeDataType circ_buffer<bufferDataType, bufferSize, sizeDataType>::copy_to(bufferDataType* pMemDest, sizeDataType size){
	sizeDataType res = 0;
	while(res < size)
	{
		if (!read(&pMemDest[res]))
		{
			return res;
		}
		res++;
	}

	return res;
	// for (uint32_t i; i < size; i++){
	//     pMemDest[i] = read();
	// }
}

template<typename bufferDataType, uint32_t Size, typename sizeDataType >
sizeDataType circ_buffer<bufferDataType, Size, sizeDataType>::copy_from(bufferDataType* pMemSrc, sizeDataType size){
	sizeDataType res = 0;

	while(res < size)
	{
		if (!write(pMemSrc[res]))
		{
			return res;
		}
		res++;
	}

	return res;

	// for (uint32_t i; i < size; i++){
	//     write(pMemSrc[i]);
	// }
}
#endif
