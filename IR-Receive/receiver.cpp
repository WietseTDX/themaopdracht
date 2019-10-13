#include "receiver.hpp"

void receiver::receiveLoop(){
    state = IDLE;
    for(;;){
        switch(state){
            case IDLE:
                bitCount = 0;
                signalPin.refresh();
                if(!signalPin.read()) {
                    oneTime = 0;
                    while(!signalPin.read()){
                        rtos::wait_us(90'000);
                        signalPin.refresh();
                        if(!signalPin.read()){
                            oneTime += 100;
                        }
                        signalPin.refresh();
                    }
                }
                if((oneTime > 1500) && (oneTime < 1700) ){
                    lastmessage = lastmessage | 1;
                    bitCount = 1;
                    state = RECEIVE;
                }
                break;
            
            case RECEIVE:
                signalPin.refresh();
                if(!signalPin.read()) {
                    oneTime = 0;
                    while(!signalPin.read()){
                        rtos::wait_us(90'000);
                        signalPin.refresh();
                        if(!signalPin.read()){
                            oneTime += 100;
                        }
                        signalPin.refresh();
                    }
                }
                if((oneTime > 1500) && (oneTime < 1700) ){
                    lastmessage = (lastmessage << 1);
                    lastmessage |= 1;
                    bitCount++;
                } else {
                    lastmessage = (lastmessage << 1);
                    bitCount++;
                }

                if(bitCount == 16) {
                    data.received(lastmessage);
                    bitCount = 0;
                    lastmessage = 0x0;
                    state = IDLE;
                }
                break;
        }
    };
};  

void receiver::setmessage(uint16_t message){
    lastmessage = message;
};

void messageData::received( uint16_t message ){
    messages.write( message );    
};

void messageData::storeLoop(){
    
};

uint8_t messageData::returnPlayer() {
    uint8_t message = (lastmessage >> 10);
    uint8_t player = (message & 0x001F);
    hwlib::cout << "player: " << player << hwlib::endl;
    return player;
};

uint8_t messageData::returnData() {
    uint8_t message = (lastmessage >> 5);
    uint8_t data = (message & 0x001F);
    hwlib::cout << "data: " << data << hwlib::endl;
    return data;
};