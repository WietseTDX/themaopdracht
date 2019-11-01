#include "hwlib.hpp"
#include "rtos.hpp"


class ButtonHandeler : public rtos::task<> {
  KeyboardControl &keyboard;
  Button *button[];
  rtos::clock PeriodFlag;
  int place = 0;

  public:
  ButthonHandeler();
  void main() override;
  void addButton(Button *object);
}