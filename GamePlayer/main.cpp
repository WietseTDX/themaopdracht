#include "PlayerInformation.hpp"

int main(){
    hwlib::wait_ms(1000);
    auto gozer = PlayerInformation<100>();
    gozer.setWeapon(1);
    gozer.setHealthPoints(200);
    gozer.setPlayerNumber(3);
    gozer.addHit(2, 4);
    gozer.addHit(3,20);
    hwlib::cout << gozer;
}