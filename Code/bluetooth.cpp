#include "display.h"
#include "bluetooth.h"

/////////////////////////////////////////////////////////////////////////
//////////////            BLUETOOTH CONNECTION            ///////////////
/////////////////////////////////////////////////////////////////////////

static PacketBuffer buffer_b(2);
static int waiting_for_data = 1;
static int is_beginner_b = 0;

//////////////////////////ESTABLISHING CONNECTION////////////////////////

void make_connection_b(){
  // Enable bluetooth
  uBit.radio.enable();
  uBit.radio.setGroup(10);
  uBit.sleep(300);
}

// CODE FROM WPO BUT CHANGED TO WORK WITH BLUETOOTH

// is called by microBit that presses button first
// send data to other microBit
void select_beginner_b(MicroBitEvent e){
  is_beginner_b = 1;
  buffer_b[0] = 0;
  uBit.radio.datagram.send(buffer_b);
}

// is called by both
// only SLAVE sends data back
void read_beginner_b(){
  if(!is_beginner_b){
    buffer_b[0] = 0;
    uBit.radio.datagram.send(buffer_b);
  }
}

// Waits untill a microBit presses a button and will select beginner
// the first microbit that presses a button will be the first one to send data,
// this will cause the other(SLAVE) microbit to stop waiting and call read_beginner
// since he hasn't changed his is_beginner flag, he will send data back
// this will cause the MASTER to stop waiting and both will leave the function
bool choose_beginner_b(){

  // call select_beginner_b when a buttons is pressed
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, select_beginner_b);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, select_beginner_b);

  // wait for data
  // receive_data changes waiting_for_data to zero when data is received
  uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, receive_data);

  while (waiting_for_data) {
    display_wait_animation();
    uBit.sleep(200);
  }
  read_beginner_b();

  // ignore all events
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, select_beginner_b);
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, select_beginner_b);
  clear_display();
  return is_beginner_b;
}


//////////////////////////////DATA EXCHANGE/////////////////////////////

// Bluetooth buffers can't be cleared so NOP
void clear_buffers_b(){
  ;
}

// Is called when data is received
void receive_data(MicroBitEvent e){

  // ignore incoming data from now
  uBit.messageBus.ignore(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, receive_data);

  // store received data in the buffer_b
  buffer_b = uBit.radio.datagram.recv();

  // functions that need data will wait on this function
  waiting_for_data = 0;
}

// Functions can call this if they need data, it will wait and return after data is received
void get_data(){

  // reset the waiting flag
  waiting_for_data = 1;

  // wait for incoming data
  uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, receive_data);
  while(waiting_for_data){
    uBit.sleep(100);
  }
}

// Asks other microBit if he has a boat on given x, y
uint8_t ask_if_hit_b(Position *cursor){

  // loads coords into buffer_b
  buffer_b[0] = cursor->x;
  buffer_b[1] = cursor->y;

  // send buffer_b
  uBit.radio.datagram.send(buffer_b);

  get_data();

  // return the answer
  return buffer_b[0];
}

// Answer other microBit on ask_if_hit
uint8_t answer_if_hit_b(Field *f){

  get_data();

  // get answer
  if(field_val(buffer_b[0], buffer_b[1], f) == UNHIT){
    field_val(buffer_b[0], buffer_b[1], f) = HIT;
    buffer_b[0] = UNHIT;
  } else buffer_b[0] = 0;

  // send answer
  uBit.radio.datagram.send(buffer_b);

  // return answer
  return buffer_b[0];
}

// Asks other microbit for dimensions
void ask_dimensions_b(uint8_t *width_address, uint8_t *height_address){

  get_data();

  // save to dimension addresses
  *width_address = buffer_b[0];
  *height_address = buffer_b[1];
}

// Sends other microbit dimensions
void send_dimensions_b(uint8_t width, uint8_t height){

  // load
  buffer_b[0] = width;
  buffer_b[1] = height;

  // send
  uBit.radio.datagram.send(buffer_b);
}
