#include "hwlib.hpp"
#include "rtos.hpp"


class ButthonHandeler : public rtos::task<> {
  KeyboardControl &keyboard;
  Button *button[1];
  int place = 0;

  public:
  void main() override;
  void addButton(Button *object);
}