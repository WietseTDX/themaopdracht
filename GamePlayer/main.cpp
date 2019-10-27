#include "OLEDController.hpp"


class testMain : rtos::task<> {
private:
    OLEDController oledcontroller;
public:

testMain(OLEDController & oledcontroller):
    oledcontroller(oledcontroller)
{}

void main() override{
    for(;;){
        hwlib::wait_ms(1000);
        hwlib::cout << "Ik ga nu updateTime aanroepen\n";
        oledcontroller.updateTime(0);
        hwlib::wait_ms(1000);
        hwlib::cout << "Ik ga nu updatePlayerNumber aanroepen\n";
        oledcontroller.updatePlayerNumber(1);
        hwlib::wait_ms(1000);
        hwlib::cout << "Ik ga ny updateHealth aanroepen\n";
        oledcontroller.updateHealth(123);
        hwlib::wait_ms(1000);
        hwlib::cout << "Ik ga nu updateWeapon aanroepen\n";
        oledcontroller.updateWeapon(0);
    }
    // for(int i = 1230; i > 0; i--){
    //     oledcontroller.updateTime(i);
    //     oledcontroller.updatePlayerNumber(i%10);
    //     oledcontroller.updateHealth(i%1000);
    //     hwlib::wait_ms(1000);
    // }
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