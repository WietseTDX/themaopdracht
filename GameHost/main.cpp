#include "WindowController.hpp"

class testMain : rtos::task<> {
 private:
  WindowController& screen;
  hwlib::pin_in& button;
  WindowStruct update_struct;
  int upd = 0;

 public:
  testMain(WindowController& w, hwlib::pin_in& b) : task("TestMain"), screen(w), button(b), update_struct{0, "C0xA"} {}

  void main() override {
    auto testScreen0 = WindowStruct{0, "C0xA"};
    auto testScreen1 = WindowStruct{1, {'*'}};
    auto testScreen2 = WindowStruct{2, "Nothing"};
    screen.updateScreen(WindowStruct{2,""});
		screen.updateScreen(WindowStruct{2,""});
    for (;;) {
      hwlib::wait_ms(100);
      button.refresh();
      if (!button.read()) {
        if (upd == 0) {
          upd++;
          update_struct = testScreen1;
        } else if (upd == 1) {
          upd++;
          update_struct = testScreen2;
        } else if (upd == 2) {
          upd = 0;
          update_struct = testScreen0;
        }
        screen.updateScreen(update_struct);
      }
    }
  }
};

int main() {
  font_default_8x8 f;
  xy zero(0, 0);

  hwlib::target::pin_oc scl = hwlib::target::pin_oc{hwlib::target::pins::scl};
  hwlib::target::pin_oc sda = hwlib::target::pin_oc{hwlib::target::pins::sda};
  hwlib::target::pin_in button = hwlib::target::pin_in{hwlib::target::pins::d14};
  hwlib::i2c_bus_bit_banged_scl_sda i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
  hwlib::glcd_oled_i2c_128x64_buffered oled = hwlib::glcd_oled_i2c_128x64_buffered(i2c_bus);
  oled.clear();

  auto oled_controller = WindowController(oled, f);
  auto main_test = testMain(oled_controller, button);
  rtos::run();
}