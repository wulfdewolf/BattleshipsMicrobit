#include "display.h"
#include "serial.h"


/////////////////////////////////////////////////////////////////////////
/////////////////////       SERIAL CONNECTION            ////////////////
/////////////////////////////////////////////////////////////////////////

static uint8_t buffer[2];
static bool beginner_chosen = 0;
static bool is_beginner_s = 0;

//////////////////////////ESTABLISHING CONNECTION////////////////////////

void make_connection_s(){
  uBit.serial.setRxBufferSize(20);
  uBit.serial.setTxBufferSize(20);
  uBit.serial.baud(115200);
  uBit.serial.redirect(MICROBIT_PIN_P1, MICROBIT_PIN_P2);
  uBit.serial.clearRxBuffer();
  uBit.serial.clearTxBuffer();
}

// CODE FROM WPO

void select_beginner_s(MicroBitEvent e) {
  is_beginner_s = true;
  beginner_chosen = true;
  uint8_t values[20] = {42, 43};
  uBit.serial.send(values, 20);
}

void read_beginner_s(MicroBitEvent e) {
  beginner_chosen = 1;
  is_beginner_s = 0;
}

bool choose_beginner_s(){
  uBit.sleep(200);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, select_beginner_s);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, select_beginner_s);
  uBit.serial.eventAfter(20);
  uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_HEAD_MATCH, read_beginner_s);
  while (!beginner_chosen){
    display_wait_animation();
    uBit.sleep(100);
  }

  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, select_beginner_s);
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, select_beginner_s);
  uBit.messageBus.ignore(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_HEAD_MATCH, read_beginner_s);
  clear_display();
  return is_beginner_s;
}


//////////////////////////////DATA EXCHANGE/////////////////////////////

// Clears receive and send buffers
void clear_buffers_s(){

    uBit.serial.clearRxBuffer();
    uBit.serial.clearTxBuffer();
}

// Asks other microBit if he has a boat on given x, y
uint8_t ask_if_hit_s(Position *cursor){

  // loads coords into buffer
  buffer[0] = cursor->x;
  buffer[1] = cursor->y;

  // send buffer
  uBit.serial.send(buffer, 2);

  // wait for answer
  uBit.serial.read(buffer, 1, SYNC_SLEEP);
  return buffer[0];
}

// Answer other microBit on ask_if_hit
uint8_t answer_if_hit_s(Field *f){

  // wait for question
  uBit.serial.read(buffer, 2, SYNC_SLEEP);

  // get answer
  if(field_val(buffer[0], buffer[1], f) == UNHIT){
    field_val(buffer[0], buffer[1], f) = HIT;
    buffer[0] = 1;
  } else buffer[0] = 0;

  // send answer
  uBit.serial.send(buffer, 1);

  // return answer
  return buffer[0];
}

// Asks other microbit for dimensions
void ask_dimensions_s(uint8_t *width_address, uint8_t *height_address){

  // wait for data
  uBit.serial.read(buffer, 1, SYNC_SLEEP);

  // save to dimension addresses
  *width_address = buffer[0];
  *height_address = buffer[1];
}

// Sends other microbit dimensions
void send_dimensions_s(uint8_t width, uint8_t height){

  // load data
  buffer[0] = width;
  buffer[1] = height;

  // send data
  uBit.serial.send(buffer, 1);
}
