#include "KeyboardController.hpp"

int main(){

    font_default_8x8 f;
    xy zero(0, 0);

    due::pin_oc scl = due::pin_oc{due::pins::scl};
    due::pin_oc sda = due::pin_oc{due::pins::sda};
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    hwlib::glcd_oled_i2c_128x64_buffered oled = hwlib::glcd_oled_i2c_128x64_buffered(i2c_bus);
    oled.clear();
    oled.flush();

    auto maincontroller = MainController(oled, f);

    due::pin_oc out0 = due::pin_oc(due::pins::d7);
    due::pin_oc out1 = due::pin_oc(due::pins::d8);
    due::pin_oc out2 = due::pin_oc(due::pins::d9);
    due::pin_oc out3 = due::pin_oc(due::pins::d10);

    due::pin_in in0  = due::pin_in(due::pins::d3);
    due::pin_in in1  = due::pin_in(due::pins::d4);
    due::pin_in in2  = due::pin_in(due::pins::d5);
    due::pin_in in3  = due::pin_in(due::pins::d6);
  
    hwlib::port_oc_from_pins_t out_port = hwlib::port_oc_from_pins_t(out0, out1, out2, out3);
    hwlib::port_in_from_pins_t in_port  = hwlib::port_in_from_pins_t(in0, in1, in2, in3);
    hwlib::matrix_of_switches matrix   = hwlib::matrix_of_switches( out_port, in_port);

    auto keyboard = KeyboardController(matrix, maincontroller, due::pins::d13);
    rtos::run();
}