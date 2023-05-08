/*
 * ringBuffer.h
 *
 *  Created on: May 1, 2023
 *      Author: 85tar
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#define RING_BUFFER_SIZE 8

class RingBuffer
{
public:
	RingBuffer()
	{
		static uint8_t buffer[RING_BUFFER_SIZE] {0};
		bufferStart = buffer;
		bufferEnd = buffer + RING_BUFFER_SIZE;
		head = buffer;
		tail = buffer;
	}

	bool put(const uint8_t& value)
	{
		// если буфер полон возвращает true и ничего не кладет в буфер
		if (isBufferFull) return true;
		else
		{
			*tail = value;
			++tail;
			if (tail == bufferEnd) tail = bufferStart;
			// если буфер "закольцевался" устанавливаем флаг
			if (tail == head) isBufferFull = true;
			return false;
		}
	}

	bool get(uint8_t* value)
	{
		if(isEmpty()) return true;
		else
		{
			*value = *head;
			++head;
			if (head == bufferEnd) head = bufferStart;
			isBufferFull = false;
			return false;
		}
	}

	// если буфер пуст возвращает true
	bool isEmpty()
	{
		if(isBufferFull) return false;
		if (head == tail) return true;
		else return false;
	}

	// если буфер полон возвращает true
	bool isFull()
	{
		return isBufferFull;
	}

private:
	// указатель на начало буфера
	uint8_t* bufferStart = nullptr;

	// указатель на конец буфера
	uint8_t* bufferEnd = nullptr;

	// указатель на начало данных
	uint8_t* head = nullptr;

	// указатель на конец данных
	uint8_t* tail = nullptr;

	// "флаг" о переполнении буфера
	bool isBufferFull = false;
};


#endif /* INC_RINGBUFFER_H_ */
