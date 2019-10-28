#ifndef RECEIVER_HPP 
#define RECEIVER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"

class IrReceive : public rtos::task<> {
private:
    enum states {IDLE, RECEIVING, CHECKING};
    states state = IDLE;
    due::pin_in sensor;
    // InputTranslator& translator;
    bool signal;
		bool signal_high = false;
		bool high_check = false;
		bool halfway = false;
		int start_high;
		int start_low;
		int resettime;
    int bitcount = 0;
    int high_time = 0;

    int first_low_time;
    int first_high_end;


    uint16_t lastmessage;
    uint16_t checkmessage;
    uint8_t lastxor;
    uint8_t checkxor;
    uint8_t player;
    uint8_t data;
    uint8_t lastMathxor;
    uint8_t checkMathxor;
    
    void printMessage();

public:
    IrReceive( due::pins sensor, const char * name ):
        task( name ),
        sensor( due::pin_in(sensor))
    {};

    void main();

};

#endif //RECEIVER_HPP