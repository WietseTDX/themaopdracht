#include "MainController.hpp"

void main() {
    while (true) {
        switch (state) {
            case (WAIT_FOR_START_GAME): {
                auto event = wait(startGameFlag);
                state  = WAIT_FOR_SHOT;
            break:
            } // case (WAIT_FOR_SHOT):
            case (WAIT_FOR_SHOT) : {
                auto event = wait(UpdateWeaponFlag + UpdatePlayerNumberFlag + TriggerPressedFlag + BeenShotFlag + PeriodFlag);
                if (event == PeriodFlag && hwlib::now_us() >= last_us += 1000000) {
                    oled.UpdateTime(--time);
                    if (time <= 0) {
                        state = DEAD_WAIT_FOR_PC;
                    }
                }
                if (event == UpdateWeaponFlag) {
                    player_information.setWeapon(commanPool.read());
                    oled.UpdateWeapon(player_information.getWeapon());
                }
                if (event == UpdatePlayerNumberFlag) {
                    player_information.setPlayerNumber(commandPool.read());
                    oled.UpdatePlayerNumber(player_information.getPlayerNumber());
                }
                if (event == triggerPressedFlag) {
                    IrSend.sendMessage(player_information.getPlayerNumber(), player_information.getWeapon());
                }
                if (event == BeenShotFlag) {
                    int damage = calculateDamage(ReceiveDataPool.read());
                    int health = player_information.getHealth();
                    if (health <= damage) {
                        player_information.setHealth(0);
                        oled.UpdateHealth(0);
                        state = DEAD_WAIT_FOR_PC;
                    } else {
                        player_information.setHealth(health - damage);
                        oled.Updatehealth(health - damage);
                    }
                }
            break;
            } // case (WAIT_FOR_SHOT)
            case (DEAD_WAIT_FOR_PC): {
                //========================================//
                // TODO                                   //
                // DEZE STAAT MOET NOG GEMODDELEERD       //
                //========================================//
            break;    
            } // case (DEAD_WAIT_FOR_PC)
        } // switch (state)
    } // while (true)
} // void main()


void triggerPressed(ShootTrigger t) {
    triggerPressed.write(t);
    triggerPressedFlag.set();
}
void beenShot(int damage, int player_number) {
    RecieveDataPool.write(damage);
    ReceiverPlayerPool.write(player_number);
    beenShotFlag.set();
}
void changeWeapon(int weapon) {
    CommandPool.write(weapon);
    UpdateWeaponFlag.set();
}
void changePlayerNumber(int player_number) {
    CommandPool.wirte(player_number);
    UpdatePlayerNumberFlag.set();
}
void startGame() {
    StartGameFlag.set();
}
void updateTime(int time) {
    UpdateTimeFlag.set();
    CommandPool.write(time);
}