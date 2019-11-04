#include "KeyboardController.hpp"

int main() {
    due::pin_oc out0 = due::pin_oc(due::pins::d6);
    due::pin_oc out1 = due::pin_oc(due::pins::d7);
    due::pin_oc out2 = due::pin_oc(due::pins::d8);
    due::pin_oc out3 = due::pin_oc(due::pins::d9);

    due::pin_in in0  = due::pin_in(due::pins::d2);
    due::pin_in in1  = due::pin_in(due::pins::d3);
    due::pin_in in2  = due::pin_in(due::pins::d4);
    due::pin_in in3  = due::pin_in(due::pins::d5);
  
    hwlib::port_oc_from_pins_t out_port = hwlib::port_oc_from_pins_t(out0, out1, out2, out3);
    hwlib::port_in_from_pins_t in_port  = hwlib::port_in_from_pins_t(in0, in1, in2, in3);
    hwlib::matrix_of_switches matrix   = hwlib::matrix_of_switches( out_port, in_port);

    auto testmain = TestMain();
    auto keyboard = KeyboardController(matrix, testmain, due::pins::d13);
    rtos::run(); 
}