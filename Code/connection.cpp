#include "serial.h"
#include "bluetooth.h"
#include "connection.h"

/////////////////////////////////////////////////////////////////////////
//////////////            MICROBIT CONNECTION           /////////////////
/////////////////////////////////////////////////////////////////////////

// ESTABLISHING CONNECTION

make_connection_function make_connection;

choose_beginner_function choose_beginner;

// DATA EXCHANGE

clear_buffers_function clear_buffers;

ask_if_hit_function ask_if_hit;

answer_if_hit_function answer_if_hit;

ask_dimensions_function ask_dimensions;

send_dimensions_function send_dimensions;

// Assigns connection functions based on mode
void get_connection_API(uint8_t mode){
  switch(mode){
    case 3:
    make_connection = make_connection_s;
    choose_beginner = choose_beginner_s;
    clear_buffers = clear_buffers_s;
    ask_if_hit = ask_if_hit_s;
    answer_if_hit = answer_if_hit_s;
    ask_dimensions = ask_dimensions_s;
    send_dimensions = send_dimensions_s;
    break;
    case 4:
    make_connection = make_connection_b;
    choose_beginner = choose_beginner_b;
    clear_buffers = &clear_buffers_b;
    ask_if_hit = ask_if_hit_b;
    answer_if_hit = answer_if_hit_b;
    ask_dimensions = ask_dimensions_b;
    send_dimensions = send_dimensions_b;
    break;
  }
}
