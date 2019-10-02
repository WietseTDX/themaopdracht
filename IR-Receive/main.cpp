#include "receiver.hpp"


int main() {
    hwlib::wait_ms(500);
    auto receiver1 = receiver(due::pins::d8);
    uint16_t message = 0x9A40;
    receiver1.setmessage(message);
    receiver1.returnPlayer();
    receiver1.returnData();
};