#include "field.h"

/////////////////////////////////////////////////////////////////////////
////////////////////////           FIELDS            ////////////////////
/////////////////////////////////////////////////////////////////////////

// Creates a field with given size, stores the size and returns a pointer to the field
Field *create_field(uint8_t width, uint8_t height){
  Field *f = (Field*)malloc(sizeof(Field));
  f->field = (uint8_t*)calloc(width*height,sizeof(uint8_t));
  f->width = width;
  f->height = height;
  return f;
}

// Frees the memory used by a Field struct
void free_field(Field *f){
  free(f->field);
  free(f);
}

// Fill field with zeros
void clear_field(Field *f){
  for(int i = 0; i < f->width; i++){
    for(int j = 0; j < f->height; j++){

      // edits the value of each cel to be water
      field_val(i, j, f) = WATER;
    }
  }
}

// Merges a list of boats and a field
void merge_field(Boat **boats, Field *f, int amount){

  // get a clear field
  clear_field(f);
  // put each boat on the field in form of 1's
  for(int i=0; i < amount; i++){
    boat_to_field(boats[i], f);
  }
}
