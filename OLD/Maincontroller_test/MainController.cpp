#include "MainController.hpp"

void MainController::main() {
    while (true) {
        switch (state) {
            case (states::WAIT_FOR_START_GAME): {
                auto event = wait(StartGameFlag);
                state  = states::WAIT_FOR_SHOT;
            break;
            } // case (WAIT_FOR_SHOT):
            case (states::WAIT_FOR_SHOT) : {
                auto event = wait(UpdateWeaponFlag + UpdatePlayerNumberFlag + TriggerPressedFlag + BeenShotFlag + PeriodFlag);
                if (event == PeriodFlag && hwlib::now_us() >= (last_us += 1000000)) {
                    display.UpdateTime(--time);
                    if (time <= 0) {
                        state = states::DEAD_WAIT_FOR_PC;
                    }
                }
                if (event == UpdateWeaponFlag) {
                    player_information.setWeapon(CommandPool.read());
                    display.UpdateWeapon(player_information.getWeapon());
                }
                if (event == UpdatePlayerNumberFlag) {
                    player_information.setPlayerNumber(CommandPool.read());
                    display.UpdatePlayerNumber(player_information.getPlayerNumber());
                }
                if (event == TriggerPressedFlag) {
                    IrSend.sendMessage(player_information.getPlayerNumber(), player_information.getWeapon());
                }
                if (event == BeenShotFlag) {
                    int damage = calculateDamage(ReceiveDataPool.read());
                    int health = player_information.getHealthPoints();
                    if (health <= damage) {
                        player_information.setHealthPoints(0);
                        display.UpdateHealth(0);
                        state = states::DEAD_WAIT_FOR_PC;
                    } else {
                        player_information.setHealthPoints(health - damage);
                        display.UpdateHealth(health - damage);
                    }
                }
            break;
            } // case (WAIT_FOR_SHOT)
            case (states::DEAD_WAIT_FOR_PC): {
                //========================================//
                // TODO                                   //
                // DEZE STAAT MOET NOG GEMODDELEERD       //
                //========================================//
                hwlib::cout << "DEAD_WAIT_FOR_PC" << hwlib::endl;
                state = states::WAIT_FOR_START_GAME;
            break;    
            } // case (DEAD_WAIT_FOR_PC)
        } // switch (state)
    } // while (true)
} // void main()


void MainController::triggerPressed(ShootTrigger t) {
    TriggerIDPool.write(t);
    TriggerPressedFlag.set();
}
void MainController::beenShot(int damage, int player_number) {
    ReceiveDataPool.write(damage);
    ReceiverPlayerPool.write(player_number);
    BeenShotFlag.set();
}
void MainController::changeWeapon(int weapon) {
    CommandPool.write(weapon);
    UpdateWeaponFlag.set();
}
void MainController::changePlayerNumber(int player_number) {
    CommandPool.write(player_number);
    UpdatePlayerNumberFlag.set();
}
void MainController::startGame() {
    StartGameFlag.set();
}
void MainController::updateTime(int time) {
    UpdateTimeFlag.set();
    CommandPool.write(time);
}