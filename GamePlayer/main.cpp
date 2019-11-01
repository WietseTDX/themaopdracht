#include "PlayerInformation.hpp"
#include "Button.hpp"

// class testMain : rtos::task<>, ButtonListener {
// private:
    
// public:

// testMain():
// {}

// void main() override{
//    return;
// }
// };

int main(){
    hwlib::wait_ms(1000);
    auto gozer = PlayerInformation<100>();
    gozer.setWeapon(1);
    gozer.setHealthPoints(200);
    gozer.setPlayerNumber(3);
    gozer.addHit(2, 4);
    gozer.addHit(3,20);
    hwlib::cout << gozer;
    due::pin_in pin = due::pin_in( due::pins::d22 );
    Button knop(pin, 1);

}