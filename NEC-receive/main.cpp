#include "hwlib.hpp"
#include "rtos.hpp"

class pauseDetector : public rtos::task<>{
    enum states = {IDLE, SIGNAL};

private:
    states state = IDLE;
    int pauseTime = 0;
    rtos::timer Timer;
    due::pin_in IRReceiver;
    pauseListener& listener;

public:
    pauseDetector(due::pins& IRpin):
        IRReceiver( due::pin_in(IRpin) )
    {}

    void main() {
        for(;;){
            switch(state){
                case IDLE:
                    Timer.set(100'000);
                    auto event = wait( Timer );
                    IRReceiver.refresh();
                    if(!IRReceiver.read()) {
                        listener.pause_detected(pauseTime);
                        state = SIGNAL;
                    } else {
                        pauseTime += 100;
                    }
                    break;

                case SIGNAL:
                    Timer.set(100'000);
                    auto event = wait( Timer );
                    IRReceiver.refresh();
                    if(IRReceiver.read()) {
                        pauseTime = 0;
                        state = IDLE;
                    }
                    break;
            }
        }
    }
};