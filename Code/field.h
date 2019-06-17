#ifndef FIELD_H
#define FIELD_H

#include "global.h"
#include "boat.h"

Field* create_field(uint8_t width, uint8_t height);

void free_field(Field *f);

void clear_field(Field *f);

void merge_field(Boat **boats, Field *f, int amount);

#endif
