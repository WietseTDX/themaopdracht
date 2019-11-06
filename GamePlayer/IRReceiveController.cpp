#include "IRReceiveController.hpp"

//==================================
// PRIVATE FUNCTIONS IrReceiveController
//==================================
void IRReceiveController::sendCommand() {
  uint16_t to_send = 0x0;
  to_send |= (player << 5);
  to_send |= data;
  main_c.translateCmd(StructData(2, to_send));
};

void IRReceiveController::messageDecode(uint16_t &to_decode) {
  uint16_t message = (to_decode >> 5);
  data = (message & 0x001F);
  message = (message >> 5);
  player = (message & 0x001F);
}

void IRReceiveController::checkingMessage() {
  if (lastmessage == checkmessage) {
    uint16_t shiftcheck = checkmessage;
    messageDecode(shiftcheck);  // set data and player using the message given in the parameter
    sendCommand();
  } else {
    uint16_t shiftcheck = checkmessage;
    uint16_t shiftlast = lastmessage;

    messageDecode(shiftcheck);
    check_mathxor = (data ^ player);

    messageDecode(shiftlast);
    last_mathxor = (data ^ player);

    lastxor = (lastmessage & 0x001F);
    checkxor = (checkmessage & 0x001F);
    if (last_mathxor == check_mathxor) {
      sendCommand();
    } else {
      if (lastxor == last_mathxor) {
        shiftlast = lastmessage;
        messageDecode(shiftlast);
        sendCommand();
      } else if (checkxor == check_mathxor) {
        shiftcheck = checkmessage;
        messageDecode(shiftcheck);
        sendCommand();
      }
    }
  }
};

//==================================
// PUBLIC FUNCTIONS IrReceiveController
//==================================
void IRReceiveController::main() {
  state = states::IDLE;
  hwlib::wait_ms(100);
  for (;;) {
    switch (state) {
      case IDLE:
        hwlib::wait_us(10);
        sensor.refresh();
        signal = !sensor.read();

        if (signal && !signal_high) {
          start_high = hwlib::now_us();
          signal_high = true;
        } else if (!signal && signal_high) {
          first_high_end = hwlib::now_us();
          start_low = first_high_end;
          high_time = first_high_end - start_high;
          signal_high = false;
          if (high_time > 300) {
            halfway = false;
            lastmessage = 0;
            bitcount = 0;
            first_low_time = 0;
            resettime = 0;
            state = states::RECEIVING;
          }
        }
        break;

      case RECEIVING:
        hwlib::wait_us(10);
        sensor.refresh();
        signal = !sensor.read();

        if (bitcount == 16 && !halfway) {
          checkmessage = lastmessage;
          lastmessage = 0;
          halfway = true;
          signal_high = false;
          hwlib::wait_us(3'000);
        }
        if (bitcount == 32) {
          checkingMessage();
          high_time = 0;
          signal_high = false;
          state = states::IDLE;
          break;
        }

        if (signal && !signal_high) {
          start_high = hwlib::now_us();
          if (bitcount == 0) {
            first_low_time = hwlib::now_us() - first_high_end;
            if (first_low_time > 700 && first_low_time < zero_high_time + offset) {
              lastmessage |= 1;
              bitcount++;
            } else {
              high_time = 0;
              signal_high = false;
              state = states::IDLE;
              break;
            }
          }
          signal_high = true;

        } else if (!signal) {
          if (signal_high) {
            start_low = hwlib::now_us();
            high_time = start_low - start_high;
            if (high_time > 700 && high_time < zero_high_time + offset) {
              lastmessage = (lastmessage << 1);
            } else if (high_time > 1500 && high_time < one_high_time + offset) {
              lastmessage = (lastmessage << 1);
              lastmessage |= 1;
            } else {
              high_time = 0;
              signal_high = false;
              state = states::IDLE;
              break;
            }
            signal_high = false;
            bitcount++;
          } else if (bitcount != 16) {
            resettime = hwlib::now_us() - start_low;
            if (resettime > 4000) {
              high_time = 0;
              signal_high = false;
              state = states::IDLE;
              break;
            }
          }
        }
        break;
    }
  };
};