#include "hwlib.hpp"
#include "rtos.hpp"

struct CommandData {
    CommandData(int type, int data) : type(type), data(data) {}
    int type = 0;
    int data = 0;
};



class MainController : public rtos::task<> {
    private:
    rtos::flag ButtonPressedFlag.set();
    rtos::pool<Button> ButtonIDPool;
    rtos::channel<CommandData, 10> CommandChannel;
    rtos::periode PeriodFlag;

    eunm class states {WAIT_FOR_COMMAND, WAIT_FOR_START_GAME, WIAT_FOR_PC};
    states state = states::WAIT_FOR_COMMAND;

    public:
    void main() override;
    void buttonPressed(Button b);
    void translateCommand(CommandData data);
    void runCommand(CommandData command);
}