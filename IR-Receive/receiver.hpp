#ifndef RECEIVER_HPP 
#define RECEIVER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"

class messageData : public rtos::task<>{
private:
    enum states {IDLE, MESSAGE};
    states state = IDLE;
    uint16_t lastmessage = 0x0;
    rtos::channel<uint16_t, 3> messages;

public:
    void received( uint16_t message );

    uint8_t returnPlayer();

    uint8_t returnData();

    void storeLoop();
};

class receiver : public rtos::task<> {
private:
    enum states {IDLE, RECEIVE};
    states state = IDLE;
    due::pin_in signalPin;
    messageData& data;
    int bitCount = 0;
    int oneTime = 0;
    uint16_t lastmessage = 0x0;

public:
    receiver( due::pins signal, messageData& data ):
        signalPin( due::pin_in(signal)),
        data( data )
    {};

    void receiveLoop();

    // setmessage for testing without actually receiving a signal
    void setmessage(uint16_t message);

};

#endif //RECEIVER_HPP