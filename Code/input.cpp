#include "field.h"
#include "display.h"
#include "input.h"

/////////////////////////////////////////////////////////////////////////
//////////////////////           INPUT             //////////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////CURSOR//////////////////////////////////

// Move cursor along x-axis
void cursor_move_x(uint8_t field_width, Position *cursor, Position *view){
  cursor->x = (cursor->x+1)%field_width;
   view->x = round_down_to(SCRLENGTH, cursor->x);
}

// Move cursor along y-axis
void cursor_move_y(uint8_t field_height, Position *cursor, Position *view){
  cursor->y = (cursor->y+1)%field_height;
  view->y = round_down_to(SCRLENGTH, cursor->y);
}

// Updates cursor position on button input
// used as loop condition
bool move_cursor(Field *f, Position *cursor, Position *view){

  // on ENTER press
  if(uBit.buttonB.isPressed() && uBit.buttonA.isPressed()) {

    // return false to stop loop
    return false;

    // on A press
  } else if(uBit.buttonA.isPressed()){
    cursor_move_x(f->width, cursor, view);

    // on Bpress
  } else if(uBit.buttonB.isPressed()){
    cursor_move_y(f->height, cursor, view);
  }
  // if ENTER wasn't pressed return true to keep loop going
  return true;
}

// Lets the player place the cursor
void player_place_cursor(Field* f, Position *cursor, Position *view){

  while(move_cursor(f, cursor, view)){
    draw_field(f, view, BOATSHIDDEN);
    flickering_cursor(cursor);
  }
  clear_display();
}

// Places cursor randomly
void ai_random_cursor(Field *f){

  uint8_t x;
  uint8_t y;

  // Get random coords untill we get some that we hadn't already tried to hit
  do{
    x = uBit.random(f->width);
    y = uBit.random(f->height);
  }while(field_val(x, y, f) == HIT || field_val(x, y, f) == MISS);
}


/////////////////////////////////VALUES//////////////////////////////////

// Updates a value on button input
// used as loop condition
bool update_val(uint8_t *val, int min, int max, int difference){

    // on ENTER press
    if(uBit.buttonB.isPressed() && uBit.buttonA.isPressed()){
      // return false to stop loop
      return false;

    // on A press
    }else if(uBit.buttonA.isPressed()){
      // decrease val
      if(*val-difference  >= min) *val = *val - difference;
      // display
      print_number(*val);

    // on B press
    }else if(uBit.buttonB.isPressed()){
      // increase val
      if(*val+difference <= max) *val = *val + difference;
      // increase val
      print_number(*val);
    }
    // if ENTER wasn't pressed return true to keep loop going
    return true;
}

// Gets a value from user input
void get_val(uint8_t *val_address, const char *msg, uint8_t min, uint8_t max, uint8_t difference){
  scroll_text(msg);

  // Initialises the value with the minimum
  *val_address = min;
  print_number(*val_address);

  // Asks the player to edit the value untill satisfied
  while(update_val(val_address, min, max, difference)){
    uBit.sleep(100);
  }
}

/////////////////////////////////BOATS///////////////////////////////////

// Rotates a boat on button input
// used as loop condition
bool get_rotation(Boat *b){

  // on ENTER press
  if(uBit.buttonB.isPressed() && uBit.buttonA.isPressed()){
    // return 0 to stop loop
    return false;

  // on A or B press
  } else if(uBit.buttonB.isPressed() || uBit.buttonA.isPressed()){
    rotate_boat(b);
  }
  // if ENTER wasn't pressed return true to keep loop going
  return true;
}

// Moves a boat on button input
// used as loop condition
bool move_boat(Boat* b, Field *f, Position *view){

  flickering_boat(b, view);

  // on ENTER press
  if(uBit.buttonB.isPressed() && uBit.buttonA.isPressed()){
    // return 0 if place is free to stop the loop, 1 if not
    if(place_free(b, f))return false;
    else display_stop();

  // on A press
  }else if(uBit.buttonA.isPressed()){
    // move boat on x-axis
    boat_move_x(b, f->width);
    // update view
    view->x = round_down_to(SCRLENGTH, b->x);

  // on B press
  }else if(uBit.buttonB.isPressed()){
    // move boat on y-axis
    boat_move_y(b, f->height);
    // update view
    view->y = round_down_to(SCRLENGTH, b->y);
  }
  // if ENTER wasn't pressed return true to keep loop going
  return true;
}

// Gets boat locations from user input
void player_place_boats(Field *f, Position *view){
  print_text("place boats");

  // Creates the boats
  Boat *boats[AMOUNT_OF_BOATS] = {make_boat(4), make_boat(3), make_boat(3), make_boat(2), make_boat(2)};

  // for each boat
  for(int i = 0; i < AMOUNT_OF_BOATS; i++){
    // reset current view
    view->x = 0;
    view->y = 0;

    // let player choose boat rotation
    while(get_rotation(boats[i])){
      merge_field(boats, f, i);
      draw_field(f, view, BOATSSHOWING);
      flickering_boat(boats[i], view);
    }
    uBit.sleep(200);

    // let player choose boat rotation
    while(move_boat(boats[i], f, view)){
      merge_field(boats, f, i);
      draw_field(f, view, BOATSSHOWING);
    }
    uBit.sleep(200);
  }
  // save placed boats to field
  merge_field(boats, f, AMOUNT_OF_BOATS);
  free_boats(boats);
}

// Places boat randomly on a field
void ai_place_boats(Field *f){

  // Create the boats
  Boat *boats[AMOUNT_OF_BOATS] = {make_boat(4), make_boat(3), make_boat(3), make_boat(2), make_boat(2)};

  // For each boat
  for(int i = 0; i < AMOUNT_OF_BOATS; i++){

    // put al previous boats on field
    merge_field(boats, f, i);

    // randomly rotate the boat
    if(uBit.random(100) < 30) rotate_boat(boats[i]);

    do{
      // get random x and y values
      boats[i]->x = uBit.random(allowed_boat_x(boats[i], f));
      boats[i]->y = uBit.random(allowed_boat_y(boats[i], f));
      // untill it's place is free
    }while(!place_free(boats[i], f));
  }
  // save placed boats to final field
  merge_field(boats, f, AMOUNT_OF_BOATS);
  free_boats(boats);
}
