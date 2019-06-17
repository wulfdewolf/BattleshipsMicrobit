#ifndef GLOBAL_H
#define GLOBAL_H

#include "MicroBit.h"

// MACROS
#define VERT 0
#define HORIZ 1

#define MISS 3
#define HIT 2
#define UNHIT 1
#define WATER 0

#define BOATSHIDDEN true
#define BOATSSHOWING false

#define AMOUNT_OF_BOATS 5
#define TOTAL_BOAT_SEGMENTS 14
#define SCRLENGTH 5

#define field_val_on_pos(POS, F) (*((F)->field + ((F)->width)*(POS->y) + (POS->x)))

#define field_val(X, Y, F) (*((F)->field + ((F)->width)*(Y) + (X)))

#define round_down_to(D, X) ((X) -((X)%(D)))

// boat orientation can be 0 or 1 so, it will only be added to the correct coordinate
#define boat_segment_x(B, I) ((B)->x + ((B)->orientation * (I)))

#define boat_segment_y(B, I) ((B)->y + (!(B)->orientation * (I)))

#define allowed_boat_x(B, F) ((F)->width - ((B)->orientation * (B)->size))

#define allowed_boat_y(B, F) ((F)->height - (!(B)->orientation * (B)->size))

// STRUCTS
struct Position {
  int x = 0;
  int y = 0;
};

struct Field {
  uint8_t *field;
  uint8_t width;
  uint8_t height;
};

#endif
