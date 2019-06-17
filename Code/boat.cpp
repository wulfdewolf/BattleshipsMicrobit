#include "boat.h"

/////////////////////////////////////////////////////////////////////////
////////////////////              BOATS                //////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////CREATION////////////////////////////////

// Creates a new boat of given size
Boat *make_boat(int size){
  Boat *b = (Boat*)calloc(1,sizeof(Boat));
  b->size = size;
  return b;
}

// Frees all boats inside a list of boats
void free_boats(Boat **boats){
  for(int i = 0; i < AMOUNT_OF_BOATS; i++){
    free(boats[i]);
  }
}


/////////////////////////////////MOVEMENT////////////////////////////////

// Moves a boat along x-axis
void boat_move_x(Boat *b, uint8_t field_width){
  if(boat_furthest_point(b, X) < field_width-1) b->x += 1;
  else b->x = 0;
}

// Moves a boat along the y-axis
void boat_move_y(Boat *b, uint8_t field_height){
  if(boat_furthest_point(b, Y) < field_height-1) b->y += 1;
  else b->y = 0;
}

// Rotates a boat
void rotate_boat(Boat *b){
  if(b->orientation == VERT) b->orientation = HORIZ;
  else b->orientation = VERT;
}


////////////////////////////FIELD INTERACTION////////////////////////////

// Gets a boat's furthest point depending on it's orientation
// f defines if we want furthest x or y point
int boat_furthest_point(Boat *b, int f){
  if(f) return boat_segment_y(b, b->size - 1);
  else return boat_segment_x(b, b->size - 1);
}

// Verifies if a boat can be stationed on it's current location
bool place_free(Boat*b, Field *f){

  // for each boat segment
  for(int i = 0; i < b->size; i++){
    if(field_val(boat_segment_x(b, i), boat_segment_y(b, i), f)) return false;
  }
  return true;
}

// Puts a boat on a field but putting 1's on it's segments
// !!ASSUMES THE PLACE IS FREE!!
void boat_to_field(Boat *b, Field *f){

  // for each boat segment
  for(int i = 0; i < b->size; i++){
    field_val(boat_segment_x(b, i), boat_segment_y(b, i), f) = UNHIT;
  }
}
