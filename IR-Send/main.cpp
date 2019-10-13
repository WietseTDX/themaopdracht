#include "sender.hpp"

int main(void) {
  // IR output LED
  auto ir = hwlib::target::d2_36kHz();
  auto test_send = sender(ir);

  uint16_t mess = test_send.generateMessage(20, 1);
  test_send.bitPrinter<16, uint16_t>(mess);
	test_send.sendMessage(mess);

}
