#include "MainController.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

int main() {
    hwlib::target::pin_oc scl = hwlib::target::pin_oc{hwlib::target::pins::scl};
	hwlib::target::pin_oc sda = hwlib::target::pin_oc{hwlib::target::pins::sda};
	hwlib::i2c_bus_bit_banged_scl_sda i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	hwlib::glcd_oled oled = hwlib::glcd_oled(i2c_bus);
    oled.clear();
    for (int i=0; i<50; i++) {
        oled.write(hwlib::xy(i,i));
    }
    oled.flush();

    InputHandler a = InputHandler();
    MainController b = MainController(a, oled);
    rtos::run();
}