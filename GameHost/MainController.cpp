#include "MainController.hpp"

/// \brief
/// public function to be used by other rtos tasks to pass a function parameter
/// \details
/// setCmd can be used by other tasks to send a command to MainController so it can be processed and sent(if possible) from MainController.
void MainController::setCmd(char* command){
    CommandFlag.set();
    CommandPool.write(command);
}

/// \brief
/// A private function to convert a char command to an integer value
int MainController::charsToInt(char left, char right){
    int l = int(left-'0')*10;
    int r = int(right-'0');
    return l+r;
}

/// \brief
/// A private function that is used to decode and check a command to see if it is valid
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

/// \brief
/// À private function to check which command has to be sent
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

/// \brief
/// MainController's main loop
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