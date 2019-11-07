#ifndef IRRECIEVECONTROLLER_HPP
#define IRRECIEVECONTROLLER_HPP

#include "MainController.hpp"

using hwlib::cout;
using hwlib::endl;

///@file

///\brief
/// IRReceiveController
///\details
/// This class implements the IR-protocol (receive) given in the casus lasertag.
/// If a message is received it passes the message to MainController 
class IRReceiveController : public rtos::task<> {
 private:
  enum states { IDLE, RECEIVING };
  states state = IDLE;
  due::pin_in sensor;
  MainController &main_c;
  bool signal;
  bool signal_high = false;
  bool high_check = false;
  bool halfway = false;
  int start_high;
  int start_low;
  int resettime;
  int offset = 200;
  int one_high_time = 1600;
  int zero_high_time = 800;
  int bitcount = 0;
  int high_time = 0;

  int first_low_time;
  int first_high_end;

  uint16_t lastmessage;
  uint16_t checkmessage;
  uint8_t lastxor;
  uint8_t checkxor;
  uint8_t player;
  uint8_t data;
  uint8_t last_mathxor;
  uint8_t check_mathxor;

  ///\brief
  /// sendCommand
  ///\details
  /// this function generates a uint16_t that is send to MainController
  void sendCommand();

  ///\brief
  /// messageDecode
  ///\details
  /// This function gets the player and data from a uint16_t
  void messageDecode(uint16_t &to_decode);

  ///\brief
  /// checkingMessage
  ///\details
  /// This function checks if the message is correct.
	/// It checks the repeat and xors
  void checkingMessage();

 public:
  ///\brief
  /// Constructor IRReceiveController
  ///\details
  /// This function returns a IRReceiveController
  IRReceiveController(due::pins sensor, MainController &main_c)
      : task(2, "Receiver"), sensor(due::pin_in(sensor)), main_c(main_c){hwlib::cout << "Ik ben in klasse IRReceive\n";};

  ///\brief
  /// main rtos
  ///\details
  /// This function contains all the logic for the IRReceiveController
  void main();
};

#endif  // IRRECIEVECONTROLLER_HPP