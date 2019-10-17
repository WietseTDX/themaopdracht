#ifndef SENDERTASK_HPP
#define SENDERTASK_HPP

#include "customPWM.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

using hwlib::cout;
using hwlib::endl;

class IRSendController : rtos::task<> {
 private:
  PinPWMD2<38000, 2> ir_led;
  uint16_t message, mask;
  unsigned int mess_repeat;
  int bit_send;
  bool bit;

  rtos::pool<uint8_t> playerPool;
  rtos::pool<uint8_t> dataPool;
  rtos::flag newMessageFlag, repeatFlag;
  rtos::timer signalTimer;

  enum class states { WAIT_FOR_FLAG, SEND_MESSAGE, BIT_1_HIGH, BIT_1_LOW, BIT_0_HIGH, BIT_0_LOW };
  states state;
  void generateMessage();


 public:
  void sendMessage(uint8_t player_num, uint8_t data);
	void repeatSend();

  IRSendController()
      : task("IR-SendController"),
        ir_led(PinPWMD2<38000,2>()),
        playerPool("playerPool"),
        dataPool("dataPool"),
        newMessageFlag(this, "newMessageFlag"),
				repeatFlag(this, "repeatFlag"),
        signalTimer(this, "signalTimer") {}

  void main() override;

  template <int N, typename F>
  void messageBitPrinter() {
    cout << "bit representation: " << endl;
    for (int i = N - 1; i >= 0; i--) {
      bool p = message & (0x01 << i);
      cout << p;
      if (i % 4 == 0) cout << ' ';
    }
    cout << endl;
  }
};

#endif