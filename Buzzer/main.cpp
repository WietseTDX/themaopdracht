#include "hwlib.hpp"
#include "Buzzer.hpp"

int main() {
    hwlib::target::pin_out pin = hwlib::target::pin_out(hwlib::target::pins::d12);
    Buzzer a = Buzzer(pin, 1000);
    a.main();
}