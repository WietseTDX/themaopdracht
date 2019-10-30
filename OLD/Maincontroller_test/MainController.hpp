#ifndef _MAIN_CONTROLLER
#define _MAIN_CONTROLLER

#include "hwlib.hpp"
#include "rtos.hpp"
#include "IrSend.cpp"
#include "OLEDController.cpp"
#include "Playerinformation.cpp"
#include "ShootTrigger.cpp"

int calculateDamage(int a) {
    return a;
}

class MainController : public rtos::task<> {
    private:
    rtos::flag UpdateWeaponFlag;
    rtos::flag UpdatePlayerNumberFlag;
    rtos::flag StartGameFlag;
    rtos::flag BeenShotFlag;
    rtos::flag TriggerPressedFlag;
    rtos::flag UpdateTimeFlag;

    // dit moet een periode flag zijn
    rtos::flag PeriodFlag;

    rtos::pool<int> CommandPool;
    rtos::pool<int> ReceiveDataPool;
    rtos::pool<int> ReceiverPlayerPool;
    rtos::pool<ShootTrigger> TriggerIDPool;

    IrSendController IrSend;
    OLEDController display;
    PlayerInformation<100> player_information;
    int time;
    uint64_t last_us;

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

#endif /* _MAIN_CONTROLLER */