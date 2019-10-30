#include "hwlib.hpp"
#include "rtos.hpp"

class ShootTrigger : public rtos::task<> {
    public:
    void main() override {
        hwlib::wait_ms(10);
    }
};