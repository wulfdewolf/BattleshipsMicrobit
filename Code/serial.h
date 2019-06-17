#ifndef SERIAL_H
#define SERIAL_H

#include "global.h"

extern MicroBit uBit;


// ESTABLISHING CONNECTION

void make_connection_s();

bool choose_beginner_s();


// DATA EXCHANGE

void clear_buffers_s();

uint8_t ask_if_hit_s(Position *cursor);

uint8_t answer_if_hit_s(Field *f);

void ask_dimensions_s(uint8_t *width_address, uint8_t *height_address);

void send_dimensions_s(uint8_t width, uint8_t height);

#endif
