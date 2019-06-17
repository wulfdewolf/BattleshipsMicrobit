#ifndef INPUT_H
#define INPUT_H

#include "global.h"
#include "boat.h"

extern MicroBit uBit;

// CURSOR
void cursor_move_x(uint8_t field_width, Position *cursor, Position *view);

void cursor_move_y(uint8_t field_height, Position *cursor, Position *view);

bool move_cursor(Field *f, Position *cursor, Position *view);

void player_place_cursor(Field* f, Position *cursor, Position *view);

void ai_random_cursor(Field *f);


// VALUES
bool update_val(int *val, int min, int max, int difference);

void get_val(uint8_t *val_address, const char *msg, uint8_t min, uint8_t max, uint8_t difference);


// BOATS
bool get_rotation();

bool move_boat(Boat* b, Field *f, Position *view);

void player_place_boats(Field *f, Position *view);

void ai_place_boats(Field *f);

#endif
