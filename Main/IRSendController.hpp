#ifndef IRSENDCONTROLLER_HPP
#define IRSENDCONTROLLER_HPP

#include "PinPWMD2.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

using hwlib::cout;
using hwlib::endl;
using hwlib::wait_us;

class IRSendController : rtos::task<> {
 private:
  PinPWMD2<38000, 2> ir_led;
  uint16_t message, mask;
  unsigned int mess_repeat;
  int bit_send;
  bool bit;
  bool sending = false;

  rtos::pool<uint8_t> PlayerPool;
  rtos::pool<uint8_t> DataPool;
  rtos::flag NewMessageFlag, RepeatFlag;
  rtos::timer SignalTimer;

  enum class states { WAIT_FOR_FLAG, TRANSMIT_MESSAGE };
  enum class states_transmit { HIGH_WAIT, LOW_WAIT };
  states state;
  states_transmit transmit_state;

  void generateMessage();

 public:
  void sendMessage(uint8_t player_num, uint8_t data);
  void repeatSend();

  IRSendController()
      : task("IR-SendController"),
        ir_led(PinPWMD2<38000, 2>()),
        PlayerPool("PlayerPool"),
        DataPool("DataPool"),
        NewMessageFlag(this, "NewMessageFlag"),
        RepeatFlag(this, "RepeatFlag"),
        SignalTimer(this, "SignalTimer") {
    ir_led.write(0);
  }

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

#endif  // IRSENDCONTROLLER_HPP