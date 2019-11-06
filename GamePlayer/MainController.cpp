#include "MainController.hpp"

//==================================
// PRIVATE FUNCTIONS MainController
//==================================
int MainController::timeWaitShot() {
  int weapon = info.getWeapon();
  int wait = 500000;
  wait += 500000 * weapon;
  return wait;
}

int MainController::calculateDamage(int data) { return (data * 1) + 1; }

void MainController::runCommand(StructData command) {
  if (command.to_change == 2) {  // change health
    int player = (command.data >> 5) & 0x001F;
    if (player != 0 && (!been_shot) && player != info.getPlayerNumber()) {
      int data = command.data & 0x001F;
      buzzer.write(1);
      BuzzerTimer.set(250'000);
      info.addHit(player, data);
      BeenShotTimer.set(3'000'000);
      been_shot = true;
      int damage = calculateDamage(data);
      int health = info.getHealthPoints();
      health -= damage;
      if (health < 0) {
				cout << "health below 0" << endl;
        health = 0;
      }
      info.setHealthPoints(health);
      window.update(2);
    }
  } else if (command.to_change == 3) {  // change weapon
    info.setWeapon(command.data);
    window.update(3);
  }
}

bool MainController::startGame() {
  StructData command = CommandChannel.read();
  if (command.to_change == 2) {  // change time or start game
    int data = command.data & 0x001F;
    int player = (command.data >> 5) & 0x001F;
    if (player == 0) {
      if (((data >> 4) & 0x01)) {
        info.setTime((data & 0b000001111) * 60);
        window.update(0);
        return false;
      }
      if (data == 0) {
        if (!(info.getPlayerNumber() == 0)) {
          return true;
        }
        return false;
      }
    }
  } else if (command.to_change == 1) {  // change player number
    info.setPlayerNumber(command.data);
    window.update(1);
  }
  return false;
}

//==================================
// PUBLIC FUNCTIONS MainController
//==================================
void MainController::buttonPressed(int button) {
  ButtonIDPool.write(button);
  ButtonPressedFlag.set();
}
void MainController::translateCmd(StructData data, int id) {
  if (id == keyboards::keypad) {
    CommandChannel.write(data);
  }
}

void MainController::main() {
  while (true) {
    switch (state) {
      case (states::WAIT_FOR_START_GAME): {
        wait(CommandChannel);
        if (startGame()) {
          count_down = 10;
          state = states::COUNTDOWN;
        }
        break;
      }  // case (states::WAIT_FOR_START_GAME)
      case (states::COUNTDOWN): {
        wait(PeriodFlag);
        count_down--;
        if (count_down <= 0) {
          state = states::WAIT_FOR_COMMAND;
        }
        break;
      }  // case (states::COUNTDOWN)
      case (states::WAIT_FOR_COMMAND): {
        auto event = wait(ShotTimer + BeenShotTimer + ButtonPressedFlag + BuzzerTimer + CommandChannel + PeriodFlag);
        if (event == ShotTimer) {
          shot = false;
        }
        if (event == BeenShotTimer) {
          been_shot = false;
        }
        if (event == ButtonPressedFlag) {
          if (ButtonIDPool.read() == buttons::shoot_trigger && (!shot)) {
            IrSend.sendMessage(info.getPlayerNumber(), info.getWeapon());
            shot = true;
            ShotTimer.set(timeWaitShot());
            buzzer.write(1);
            BuzzerTimer.set(100000);
          }
        }
        if (event == BuzzerTimer) {
          buzzer.write(0);
        }
        if (event == CommandChannel) {
          StructData command = CommandChannel.read();
          runCommand(command);
          if (info.getHealthPoints() <= 0) {
						info.setTime(0);
            state = states::WAIT_FOR_PC;
          }
        }
        if (event == PeriodFlag) {
          info.setTime(info.getTime() - 1);
          window.update(0);
          if (info.getTime() <= 0) {
            window.update(0);
            hwlib::wait_ms(350);
            state = states::WAIT_FOR_PC;
          }
        }
        break;
      }  // case (states::WAIT_FOR_COMMAND)
      case (states::WAIT_FOR_PC): {
        char keyboard_input;
        hwlib::cin >> keyboard_input;
        if (keyboard_input == 111) {
          cout << info;
          info.resetPlayer();
          for (int i = 0; i < 4; i++) {
            window.update(i);
          }
            state = states::WAIT_FOR_START_GAME;
          break;
        }  // case (states::WAIT_FOR_PC)
      }    // switch (state)
    }      // while (true)
  }
}