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

///@file

/// \brief
/// Struct window information
/// \details
/// This struct is used by Maincontroller to communicate with WindowController.
/// The struct contains a uint8_t type and a char array to_show.
/// The variable type, is used to specify which screen should be shown. It has the following lay-out:
/// -0: Bad command screen
/// -1: Good command screen
/// -2: Clear screen
/// -3: Good command 'with push *' scren
struct WindowStruct {
  uint8_t type;
  char to_show[16];  // screen is 128 pixels wide so 16*8 = 128
};

/// \brief
/// WindowController GameHost
/// \details
/// This class implements a simple interface to update a hwlib::window.
/// The class inheritances from rtos::task<>. This makes it possible to run this class/ task among side other tasks.
/// The class has 4 basic layouts for the window. 
/// The window is changed based on a parameter given in the update function.
/// It is advised to use this class in conjection with a hwlib::glcd_oled window (or a variation on this).
/// The reason for this is, that the textblocks are made for a window with a size of 128 by 64.
class WindowController : rtos::task<> {
 private:
  window& oled;
  WindowStruct info;
  window_part_t block_top, block_mid, block_bot;
  terminal_from text_top, text_mid, text_bot;

  rtos::pool<WindowStruct> WindowPool;
  rtos::flag UpdateFlag;

  uint8_t current_screen = 0;

  enum class states { WAIT_FOR_UPDATE, BAD_CMD, GOOD_CMD, CLEAR_CMD };
  states state;

 public:
  /// \brief
  /// Constructor WindowController
  /// \details
  /// The constructor creates a WindowController object.\n
  /// You are obligated to give a hwlib::window and a hwlib::font.\n
  /// Call example: auto screen = WindowController(oled, font_default_8x8)
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

  /// \brief
  /// Update WindowController
  /// \details
  /// This function updates the window.
  /// The update is based on the new_info parameter.
	/// The behavior for how the window will look like is done in the function main. 
  /// Call
  void update(const WindowStruct& new_info);

  /// \brief
  /// Implementaion main WindowController
  /// \details
  /// The function main() is inherited from rtos::task<>.
  /// The main function says what the class will do in it possible states.
  /// The function contains all the behavior of the WindowController.
  void main() override;
};

#endif  // WINDOWCONTROLLER_HPP