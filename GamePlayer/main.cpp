#include "WindowController.hpp"
#include "PlayerInformation.hpp"

class testMain : rtos::task<> {
private:
    PlayerInformation<100> & info;
    hwlib::target::pin_oc scl                   = hwlib::target::pin_oc{ hwlib::target::pins::scl };
    hwlib::target::pin_oc sda                   = hwlib::target::pin_oc{ hwlib::target::pins::sda };
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus   = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    hwlib::glcd_oled oled                       = hwlib::glcd_oled( i2c_bus );
    WindowController oled_controller            = WindowController(oled, info);
public:

testMain(PlayerInformation<100> & info):
    info(info)
{

}

void main() override{
    oled_controller.update(0);
    hwlib::wait_ms(5);
    oled_controller.update(1);
    hwlib::wait_ms(5);
    oled_controller.update(2);
    hwlib::wait_ms(5);
    oled_controller.update(3);
    hwlib::wait_ms(5);
    for(;;){
        info.setTime(info.getTime()-1);
        oled_controller.update(0);
        hwlib::wait_ms(1000);
    }
}
};

int main(){
    hwlib::wait_ms(2000);
    PlayerInformation<100> info;
    info.setTime(640);
    info.setHealthPoints(100);
    info.setPlayerNumber(2);
    info.setWeapon(4);

    auto main = testMain(info);
    
    rtos::run();
}