#include "receiver.hpp"

void receiver::setmessage(uint16_t message){
    lastmessage = message;
};

uint8_t receiver::returnPlayer() {
    uint8_t message = (lastmessage >> 10);
    uint8_t player = (message & 0x001F);
    hwlib::cout << "player: " << player << hwlib::endl;
    return player;
};

uint8_t receiver::returnData() {
    uint8_t message = (lastmessage >> 5);
    uint8_t data = (message & 0x001F);
    hwlib::cout << "data: " << data << hwlib::endl;
    return data;
};