#include "MainController.hpp"
#include "IRReceiveController.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

int main() {
    hwlib::target::pin_oc scl = hwlib::target::pin_oc{hwlib::target::pins::scl};
	hwlib::target::pin_oc sda = hwlib::target::pin_oc{hwlib::target::pins::sda};
	hwlib::i2c_bus_bit_banged_scl_sda i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	hwlib::glcd_oled oled = hwlib::glcd_oled(i2c_bus);

    InputHandler a = InputHandler();
    MainController b = MainController(a, oled);

    hwlib::target::pins receive = hwlib::target::pins::d3;
    IRReceiveController ir_receive = IRReceiveController(receive, b);

    rtos::run();
}