#include "KeyboardControl.hpp"

int main() {
    hwlib::wait_ms(1000);
    MainController b;
    KeyboardController a(b);
    while (true) {
        a.update();
        hwlib::wait_ms(100);
        // hwlib::cout << "Loop" <<hwlib::endl;
    }
}