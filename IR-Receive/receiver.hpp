#ifndef RECEIVER_HPP 
#define RECEIVER_HPP
#include "hwlib.hpp"

class receiver {
private:
    due::pin_in signalPin;
    uint16_t lastmessage = 0x0;

public:
    receiver( due::pins signal ):
    signalPin( due::pin_in(signal))
    {};

    // setmessage for testing without actually receiving a signal
    void setmessage(uint16_t message);

    uint8_t returnPlayer();

    uint8_t returnData();


};

#endif //RECEIVER_HPP