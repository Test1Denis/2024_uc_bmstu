/*
 * ring_buffer.h
 *
 *  Created on: May 6, 2024
 *      Author: protuberanec
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "stm32f0xx.h"

struct RingBuffer {
	char *buf;
	int size;
	int start;
	int end;
	int dataLen;
};

void ring_init(struct RingBuffer*, int size, char* buf);

void ring_flush(struct RingBuffer*);
int ring_PutToEnd(struct RingBuffer*, char);
int ring_GetFromFront(struct RingBuffer*, char*);


#endif /* RING_BUFFER_H_ */
