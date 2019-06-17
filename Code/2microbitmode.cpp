#include "global.h"
#include "field.h"
#include "display.h"
#include "modes.h"
#include "input.h"
#include "connection.h"

/////////////////////////////////////////////////////////////////////////
//////////////////          2 MICROBITS MODE            /////////////////
/////////////////////////////////////////////////////////////////////////

void start_2_microbit_mode(uint8_t mode){

  // Gets the right connection functions
  get_connection_API(mode);

  make_connection();
  uint8_t is_beginner = choose_beginner();

  uint8_t field_width;
  uint8_t field_height;
  Position *view_position = (Position*)calloc(1, sizeof(Position));
  Position *cursor_position = (Position*)malloc(sizeof(Position));

  clear_buffers();

  if(is_beginner){
    get_val(&field_width, "width?", 5, 15, 5);
    get_val(&field_height, "height?", 5, 15, 5);
    send_dimensions(field_width, field_height);
    scroll_text("1st");
  } else {
    ask_dimensions(&field_width, &field_height);
    scroll_text("2nd");
  }

  clear_buffers();

  // Create fields
  Field *field = create_field(field_width, field_height);
  Field *play_field = create_field(field_width, field_height);

  // Create counters
  int p1Counter = TOTAL_BOAT_SEGMENTS;
  int p2Counter = TOTAL_BOAT_SEGMENTS;

  // player places boats
  player_place_boats(field, view_position);

  // Game loop
  while(p2Counter > 0 && p1Counter > 0){
    // Define who is playing

    if(is_beginner){
      // PLAYING MICROBIT
      // Reset cursor and current fieldview
      cursor_position->x = 0;
      cursor_position->y = 0;
      view_position->x = 0;
      view_position->y = 0;

      player_place_cursor(play_field, cursor_position, view_position);

      // on enter press
      if(ask_if_hit(cursor_position)){
        if(!--p2Counter)  break;
        field_val_on_pos(cursor_position, play_field) = HIT;
        scroll_text("hit");
      } else {
        field_val_on_pos(cursor_position, play_field) = MISS;
        scroll_text("miss");
      }
      // SWAP
      is_beginner = 0;

    } else {
      // ANSWERING MICROBIT
        draw_field(play_field, view_position, BOATSHIDDEN);
        if(answer_if_hit(field)){
          scroll_text("p2 hit");
          if(!--p1Counter)  break;
        } else {
          scroll_text("p2 miss");
        }
        // SWAP
        is_beginner = 1;
        clear_buffers();
    }
  }
  // WHEN GAME IS DONE

  // Announce winner
  if(!p2Counter) print_text("you win!");
  else print_text("you lose!");

  // Free fields and positions
  free_field(field);
  free_field(play_field);
  free(cursor_position);
  free(view_position);
}
