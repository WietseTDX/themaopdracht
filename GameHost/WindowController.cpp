#include "WindowController.hpp"

void WindowController::main() {
  state = states::WAIT_FOR_UPDATE;
  for (;;) {
    switch (state) {
      case states::WAIT_FOR_UPDATE: {
        wait(UpdateFlag);
        info = WindowPool.read();
        if (info.type == 0) {
          state = states::BAD_CMD;
        } else if (info.type == 1) {
          state = states::GOOD_CMD;
        } else if (info.type == 2) {
          state = states::CLEAR_CMD;
        } else {
          cout << "DEBUG: info.type not correct" << endl;
          cout << "info.type = " << info.type << endl;
          cout << "info.to_show = " << info.to_show << endl;
        }
        break;
      }  // case states::WAIT_FOR_UPDATE
      case states::BAD_CMD: {
        if (current_screen != 0) {
          current_screen = 0;
          block_top.clear();
          block_mid.clear();
          block_bot.clear();
          text_top << "\f"
                   << "unkown command:";
          text_mid << "\f" << info.to_show;
          text_bot << "\f"
                   << "enter new cmd";
          oled.flush();
        } else {
          block_mid.clear();
          text_mid << info.to_show;
          block_mid.flush();
        }
        state = states::WAIT_FOR_UPDATE;
        break;
      }  // case states::BAD_CMD
      case states::GOOD_CMD: {
        if (current_screen != 1) {
          current_screen = 1;
          block_top.clear();
          text_top << "\f"
                   << "good command:";
        }
        block_mid.clear();
        block_bot.clear();
        text_mid << "\f" << info.to_show;
        if (info.to_show != comp) {
          text_bot << "\f"
                   << "press # to send";
        } else {
          text_bot << "\f"
                   << "press * to send";
        }
        oled.flush();
        state = states::WAIT_FOR_UPDATE;
        break;
      }  // case states::GOOD_CMD
      case states::CLEAR_CMD: {
        if (current_screen != 2) {
          current_screen = 2;
          block_top.clear();
          block_mid.clear();
          block_bot.clear();
          text_top << "\f"
                   << "Command cleared";
          text_mid << "\f"
                   << "enter new";
          text_bot << "\f"
                   << "command";
          oled.flush();
        }
          state = states::WAIT_FOR_UPDATE;
        break;
      }  // case states::CLEAR_CMD
    }    // switch state
  }      // for(;;)
}  // main()

void WindowController::updateScreen(const WindowStruct& new_info) {
  UpdateFlag.set();
  WindowPool.write(new_info);
}  // update(const WindowStruct& new_info)