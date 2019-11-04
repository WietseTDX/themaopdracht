#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeyboardControl.hpp"
#include "Button.hpp"


class ButtonHandeler : public rtos::task<> {
  KeyboardControl &keyboard;
  Button *button[];
  rtos::clock PeriodFlag;
  int place = 0;

  public:
  /// \brief
  /// Constructor
  /// \details
  /// initiate all the rots objects
  ButthonHandeler();

  /// \brief
  /// The main function
  /// \details
  /// The main functions of the code with all the states
  void main() override;

  /// \brief
  /// Add a button object
  /// \details
  /// Add a button to update every period
  void addButton(Button *object);
}