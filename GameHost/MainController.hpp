#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <stdio.h>
#include <string.h>
#include "GameRules.hpp"
#include "../Main/IRSendController.hpp"
#include "WindowController.hpp"

///\brief
/// MainController
///\details
/// This class creates an MainController object.
/// This class implements the GameHost logic
class MainController : rtos::task<> {
 private:
  rtos::flag CommandFlag;
  rtos::pool<char*> CommandPool;
  uint8_t data;
  char command[10];
  char set_command[10];
  bool first_send;
  WindowStruct screen_data;
  WindowController window;
  IRSendController ir_sender;
  GameRules game_rules;
  enum states { IDLE, COMMAND_SET };
  states state = IDLE;

  ///\brief
  /// charsToInt
  ///\details
  /// the function returns a int based on the two chars
  /// char left is multiplied with 10, char right is added to the answer of the multiplication
  int charsToInt(char left, char right);

  /// \brief
  /// A private function that is used to decode and check a command to see if it is valid.
  void verifyCommand();

  /// \brief
  /// À private function to check which command has to be sent
  void commandActions();

 public:
 /// \brief
  /// Constructor MainController
  /// \details
  /// This function creates a MainController object.\n
  /// The constructor expects a hwlib::window& and a hwlib::font&
  /// Call Example: auto sender = IRSendController();
  MainController(hwlib::window& w, const hwlib::font& f)
      : CommandFlag(this, "CommandFlag"),
        CommandPool("CommandPool"),
        screen_data(),
        window(w, f),
        ir_sender(),
        game_rules() {
    screen_data.type = 2;
    window.update(screen_data);
  }

  /// \brief
  /// public function to be used by other rtos tasks to pass a function parameter
  /// \details
  /// setCmd can be used by other tasks to send a command to MainController so it can be processed and send(if possible)
  /// from MainController.
  void setCmd(char* command);

  /// \brief
  /// MainController's main loop
  void main();
};

#endif  // MAINCONTROLLER_HPP
