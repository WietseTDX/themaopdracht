#ifndef KEYBOARDCONTOLLER_HPP
#define KEYBOARDCONTOLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "StructData.cpp"

class KeyboardListener {
  public:
  virtual void translateCmd(StructData info, int id) = 0;
};


class KeyboardController {
  hwlib::target::pin_oc out0 = hwlib::target::pin_oc(hwlib::target::pins::d7);
  hwlib::target::pin_oc out1 = hwlib::target::pin_oc(hwlib::target::pins::d8);
  hwlib::target::pin_oc out2 = hwlib::target::pin_oc(hwlib::target::pins::d9);
  hwlib::target::pin_oc out3 = hwlib::target::pin_oc(hwlib::target::pins::d10);

  hwlib::target::pin_in in0  = hwlib::target::pin_in(hwlib::target::pins::d3);
  hwlib::target::pin_in in1  = hwlib::target::pin_in(hwlib::target::pins::d4);
  hwlib::target::pin_in in2  = hwlib::target::pin_in(hwlib::target::pins::d5);
  hwlib::target::pin_in in3  = hwlib::target::pin_in(hwlib::target::pins::d6);
  
  hwlib::port_oc_from_pins_t out_port = hwlib::port_oc_from_pins_t(out0, out1, out2, out3);
  hwlib::port_in_from_pins_t in_port  = hwlib::port_in_from_pins_t(in0, in1, in2, in3);
  hwlib::matrix_of_switches matrix   = hwlib::matrix_of_switches( out_port, in_port);
  hwlib::keypad<16> keypad   = hwlib::keypad<16>(matrix, "123A456B789C*0#D");
  char current_cmd[3];
  int place = 0;
  int id;
  int index_listener = 0;
  KeyboardListener *keyboard_listener[2];
  public:
  /// \brief
  /// Constructor
  /// \details
  /// Initialise all the rtos objects and takes a reference to the maincontroller
  KeyboardController(int id) :id(id) {}

  /// \brief
  /// Update functie
  /// \details
  /// it checkes if a key is pressed on the keypad and if a command is competed it wil give that to the maincontroller
  void update();

  /// \brief
  /// add a buttonlistener
  /// \details
  /// add a button listener with the right object to send the right message to the right object
  void addKeyboardListener(KeyboardListener *object);
};
#endif // KEYBOARDCONTOLLER_HPP