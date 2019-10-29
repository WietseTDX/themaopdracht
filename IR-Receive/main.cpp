#include "receiver.hpp"


int main() {
    hwlib::wait_ms(500);

    auto receiver = IrReceive(due::pins::d8, "receiver" );
    receiver.main();
};