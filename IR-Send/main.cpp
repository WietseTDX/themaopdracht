#include "senderTask.hpp"

class testMain : rtos::task<> {
 private:
  hwlib::pin_in& but;
  hwlib::pin_out& red_led;
  IRSendController& sender;

 public:
  testMain(hwlib::pin_in& b, hwlib::pin_out& l, IRSendController& s) : but(b), red_led(l), sender(s) {}
  void main() {
    for (;;) {
      but.refresh();
      if (!but.read()) {
        red_led.write(1);
        sender.sendMessage(20, 1);
      } else {
        red_led.write(0);
      }
			hwlib::wait_ms(100);
    }
  }
};

int main(void) {
  hwlib::wait_ms(500);
  // IR output LED
	auto but = hwlib::target::pin_in(hwlib::target::pins::d14);
	auto led = hwlib::target::pin_out(hwlib::target::pins::d21);
  auto test_send = IRSendController();
  auto gozer = testMain(but, led, test_send);

	rtos::run();
}
