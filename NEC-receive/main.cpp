#include "hwlib.hpp"
#include "rtos.hpp"

class msg_logger : public rtos::task<>{
private:
    enum loggerstates { IDLE, SHOW_MESSAGE };
    loggerstates state = IDLE;
    int m;
    rtos::timer Timer;
    rtos::channel<int, 10> messages;

public:

    void msg_received( int m ){
        messages.write( m );
    }

    void main(){
        for(;;){
            switch(state){
                case IDLE:
                    auto event = wait(messages);
                    if(event == messages) {
                        state = SHOW_MESSAGE;
                    }
                    
                    break;
                
                case SHOW_MESSAGE:
                    m = messages.read();
                    hwlib::cout << m << hwlib::endl;
                    Timer.set(5'000'000);
                    event = wait(messages + Timer);
                    if( event == Timer) {
                        state = IDLE;
                    }

                    break;
            }
        }
    }
};

class msg_decoder : public rtos::task<> {
    enum decoderstates = { IDLE, MESSAGE };

private:
    decoderstates state = IDLE;
    int p;
    int n;
    int m;
    rtos::channel<int, 10> pauses;   auto tsop_signal = target::pin_in( target::pins::d8 );
    msg_logger& listener;   auto tsop_signal = target::pin_in( target::pins::d8 );

public:
    msg_decoder( msg_logg   auto tsop_signal = target::pin_in( target::pins::d8 );logger ):
        listener( logger    auto tsop_signal = target::pin_in( target::pins::d8 );
    {}

    void pause_detected(    auto tsop_signal = target::pin_in( target::pins::d8 );t ){
        pauses.write(t);
    }

    void main(){
        for(;;){
            switch(state){
                case IDLE:
                    auto event = wait(pauses);
                    p = pauses.read();
                    if(p > 4000 && p < 5000){
                        n = 0;
                        m = 0;
                        state = MESSAGE;
                    }
                    break;
                
                case MESSAGE:
                    event = wait(pauses);
                    p = pauses.read();
                    if(p > 200 && p < 2000){
                        n++;
                        m = m << 1;
                        m | (p > 1000) ? 1 : 0;
                        if(n == 0){
                            listener.msg_received(m);
                            state = IDLE;
                        }
                    } else {
                        state = IDLE;
                    }
                    break;
            }
        }
    }
};


class pauseDetector : public rtos::task<>{
private:
    enum class pausestates {IDLE, SIGNAL};
    pausestates state = IDLE;
    int pauseTime = 0;
    rtos::timer Timer;
    due::pin_in IRReceiver;
    msg_decoder& listener;

public:
    pauseDetector(msg_decoder& listener, due::pins IRpin):
        IRReceiver( due::pin_in(IRpin) ),
        listener( listener )
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

int main( void ){
   
   namespace target = hwlib::target;
   
   auto tsop_signal = target::pin_in( target::pins::d8 );
   auto tsop_gnd    = target::pin_out( target::pins::d9 );
   auto tsop_vdd    = target::pin_out( target::pins::d10 );
   tsop_gnd.write( 0 );
   tsop_vdd.write( 1 );
   tsop_gnd.flush();
   tsop_vdd.flush();

   msg_logger logger = msg_logger();
   msg_decoder decoder = msg_decoder( logger );
   pauseDetector detector = pauseDetector( decoder, due::pins::d8 );

   detector.main();
   decoder.main();
   logger.main();

};