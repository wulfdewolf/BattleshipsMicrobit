#include "global.h"
#include "field.h"
#include "display.h"
#include "input.h"
#include "modes.h"

/////////////////////////////////////////////////////////////////////////
//////////////////          1 MICROBITS MODE            /////////////////
/////////////////////////////////////////////////////////////////////////

void start_1_microbit_mode(uint8_t mode){

  uint8_t field_width;
  uint8_t field_height;
  Position *view_position = (Position*)calloc(1, sizeof(Position));
  Position *cursor_position = (Position*)malloc(sizeof(Position));

  // Get field size from player input
  get_val(&field_width, "width?", 5, 15, 5);
  get_val(&field_height, "height?", 5, 15, 5);

  // Create fields
  Field *field1 = create_field(field_width, field_height);
  Field *field2 = create_field(field_width, field_height);

  // Create counters
  uint8_t p1Counter = TOTAL_BOAT_SEGMENTS;
  uint8_t p2Counter = TOTAL_BOAT_SEGMENTS;

  Field *current_field = field2;
  uint8_t *currentCounter = &p2Counter;
  bool is_ai = false;

  // player1 places boats
  print_text("p1");
  player_place_boats(field1, view_position);

  // Define if ai or p2 has to place boats
  switch(mode){
    case 1:
    ai_place_boats(field2);break;
    case 2:
    // player2 places boats
    print_text("p2");
    player_place_boats(field2, view_position);
  }

  print_text("game begins");

  // Game loop
  while(p1Counter > 0 && p2Counter > 0){

    // Reset cursor and current fieldview
    cursor_position->x = 0;
    cursor_position->y = 0;
    view_position->x = 0;
    view_position->y = 0;

    // Define if ai or player has to play
    if(is_ai){
      scroll_text("ai");
      ai_random_cursor(field1);
    } else {
      if(mode == 2)scroll_text("switch");
      player_place_cursor(current_field, cursor_position, view_position);
    }

    // on cursor enter press
    if(field_val_on_pos(cursor_position, current_field) == UNHIT){
      if(!--(*currentCounter))break;
      field_val_on_pos(cursor_position, current_field) = HIT;
      scroll_text("hit");
    } else {
      field_val_on_pos(cursor_position, current_field) = MISS;
      scroll_text("miss");
    }

    // SWAP current player
    if(current_field == field2){
      current_field = field1;
      currentCounter = &p1Counter;
    } else {
      current_field = field2;
      currentCounter = &p2Counter;
    }
    // SWAP to ai if needed
    if(mode == 1){
      if(is_ai){
        is_ai = false;
      } else is_ai = true;
    }
  }
  // WHEN GAME IS DONE

  // Announce winner
  if(!p2Counter) print_text("p1 wins!");
  else if(mode == 1){
    print_text("ai wins!");
  } else print_text("p2 wins!");

  // Free fields and boats
  free_field(field1);
  free_field(field2);
  free(cursor_position);
  free(view_position);
}
