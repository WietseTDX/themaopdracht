#include "receiver.hpp"
using hwlib::cout;
using hwlib::endl;
//==================================
// Private functions "IrReceive"
//==================================

void IrReceive::printMessage() {
    cout << "player: " << player << endl;
    cout << "data: " << data << endl;
};

//==================================
// Public funtions "IrReceive"
//==================================

void IrReceive::main(){
    state = IDLE;
    for(;;){
        switch(state){
            case IDLE:
                hwlib::wait_us(10'000);
                sensor.refresh();
                signal = !sensor.read();
                if(signal) {
                    oneTime += 10;
                } else {
                    if(oneTime > 0){
                        endTime = hwlib::now_us();
                        lastmessage = 0;
                        state = RECEIVING;
                    };
                };
                break;
            
            case RECEIVING:
                hwlib::wait_us(10'000);
                sensor.refresh();
                signal = !sensor.read();
                
                if(bitCount == 16){
                    checkmessage = lastmessage;
                    lastmessage = 0;
                    hwlib::wait_us(300'000);
                };

                if(bitCount == 32){
                    state = CHECKING;
                    break;
                };

                if(signal){
                    if(bitCount == 0){
                        zeroTime = hwlib::now_us() - endTime;
                        if(zeroTime > 750 && zeroTime < 850){
                            lastmessage |= 1;
                            bitCount++;
                        } else {
                            zeroTime = 0;
                            oneTime = 0;
                        }
                    }
                    oneTime += 10;
                } else {
                    if(oneTime > 750 && oneTime < 850){
                        lastmessage = (lastmessage << 1);
                    } else if(oneTime > 1550 && oneTime < 1700){
                        lastmessage = (lastmessage << 1);
                        lastmessage |= 1;
                    } else {
                        zeroTime = 0;
                        oneTime = 0;
                        state = IDLE;
                        break;
                    }
                    oneTime += 10;
                };
                break;
            
            case CHECKING:
                uint8_t message = (checkmessage >> 10);
                player = (message & 0x001F);
                message = (checkmessage >> 5);
                data = (message & 0x001F);

                checkMathxor = (data ^ player);

                message = (lastmessage >> 10);
                player = (message & 0x001F);
                message = (lastmessage >> 5);
                data = (message & 0x001F);

                lastMathxor = (data ^ player);

                message = lastmessage;
                lastxor = (message & 0x001F);

                message = checkmessage;
                checkxor = (message & 0x001F);

                if(lastmessage == checkmessage){
                    printMessage();
                } else if(lastMathxor == checkMathxor){
                    printMessage();
                } else {
                    if(lastxor == lastMathxor){
                        message = (checkmessage >> 10);
                        player = (message & 0x001F);
                        message = (checkmessage >> 5);
                        data = (message & 0x001F);
                    } else if(checkxor == checkMathxor){
                        message = (lastmessage >> 10);
                        player = (message & 0x001F);
                        message = (lastmessage >> 5);
                        data = (message & 0x001F);
                    } else {
                        zeroTime = 0;
                        oneTime = 0;
                        state = IDLE;
                        break;
                    }
                    printMessage();
                }
                zeroTime = 0;
                oneTime = 0;
                state = IDLE;
                break;
        }
    };
};  