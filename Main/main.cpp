#include "IRSendController.hpp"

class TestTrans : rtos::task<> {
 private:
  PinPWMD2<38000, 2> ir_led;

 public:
  TestTrans() : task("Test transistor"), ir_led(PinPWMD2<38000, 2>()) {}

  void main() override {
    for (;;) ir_led.write(1);
  }
};

class testMain : rtos::task<> {
 private:
  hwlib::pin_in& but;
  hwlib::pin_out& red_led;
  IRSendController& sender;

 public:
  testMain(hwlib::pin_in& b, hwlib::pin_out& l, IRSendController& s) : but(b), red_led(l), sender(s) {}
  void main() {
        sender.sendMessage(20, 1);
    for (;;) {
      but.refresh();
      if (!but.read()) {
        red_led.write(1);
        sender.repeatSend();
      } else {
        red_led.write(0);
      }
      hwlib::wait_ms(100);
    }
    // for (;;) {
    //   but.refresh();
    //   if (!but.read()) {
    //     sender.sendMessage(1, 1);
    //     for (int i = 0; i < 600; i++) {
    //       red_led.write(1);
    //       hwlib::wait_ms(125);
    //       sender.repeatSend();
    //       red_led.write(0);
    //       if (i % 10 == 0) hwlib::cout << i << endl;
    //       hwlib::wait_ms(125);
    //     }
    //   }
    //   hwlib::wait_ms(100);
    // }
  }
};

int main(void) {
  hwlib::wait_ms(500);
  // IR output LED
  auto but = hwlib::target::pin_in(hwlib::target::pins::d14);
  auto led = hwlib::target::pin_out(hwlib::target::pins::d21);
  auto test_send = IRSendController();
  auto gozer = testMain(but, led, test_send);

  // auto trans = TestTrans();

  rtos::run();
}
