#ifndef WINDOWCONTROLLER_HPP
#define WINDOWCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

using hwlib::cout;
using hwlib::endl;
using hwlib::font_default_8x8;
using hwlib::terminal_from;
using hwlib::window;
using hwlib::window_part_t;
using hwlib::xy;
using std::string;

struct WindowStruct {
  uint8_t type;
  char to_show[8];  // screen is 64 pixels wide
};

class WindowController : rtos::task<> {
 private:
  window& oled;
  WindowStruct info;
  window_part_t block_top, block_mid, block_bot;
  terminal_from text_top, text_mid, text_bot;

  rtos::pool<WindowStruct> WindowPool;
  rtos::flag UpdateFlag;

  uint8_t current_screen = 0;
	char comp[8] = {'*'};

  enum class states { WAIT_FOR_UPDATE, BAD_CMD, GOOD_CMD, CLEAR_CMD };
  states state;

 public:
  WindowController(window& window, const hwlib::font& f)
      : task("WindowController"),
        oled(window),
        block_top(window, xy(0, 0), xy(128, 20)),
        block_mid(window, xy(0, 21), xy(128, 20)),
        block_bot(window, xy(0, 42), xy(128, 20)),
        text_top(block_top, f),
        text_mid(block_mid, f),
        text_bot(block_bot, f),
        WindowPool("WindowPool"),
        UpdateFlag(this, "UpdateFlag") {}

  void updateScreen(const WindowStruct& new_info);
  void main() override;
};

#endif  // WINDOWCONTROLLER_HPP