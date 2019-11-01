#include "KeybaordControl.hpp"

void KeyboardController::update() {
  char key = keypad.pressed();
  if (key != '\0') {
    if (key != '#') {
      current_cmd[place] = key;
      place++;
    } else {
      if (current_cmd[3] > 9 || place > 3) {
        return;
      }
      if (current_cmd[0] == 'C') {
        if (current_cmd[1] == 'A') {
          mainC.translateCommand(StructData(1, current_cmd[2]));
        } else if (current_cmd[1] == 'B') {
          mainC.translateCommand(StructData(3, current_cmd[2]));
        }
      }
    }
    
  }
}