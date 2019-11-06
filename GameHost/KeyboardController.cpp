#include "KeyboardController.hpp"

void KeyboardController::ledOnOff() {
  led.write(1);
  led.flush();
  hwlib::wait_ms(100);
  led.write(0);
  led.flush();
}

/// \brief
/// KeyboardController's main loop
/// \details
/// Every registered keypress activates an LED to show the user the keypress was registered.
void KeyboardController::main() {
  state = states::IDLE;
  for (;;) {
    switch (state) {
      case states::IDLE: {
        wait(InputClock);
        input = keypad.pressed();
        if (input == 'C') {  // Als de input C is moet keyboardcontroller door naar de RECEIVING state
          char_count++;
          strcpy(command, "C");
          ledOnOff();
          state = states::RECEIVING;
          hwlib::wait_us(200'000);
          break;
        } else if (input == '*') {
          strcpy(command, "*");
          ledOnOff();  // Als de input * is meot keyboardcontroller meteen het start command senden
          main_c.setCmd(command);
          state = states::SENDING;
          hwlib::wait_us(200'000);
          break;
        }
        break;
      }  // states::IDLE
      case states::RECEIVING: {
        wait(InputClock);
        input = keypad.pressed();
        if (input == '#') {  // zolang de input geen # is moet de input aan het command anders moet keyboardcontroller
                             // naar de sending state
          ledOnOff();
          main_c.setCmd(command);
          state = states::SENDING;
          hwlib::wait_us(200'000);
          break;
        } else if (input != '\0' && char_count < 16) {
          char_count++;
          ledOnOff();
          strncat(command, &input, 1);
          hwlib::wait_us(200'000);
          break;
        }
        break;
      }  // states::RECEIVING
      case states::SENDING: {
        wait(InputClock);
        input = keypad.pressed();
        if (input == '#') {
          ledOnOff();
          strcpy(command, "#");
          main_c.setCmd(command);
          hwlib::wait_us(200'000);
        } else if (input == '*') {
          ledOnOff();
          strcpy(command, "*");
          main_c.setCmd(command);
          hwlib::wait_us(200'000);
        } else if (input == 'C') {
          char_count = 0;
          ledOnOff();
          strcpy(command, "Clear");
          main_c.setCmd(command);
          state = states::IDLE;
          hwlib::wait_us(200'000);
        }
        break;
      }  // states::SENDING
    }    // switch
  }
}