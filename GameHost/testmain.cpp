#include "hwlib.hpp"
#include "rtos.hpp"
#include <stdio.h>
#include <string.h>

using hwlib::cout;
using hwlib::endl;

class TestMain : rtos::task<> {
    private:
        enum states {IDLE};
        states state = IDLE;
        char command[10];
        rtos::flag CommandFlag;
        rtos::pool<char*> CommandPool;

        void printInput(){
            cout << command << endl;
        }
    
    public:
        TestMain():
            task("TestMain"),
            CommandFlag( this, "CommandFlag"),
            CommandPool( "CommandPool" )
        {}

        void setCmd(char command[]){
            CommandPool.write(command);
            CommandFlag.set();
        }

        void main(){
            state = IDLE;
            for(;;){
                switch(state){
                    case states::IDLE:
                        {
                            wait( CommandFlag );
                            strcpy(command, CommandPool.read());
                            printInput();
                            break;
                        }
                    default:
                        break;
                }
            }
        }       
};