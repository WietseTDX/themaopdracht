#include "OLEDController.hpp"


class testMain : rtos::task<> {
private:
    OLEDController& oledcontroller;
public:

testMain(OLEDController & oledcontroller):
    oledcontroller(oledcontroller)
{}

void main() override{
    int time = 630;
    for(;;){
        hwlib::wait_ms(1000);
        oledcontroller.updateTime(time);
        hwlib::wait_ms(1000);
        oledcontroller.updatePlayerNumber(time%10);
        hwlib::wait_ms(1000);
        oledcontroller.updateHealth(time);
        hwlib::wait_ms(1000);
        oledcontroller.updateWeapon(time%100);
        time--;
    }
}
};

int main(){
    hwlib::wait_ms(2000);
    auto scl             = hwlib::target::pin_oc{ hwlib::target::pins::scl };
    auto sda             = hwlib::target::pin_oc{ hwlib::target::pins::sda };
    auto i2c_bus         = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    auto oled            = hwlib::glcd_oled( i2c_bus );

    auto oledcontroller = OLEDController(oled);
    auto gozer = testMain(oledcontroller);
    rtos::run();
}