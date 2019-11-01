#include "ButtonHandeler.hpp"

ButtonHandeler::Buttonhandeler() : task("ButtonHandeler"), PeriodFlag(this, 100000, "PeriodFlag") {}

void ButtonHandeler::main() {
  while (true) {
    auto event = wait(PeriodFlag);
    keyboard.update();
    for (int i=0; i<place; i++) {
      button[i]->update();
    }
  }
}


void ButtonHandeler::addButton(Button *object) {
  button[place] = object;
  place++;
}