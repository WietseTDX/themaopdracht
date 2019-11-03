#include "receiver.hpp"

int main() {
    hwlib::wait_ms(500);

    auto receiver = IrReceiveController(due::pins::d8, "receiver" );
		rtos::run();
};