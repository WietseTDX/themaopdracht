#include "KeyboardControl.hpp"

void KeyboardController::update() {
  char key = keypad.pressed();
  if (key != '\0') {
    if (key != '#') {
      if (place == 0) {
        current_cmd[place] = key;
        place++;
      } else if (key != current_cmd[place - 1]) {
        if (place >= 3) {
          return;
        }
        current_cmd[place] = key;
        place++;
      }
    } else {
      place = 0;
      int empty = 0;
      for (int i=0; i<3; i++) {
        if (!current_cmd[i]) {
          empty++;
        }
      }
      if (empty == 3) {
        return;
      }
      int number = current_cmd[2] - '0';
      if (current_cmd[0] == 'C') {
        if (number > 9 || number < 0) {
          for (int i=0; i<3; i++) {
            current_cmd[i] = 0;
          }
          return;
        } else if (current_cmd[1] == 'A') {
          for (int i=0; i<index_listener; i++) {
            keyboard_listener[i]->translateCmd(StructData(1, number), id);
          }
        } else if (current_cmd[1] == 'B') {
          for (int i=0; i<index_listener; i++) {
            keyboard_listener[i]->translateCmd(StructData(3, number), id);
          }
        }
      }
      for (int i=0; i<3; i++) {
        current_cmd[i] = 0;
      }
    } 
  }
}

void KeyboardController::addKeyboardListener(KeyboardListener *object) {
  keyboard_listener[index_listener] = object;
  index_listener++;
}