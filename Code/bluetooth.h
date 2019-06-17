#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "global.h"

extern MicroBit uBit;


// ESTABLISHING CONNECTION
void make_connection_b();

void select_beginner_b(MicroBitEvent e);

void read_beginner_b();

bool choose_beginner_b();

// DATA EXCHANGE
void clear_buffers_b();

void receive_data(MicroBitEvent e);

uint8_t ask_if_hit_b(Position *cursor);

uint8_t answer_if_hit_b(Field *f);

void ask_dimensions_b(uint8_t *width_address, uint8_t *height_address);

void send_dimensions_b(uint8_t width, uint8_t height);


#endif
