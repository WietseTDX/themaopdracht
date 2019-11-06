#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeyboardController.hpp"
#include "Button.hpp"


class InputHandler : public rtos::task<> {
  rtos::clock PeriodFlag;
  int place_b = 0;
  int place_k = 0;
  KeyboardController *keyboard[2];
  Button *button[2];

  public:
  /// \brief
  /// Constructor
  /// \details
  /// initiate all the rots objects
  InputHandler() : task("InputHandler"), PeriodFlag(this, 100000, "PeriodFlag") {}

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

  /// \brief
  /// Add a KeyboardController object 
  /// \details
  /// Add a Keyboar to the update cycle
  void addKeyboard(KeyboardController *object);
};  

#endif // INPUTHANDLER_HPP