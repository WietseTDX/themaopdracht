#include "hwlib.hpp"
#include "rtos.hpp"
#include "Maincontroller.cpp"

class InputTranslator : public rtos::task<> {
    private:
    rtos::flag CommandFlag;
    rtos::flag ReceiveIrFlag;
    rtos::pool<char*> commandPool;
    rtos::pool<int> ReceivePlayerPool;
    rtos::pool<int> ReceiveDataPool;

    MainController & mainC;

    enum class states {
        WAIT
    };
    states state = states::WAIT;


    //========================
    // public InputTranslator
    //========================
    public:
    void main() override; 
    InputTranslator(MainController & mainC);
    void convertCommand(char* command);
    void convertIrInput(int data, int player_number); 
};