#include "hwlib.hpp"
#include "rtos.hpp"

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
    /// \brief
    /// Main functie
    /// \details
    /// This is the main function of the code it has the stats and logic
    void main() override; 

    /// \brief
    /// Constructor
    /// 
    InputTranslator(MainController & mainC);
    void convertCommand(char* command);
    void convertIrInput(int data, int player_number); 
};