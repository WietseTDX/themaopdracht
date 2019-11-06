#include "MainController.hpp"

//========================================
// PRIVATE FUNCTIONS MainController
//========================================
int MainController::charsToInt(char left, char right) {
  int l = int(left - '0') * 10;
  int r = int(right - '0');
  return l + r;
}

void MainController::verifyCommand() {
  strcpy(screen_data.to_show, command);
  if (command[1] == 'A') {
    int minutes = charsToInt(command[2], command[3]);
    if (minutes > 0 && minutes < 16) {
      screen_data.type = 1;
      data = minutes;
      data += 16;  // data should look like 1xxxx with time. xxxx is the minutes
    } else {
      screen_data.type = 0;
    }
  } else if (command[0] == '*') {
    screen_data.type = 3;
    data = 0;
    strcpy(screen_data.to_show, "Start command");
  } else {
    screen_data.type = 0;
  }
}

void MainController::commandActions() {
  if (screen_data.type == 1 || screen_data.type == 3) {
    if (set_command[0] == '*') {
      if (first_send) {
        ir_sender.sendMessage(0, data);
        first_send = false;
      } else {
        ir_sender.repeatSend();
      }
    } else if (set_command[0] == '#') {
      if (first_send) {
        ir_sender.sendMessage(0, data);
        first_send = false;
      } else {
        ir_sender.repeatSend();
      }
    }  // command[0] == #
  }    // type == 1 || type == 3
}

//========================================
// PUBLIC FUNCTIONS MainController
//========================================
void MainController::setCmd(char* command) {
  CommandPool.write(command);
  CommandFlag.set();
}

void MainController::main() {
  state = states::IDLE;
  for (;;) {
    switch (state) {
      case states::IDLE: {
        wait(CommandFlag);
        strcpy(command, CommandPool.read());
        verifyCommand();
        first_send = true;
        state = states::COMMAND_SET;
        break;
      }  // states::IDLE
      case states::COMMAND_SET: {
        window.update(screen_data);
        wait(CommandFlag);
        strcpy(set_command, CommandPool.read());
        if (set_command[0] == 'C') {  // Clear screen
          screen_data.type = 2;
          window.update(screen_data);
          state = states::IDLE;
        } else {
          commandActions();
        }
        break;
      }  // states::COMMAND_SET
    }    // switch
  }
}