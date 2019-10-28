#include "hwlib.hpp"
#include "rtos.hpp"

class InputTanslator : public rtos::task<> {
    private:
    rtos::flag commandFlag;
    rtos::pool commandPool;
    rtos::pool ReceivePlayerPool;
    rtos::pool ReceiveDataPool;
    rtos::flag ReceiveIrFlag;

    MainController & main;

    enum state_t = {
        WAIT
    };
    int main() {
        state_t state = WAIT;
        while(true) {
            switch(state){
                case(WAIT): {
                    auto event wait(ReceiveFlag + commandFlag);
                    if (event == ReceiveFlag) {
                        uint8_t data = RecieveDataPool.read();
                        uint8_t player = ReceivePlayerPool.read();
                        if (player > 0 && player < 10) {
                            beenShot(data, player);
                            break;
                        } else if (player == 0) {
                            if (!((data >> 5) & 0x01)) {
                                main.updateTime(data);
                                break;
                            } else if (!data) {
                                main.startGame();
                            } else {
                                break;
                            }
                        } else {
                            break;
                        } 
                    }
                    if (event == commandFlag) {
                        char[4] command = command.read();
                        if (command[0] == 'C') {
                            if (command[1] == 'A') {
                                int number = command[2] - '0';
                                main.changePlayerNumber(number);
                                break;
                            } else if (command[1] == 'B') {
                                int number = command[2] - '0';
                                main.changeWeapon(number);
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                break;
                }
            }
        }
    } 

    //========================
    //    
    //========================
    public:
    convertCommand(command) {
        commandPool.write(command); 
        commandFlag.set();
    }
    convertIrInput(data, player_number) {
        ReceivePlayerPool.write(player_number); 
        ReceiveDataPool.write(data); 
        ReceiveIrFlag.set();
    } 
};