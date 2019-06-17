#ifndef CONNECTION_H
#define CONNECTION_H


// Assigns connection functions based on mode
void get_connection_API(uint8_t mode);

// Different functions to be assigned
typedef void(*make_connection_function)();
typedef bool(*choose_beginner_function)();
typedef void(*clear_buffers_function)();
typedef uint8_t(*ask_if_hit_function)(Position*);
typedef uint8_t(*answer_if_hit_function)(Field*);
typedef void(*ask_dimensions_function)(uint8_t*, uint8_t*);
typedef void(*send_dimensions_function)(uint8_t, uint8_t);

// ESTABLISHING CONNECTION

extern make_connection_function make_connection;

extern choose_beginner_function choose_beginner;

// DATA EXCHANGE

extern clear_buffers_function clear_buffers;

extern ask_if_hit_function ask_if_hit;

extern answer_if_hit_function answer_if_hit;

extern ask_dimensions_function ask_dimensions;

extern send_dimensions_function send_dimensions;


#endif
