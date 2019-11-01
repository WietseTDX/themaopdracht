#include "hwlib.hpp"
#include "rtos.hpp"

template<int count_button>
class ButthonHandeler : public rtos::task<> {
  KeyboardControl &keyboard;
  Button *button[count_button];
  int place = 0;

  public:
  void main() override;
  void addButton(Button button);
}