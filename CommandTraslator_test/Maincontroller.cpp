#include "hwlib.hpp"
#include "rtos.hpp"
class MainController : rtos::task<> {
    public:
    void main() {
        while (true) {
            hwlib::wait_ms(10);
        }
    }
    void updateTime(int data) {
        hwlib::cout << "updateTime " << data << hwlib::endl;
    }
    void startGame() {
        hwlib::cout << "StartGame " << hwlib::endl;
    }
    void beenShot(int data, int player) {
        hwlib::cout << "beenShot " << data << " " << player << hwlib::endl;
    }
    void changePlayerNumber(int weapon) {
        hwlib::cout << "changePlayerNumber " << weapon << hwlib::endl;
    }
    void changeWeapon(int weapon) {
        hwlib::cout << "changeWeapon " << weapon << hwlib::endl;
    }
};