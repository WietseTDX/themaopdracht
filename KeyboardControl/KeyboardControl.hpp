#include "hwlib.hpp"
#include <string>

class KeyboardController {
  hwlib::keypad keypad;
  MainController& main_c;
  std::string current_cmd;
  int place;
  public:
  void update();
}