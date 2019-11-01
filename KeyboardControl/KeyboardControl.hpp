#include "hwlib.hpp"
#include "rtos.hpp"
// #include <string>

struct StructData {
  int to_change;
  uint16_t data;
  StructData(int to_change, uint16_t data) : to_change(to_change), data(data) {}
};

class MainController {
  public:
  void translateCommand(StructData data) {
      hwlib::cout << data.to_change << hwlib::endl;
      hwlib::cout << "data " << data.data << hwlib::endl;
  }
};



class KeyboardController {
  hwlib::target::pin_oc out0 = hwlib::target::pin_oc(hwlib::target::pins::d6);
  hwlib::target::pin_oc out1 = hwlib::target::pin_oc(hwlib::target::pins::d7);
  hwlib::target::pin_oc out2 = hwlib::target::pin_oc(hwlib::target::pins::d8);
  hwlib::target::pin_oc out3 = hwlib::target::pin_oc(hwlib::target::pins::d9);

  hwlib::target::pin_in in0  = hwlib::target::pin_in(hwlib::target::pins::d2);
  hwlib::target::pin_in in1  = hwlib::target::pin_in(hwlib::target::pins::d3);
  hwlib::target::pin_in in2  = hwlib::target::pin_in(hwlib::target::pins::d4);
  hwlib::target::pin_in in3  = hwlib::target::pin_in(hwlib::target::pins::d5);
  
  hwlib::port_oc_from_pins_t out_port = hwlib::port_oc_from_pins_t(out0, out1, out2, out3);
  hwlib::port_in_from_pins_t in_port  = hwlib::port_in_from_pins_t(in0, in1, in2, in3);
  hwlib::matrix_of_switches matrix   = hwlib::matrix_of_switches( out_port, in_port);
  hwlib::keypad<16> keypad   = hwlib::keypad<16>(matrix, "123A456B789C*0#D");
  MainController& main_c;
  char current_cmd[3];
  int place = 0;
  public:
  KeyboardController(MainController & main_c);
  void update();
};