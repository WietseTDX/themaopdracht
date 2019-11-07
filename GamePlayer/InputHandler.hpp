#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include "Button.hpp"
#include "KeyboardController.hpp"

///@file

///\brief
/// InputHandler
///\details
/// rtos task that calls the update function on all input objects
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
  /// initiate all the rtos objects
  InputHandler() : task(500,"InputHandler"), PeriodFlag(this, 100000, "PeriodFlag") {hwlib::cout << "Ik ben in klasse InputHandler\n";}

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

  /// \brief
  /// The main function
  /// \details
  /// The main functions of the code with all the states
  void main() override;
};

#endif  // INPUTHANDLER_HPP