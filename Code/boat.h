#ifndef BOAT_H
#define BOAT_H

#include "global.h"

#define X 0
#define Y 1

// STRUCT
struct Boat {
  int size;
  int orientation = VERT;
  int x = 0;
  int y = 0;
};


// CREATION
Boat *make_boat(int size);
void free_boats(Boat **boats);

// MOVEMENT
void boat_move_x(Boat *b, uint8_t field_size);

void boat_move_y(Boat *b, uint8_t field_size);

void rotate_boat(Boat *b);

// FIELD INTERACTION
int boat_furthest_point(Boat *b, int f);

bool place_free(Boat*b, Field *f);

void boat_to_field(Boat *b, Field *f);

#endif
