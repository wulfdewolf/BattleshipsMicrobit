#ifndef DISPLAY_H
#define DISPLAY_H

#include "global.h"
#include "boat.h"

extern MicroBit uBit;

void init_display();

// PRINTING
void print_text(const char* msg);

void print_number(uint8_t number);

void scroll_text(const char* msg);

void clear_display();

void draw_field(Field *f, Position *view, bool hidden);

// FLICKERING

bool in_view(int x, int y, Position *view);

void flickering_cursor(Position *cursor);

void flickering_boat(Boat *b, Position *view);

// ANIMATIONS

void display_wait_animation();

void display_stop();



#endif
