#include "hwlib.hpp"
class Buzzer {
    int wait;
    hwlib::target::pin_out pin;
    bool on = true;
    public:
    Buzzer(hwlib::target::pin_out & pin, int ferquentie) : pin(pin) {
        wait = 1 / ferquentie;
    }
    void main() {
        while (true) {
            hwlib::wait_ms(wait);
            on = !on;
            pin.write(on);
        }
    }
};