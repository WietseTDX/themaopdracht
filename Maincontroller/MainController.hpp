#include "hwlib.hpp"
#include "rtos.hpp"

class Maincontroller : public rtos::task<> {
    private:
    rtos::flag UpdateWeaponFlag;
    rtos::flag UpdatePlayerNumberFlag;
    rtos::flag StartGameFlag;
    rtos::flag BeenShotFlag;
    rtos::flag TriggerPressedFlag;

    rtos::pool<int> CommandPool;
    rtos::pool<ShootTrigger> TriggerIDPool;

    IrSendController send;
    OLEDController display;
    PlayerInformation player_information;
    int time;
    int last_us;

    enum class states {WAIT_FOR_SHOT, WAIT_FOR_START_GAME, DEAD_WAIT_FOR_PC};
    states state = states::WAIT_FOR_SHOT;

    public:
    void main() override;
    void triggerPressed(ShootTrigger t);
    void beenShot(int damage, int player_number);
    void changeWeapon(int weapon);
    void changePlayerNumber(int player_number);
    void startGame();
    void updateTime(int time);
    void minSecondPast();
};