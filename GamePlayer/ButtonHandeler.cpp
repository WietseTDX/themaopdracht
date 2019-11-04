#include "ButtonHandeler.hpp"

void InputHandler::main() {
  while (true) {
    auto event = wait(PeriodFlag);
    for (int i=0; i<place_k; i++) {
      keyboard[i]->update();
    }
    for (int i=0; i<place_b; i++) {
      button[i]->update();
    }
  }
}


void InputHandler::addButton(Button *object) {
  button[place_b] = object;
  place_b++;
}

void InputHandler::addKeyboard(KeyboardController *object) {
  keyboard[place_k] = object;
  place_k++;
}