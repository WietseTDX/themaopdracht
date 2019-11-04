#include "MainController.hpp"

void MainController::setCmd(char* command){
    CommandFlag.set();
    CommandPool.write(command);
}

int MainController::charsToInt(char left, char right){
    int l = int(left-'0')*10;
    int r = int(right-'0');
    return l+r;
}

void MainController::verifyCommand(){
    screen_data.to_show = command;
    if(command[1] == 'A'){
      int minutes = charsToInt(command[2], command[3]);
      if(minutes > 0 && minutes << 16){
        screen_data.int = 1;
        data = minutes;
      } else{
        screen_data.int = 0;
      }
    } else if(command == "*"){
      screen_data.int = 3;
      data = 0;
      screen_data.to_show = "Start command"
    } else{
      screen_data.int = 0;
    }
}

void MainController::commandActions(){
  if(screen_data.type ==1){
    if(set_cmd == "*" && str.cmd == start){
      if(first_send) {
        IRSend.sendMessage();
        first_send = true;
      }else {
        IRSend.repeatSend();
      }
    }
    else if(set_cmd == "#" && str.cmd != start){
      if(first_send) {
        IRSend.sendMessage();
        first_send = true;
      }else {
        IRSend.repeatSend();
      }
    }
  }
}

void MainController::main(){
    state = states::IDLE;
    for(;;){
        switch(state){
            case states::IDLE:
                {
                    wait( CommandFlag );
                    strcpy(command, CommandPool.read());
                    verifyCommand();
                    state = states::COMMAND_SET;
                    break;
                } // states::IDLE

            case states::COMMAND_SET:
                {
                    window.update(screen_data);
                    wait( CommandFlag );
                    strcpy(set_command, CommandPool.read());
                    if(set_command == "C"){
                        screen_data.type = 0;
                        window.update(screen_data);
                        first_send = false;
                        state = states::IDLE;
                    } else {
                        commandActions();
                    }
                    break;
                } // states::COMMAND_SET

        } // switch
    }
}