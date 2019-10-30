#include "hwlib.hpp"
#include "rtos.hpp"

class OLEDController : public rtos::task<> {
    public:
    void main() override {
        hwlib::wait_ms(10);
    }
    void UpdateTime(int time) {
        hwlib::cout << "Update time, time: " << time << hwlib::endl;
    }

    void UpdateWeapon(int weapon) {
        hwlib::cout << "UpdateWeapon, weapon: " << weapon << hwlib::endl;
    }

    void UpdatePlayerNumber(int player) {
        hwlib::cout << "update player number, player: " << player << hwlib::endl;
    }

    void UpdateHealth(int health) {
        hwlib::cout << "update health, health: " << health << hwlib::endl;
    }

};