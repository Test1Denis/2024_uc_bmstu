/*
 * ring_buffer.c
 *
 *  Created on: May 6, 2024
 *      Author: protuberanec
 */
#include "ring_buffer.h"


void ring_init(struct RingBuffer* b, int size, char* buf) {
	b->size = size;
	b->buf = buf;
	b->datalen = 0;
	b->start = 0;
	b->end = b->start;
}

void ring_flush(struct RingBuffer* b) {
	b->start = b->end;
	b->datalen = 0;
}
int ring_PutToEnd(struct RingBuffer* b, char el) {
	if (b->datalen > b->size - 1) {
		return -1;
	}
	b->buf[b->end++] = el;
	if (b->end == b->size) {
		b->end = 0;
	}
	b->datalen++;

	return 0;
}

int ring_GetFromFront(struct RingBuffer* b, char* el) {
	if (!b->datalen) {
		return -1;
	}
	*el = b->buf[b->start++];
	if (b->start == b->size) {
		b->start = 0;
	}
	b->datalen--;
}


