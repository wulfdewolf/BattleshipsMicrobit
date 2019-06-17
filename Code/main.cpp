#include "global.h"
#include "display.h"
#include "modes.h"
#include "input.h"

/*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*/
/* PROJECT: STRUCTUUR VAN COMPUTERPROGRAMMA'S II                               */
/*                                                                             */
/*                                BATTLESHIPS                                  */
/*                                                                             */
/* BBC MicroBit               author: Wolf De Wulf                             */
/* 2018                                                                        */
/*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*;*/



// UBIT
MicroBit uBit;

int main(){

    // Initialise the micro:bit runtime
    uBit.init();
    init_display();

    uint8_t mode;

    // Infinite loop
    programloop:

    // Get mode from player input
    get_val(&mode, "mode?", 1, 4, 1);

    // Start game depending on mode
    switch(mode){

      case 1: // Singleplayer mode against ai on 1 microBit
      case 2: // Multiplayer mode on 1 microBit
        start_1_microbit_mode(mode);break;

      case 3: // Multiplayer mode on seperate microBits --> with cables
      case 4: // Multiplayer mode on seperate microBits --> with bluetooth
        start_2_microbit_mode(mode);break;
    }

    // jump back to beginning of loop
    goto programloop;
}
