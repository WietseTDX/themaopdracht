#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "MainController.hpp"

/// \brief
/// KeyboardController task for GameLeader
/// \details
/// KeyboardController checks its keypad every 200 milliseconds and passes the input through
/// to a MainController object
class KeyboardController : rtos::task<> {
    private:
        enum states {IDLE, RECEIVING, SENDING};
        states state = IDLE;
        hwlib::target::pin_out led;
        MainController &main_c;
        hwlib::keypad<17> keypad;
        char command[10];
        char input;
        rtos::clock InputClock;

        void ledOnOff();
    
    public:
        KeyboardController(hwlib::matrix_of_switches& matrix, MainController &main, hwlib::target::pins ledpin):
            task("KeyboardController"),
            led( hwlib::target::pin_out(ledpin)),
            main_c(main),
            keypad(hwlib::keypad<17>(matrix, "123A456B789C*0#D")),
            InputClock( this, 200'000, "InputClock")
        {}

        void main();

};

#endif // KEYBOARDCONTROLLER_HPP
