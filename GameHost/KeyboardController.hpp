#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "MainController.hpp"

using hwlib::wait_ms;

/// \brief
/// KeyboardController task for GameLeader
/// \details
/// KeyboardController checks its keypad every 200 milliseconds and passes the input through
/// to a MainController object
class KeyboardController : rtos::task<> {
 private:
  rtos::clock InputClock;
  hwlib::target::pin_out led;

  MainController &main_c;
  hwlib::keypad<17> keypad;

  char command[16];
  char input;
  unsigned int char_count = 0;
  unsigned int no_register_time = 200;
  enum states { IDLE, RECEIVING, SENDING };
  states state = IDLE;

  ///\brief
  /// Control keypress registerd LED
  ///\details
  /// This function turns on, waits and turns off the LED.
  /// The function is called when a key is pressed.
  void ledOnOff();

 public:
  KeyboardController(hwlib::matrix_of_switches &matrix, MainController &main, hwlib::target::pins ledpin)
      : task("KeyboardController"),
        InputClock(this, 200'000, "InputClock"),
        led(hwlib::target::pin_out(ledpin)),
        main_c(main),
        keypad(hwlib::keypad<17>(matrix, "123A456B789C*0#D"))
  {}

  /// \brief
  /// KeyboardController's main loop
  /// \details
  /// Every registered keypress activates an LED to show the user the keypress was registered.
  void main();
};

#endif  // KEYBOARDCONTROLLER_HPP
