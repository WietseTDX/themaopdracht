#ifndef IRRECIEVECONTROLLER_HPP
#define IRRECIEVECONTROLLER_HPP

#include "MainController.hpp"

using hwlib::cout;
using hwlib::endl;

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

  void sendCommand();

  void messageDecode(uint16_t &to_decode);

  void checkingMessage();

  template <int N, typename F>
  void messageBitPrinter(F printing) {
    cout << "bit representation: " << endl;
    for (int i = N - 1; i >= 0; i--) {
      bool p = printing & (0x01 << i);
      cout << p;
      if (i % 4 == 0) cout << ' ';
    }
    cout << endl;
  }

 public:
  IRReceiveController(due::pins sensor, MainController & main_c) : task("Receiver"), sensor(due::pin_in(sensor)), main_c(main_c){};

  void main();
};

#endif  // IRRECIEVECONTROLLER_HPP