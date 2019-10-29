#include "hwlib.hpp"
#include "rtos.hpp"

class IrSendController : public rtos::task<> {
    public:
    void main() override { 
        hwlib::wait_ms(10);
    }
    void sendMessage(int player, int weapon) {
        hwlib::cout << "sendMessage IrSend, Player: " << player << " Weapon: " << weapon << hwlib::endl;
    }
};