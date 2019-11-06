#include "KeyboardController.hpp"

//========================================
// PRIVATE FUNCTIONS KeyboardController
//========================================
void KeyboardController::ledOnOff() {
  led.write(1);
  led.flush();
  hwlib::wait_ms(100);
  led.write(0);
  led.flush();
}

//========================================
// PUBLIC FUNCTIONS KeyboardController
//========================================
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
          wait_ms(no_register_time);
          state = states::RECEIVING;
          break;
        } else if (input == '*') {
          strcpy(command, "*");
          ledOnOff();  // Als de input * is moet keyboardcontroller meteen naar SENDING state
          main_c.setCmd(command);
          wait_ms(no_register_time);
          state = states::SENDING;
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
          wait_ms(no_register_time);
          state = states::SENDING;
          break;
        } else if (input != '\0' && char_count < 16) {
          char_count++;
          ledOnOff();
          strncat(command, &input, 1);
          wait_ms(no_register_time);
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
          wait_ms(no_register_time);
        } else if (input == '*') {
          ledOnOff();
          strcpy(command, "*");
          main_c.setCmd(command);
          wait_ms(no_register_time);
        } else if (input == 'C') {
          char_count = 0;
          ledOnOff();
          strcpy(command, "Clear");
          main_c.setCmd(command);
          state = states::IDLE;
          wait_ms(no_register_time);
        }
        break;
      }  // states::SENDING
    }    // switch
  }
}