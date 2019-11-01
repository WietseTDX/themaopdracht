#ifndef RECEIVER_HPP 
#define RECEIVER_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
// #include "StructData.hpp"

class IrReceiveController : public rtos::task<> {
private:
    enum states {IDLE, RECEIVING};
    states state = IDLE;
    due::pin_in sensor;
    // StructData dataStruct(2, 0);
    // MainController &main_c;
    bool signal;
		bool signal_high = false;
		bool high_check = false;
		bool halfway = false;
		int start_high;
		int start_low;
		int resettime;
    int offset = 175;
    int one_high_time = 1600;
    int zero_high_time = 800;
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
    
    void sendCommand();

    void messageDecode(uint16_t &to_decode);

    void checkingMessage();

public:
    IrReceiveController( due::pins sensor, const char * name ):
        task( name ),
        sensor( due::pin_in(sensor))
    {};

    void main();

};

#endif //RECEIVER_HPP