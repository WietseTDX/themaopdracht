#include "InputHandler.hpp"

//========================================
// PUBLIC FUNCTIONS InputHandler
//========================================
void InputHandler::addButton(Button *object) {
  button[place_b] = object;
  place_b++;
}

void InputHandler::addKeyboard(KeyboardController *object) {
  keyboard[place_k] = object;
  place_k++;
}

void InputHandler::main() {
  while (true) {
    wait(PeriodFlag);
    for (int i = 0; i < place_k; i++) {
      keyboard[i]->update();
    }
    for (int i = 0; i < place_b; i++) {
      button[i]->update();
    }
  }
}