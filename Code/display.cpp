#include "field.h"
#include "display.h"

/////////////////////////////////////////////////////////////////////////
////////////////////////           DISPLAY            ///////////////////
/////////////////////////////////////////////////////////////////////////

// Initialises the display
void init_display(){
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
}

////////////////////////////////PRINTING/////////////////////////////////

// Prints a message on the display(one letter at a time)
void print_text(const char* msg){
  uBit.display.print(msg);
}

// Prints a message on the display(one digit at a time)
void print_number(uint8_t number){
  uBit.display.print(number);
}

// Scrolls a message on the display
void scroll_text(const char* msg){
  uBit.display.scroll(msg);
}

// Clears the dispplay
void clear_display(){
  uBit.display.clear();
}

// holds pixel intensities for WATER, UNHIT, HIT, MISS
static int intensities[4] = { 0, 255, 255, 20 };

// Draws a field on the microBit
void draw_field(Field *f, Position *view, bool hidden){
  int intensity;

  // clear the display
  uBit.display.clear();

  // for each bit in current view
  for(int x=view->x; x<view->x+SCRLENGTH; x++){
    for(int y=view->y; y<view->y+SCRLENGTH; y++){

      // if BOATSHIDDEN is true don't show the boatsegments
      if(hidden && field_val(x, y, f) == UNHIT)intensity = 0;
      // else get the correct intensity
      else intensity = intensities[field_val(x, y, f)];
      // draw
      uBit.display.image.setPixelValue(x%SCRLENGTH,y%SCRLENGTH, intensity);
    }
  }
}

///////////////////////////////FLICKERING////////////////////////////////

// Makes cursor flicker, supposed to be used in a loop
void flickering_cursor(Position *cursor){

  int intensity = 255;
  int done = 0;

  while(!done){
    uBit.display.image.setPixelValue(cursor->x%SCRLENGTH,cursor->y%SCRLENGTH, intensity);
    
    if(intensity)intensity = 0;
    else done = 1;
    uBit.sleep(100);
  }
}

// Checks if a coordinate is inside a view or not
bool in_view(int x, int y, Position *view){
 return ((x >= view->x) &&
         (x < view->x+SCRLENGTH) &&
         (y >= view->y) &&
         (y < view->y+SCRLENGTH));
}


// Makes boat flicker, supposed to be used in a loop
void flickering_boat(Boat *b, Position *view){

  int intensity = 255;
  int done = 0;

  while(!done){
    // for each boat segment
    for(int i = 0; i < b->size; i++){
      if(in_view(boat_segment_x(b, i), boat_segment_y(b, i), view)){
        uBit.display.image.setPixelValue(boat_segment_x(b, i)%SCRLENGTH, boat_segment_y(b, i)%SCRLENGTH, intensity);
      }
    }
    if(intensity)intensity = 0;
    else done = 1;
    uBit.sleep(100);
  }
}

///////////////////////////////ANIMATIONS////////////////////////////////

const uint8_t load[] = { 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 255, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 255, 0, 0,
                         0, 255, 255, 255, 0,
                         0, 0, 255, 0, 0,
                         0, 255, 255, 255, 0,
                         255, 255, 255, 255, 255,
                         0, 0, 0, 0, 0,
                         0, 0, 255, 0, 0,
                         0, 255, 255, 255, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 255, 0, 0};

static MicroBitImage waitAnimation(15 ,5 ,load);

// Displays a waiting animation on the uBit display
void display_wait_animation(){
  uBit.display.animate(waitAnimation,200,5);
}

const uint8_t stop[] = { 255, 0, 0, 0, 255,
                         255, 0, 0, 0, 255,
                         0, 255, 0, 255, 0,
                         0, 255, 0, 255, 0,
                         0, 0, 255, 0, 0,
                         0, 0, 255, 0, 0,
                         0, 255, 0, 255, 0,
                         0, 255, 0, 255, 0,
                         255, 0, 0, 0, 255,
                         255, 0, 0, 0, 255};

static MicroBitImage stopAnimation(10, 5, stop);

// Displays stopsign on the uBit display
void display_stop(){
  uBit.display.clear();
  uBit.sleep(100);
  uBit.display.animate(stopAnimation, 200, 5);
}
