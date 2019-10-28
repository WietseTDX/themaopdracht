#include "CommandTranslator.hpp"

InputTranslator::InputTranslator(MainController & mainC) : CommandFlag(this, "CommandFlag"), ReceiveIrFlag(this, "RecieveIrFlag"), mainC(mainC) {}

void InputTranslator::main() {
    while(true) {
        switch(state){
            case(states::WAIT): {
                hwlib::wait_us(100);
                auto event = wait(ReceiveIrFlag + CommandFlag);
                if (event == ReceiveIrFlag) {
                    uint8_t data = ReceiveDataPool.read();
                    uint8_t player = ReceivePlayerPool.read();
                    if (player > 0 && player < 10) {
                        mainC.beenShot(data, player);
                        break;
                    } else if (player == 0) {
                        if (((data >> 5) & 0x01)) {
                            uint8_t or_bit = 0x01 << 5;
                            data ^= or_bit;
                            mainC.updateTime(data);
                            break;
                        } else if (!data) {
                            mainC.startGame();
                        } else {
                            break;
                        } // if (((data >> 5) & 0x01)) -- else
                    } else {
                        break;
                    } // if (player > 0 && player < 10) -- else
                } // if (event == ReceiveIrFlag)
                if (event == CommandFlag) {
                    char* command = commandPool.read();
                    if (command[0] == 'C') {
                        if (command[1] == 'A') {
                            int number = command[2] - '0';
                            mainC.changePlayerNumber(number);
                            break;
                        } else if (command[1] == 'B') {
                            int number = command[2] - '0';
                            mainC.changeWeapon(number);
                            break;
                        } // if (command[1] == 'A') - else if (command[1] == 'B')
                    } else {
                        break;
                    } // if (event == CommandFlag)
                } // if (event == CommandFlag)
            break;
            } // case(states::WAIT):
        } // switch(state)
    } // while(true)
} // void main()

void InputTranslator::convertCommand(char* command) {
    commandPool.write(command); 
    CommandFlag.set();
}
void InputTranslator::convertIrInput(int data, int player_number) {
    ReceivePlayerPool.write(player_number); 
    ReceiveDataPool.write(data); 
    ReceiveIrFlag.set();
} 