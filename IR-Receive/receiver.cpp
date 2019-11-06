#include "receiver.hpp"

//==================================
// Private functions "IrReceiveController"
//==================================

void IrReceiveController::sendCommand() {
  uint16_t to_send = 0x0;
  to_send |= (player << 5);
  to_send |= data;
  // dataStruct.data = to_send;
  // main_c.translateCmd(dataStruct);
  cout << "total: " << to_send << " player: " << player << " data: " << data << endl;
  // messageBitPrinter<16, uint16_t>(to_send);
};

void IrReceiveController::messageDecode(uint16_t &to_decode) {
  uint16_t message = (to_decode >> 5);
  data = (message & 0x001F);
  message = (message >> 5);
  player = (message & 0x001F);
}

void IrReceiveController::checkingMessage() {
  if (lastmessage == checkmessage) {
    cout << "check: 1" << endl;
    uint16_t shiftcheck = checkmessage;
    messageDecode(shiftcheck);  // set data and player using the message given in the parameter
    sendCommand();
  } else {
    uint16_t shiftcheck = checkmessage;
    uint16_t shiftlast = lastmessage;

    messageDecode(shiftcheck);
    checkMathxor = (data ^ player);

    messageDecode(shiftlast);
    lastMathxor = (data ^ player);

    lastxor = (lastmessage & 0x001F);
    checkxor = (checkmessage & 0x001F);
    if (lastMathxor == checkMathxor) {
      cout << "check: 2" << endl;
      sendCommand();
    } else {
      if (lastxor == lastMathxor) {
        shiftlast = lastmessage;
        messageDecode(shiftlast);
        cout << "check: 3" << endl;
        sendCommand();
      } else if (checkxor == checkMathxor) {
        shiftcheck = checkmessage;
        messageDecode(shiftcheck);
        cout << "check: 3" << endl;
        sendCommand();
      }
    }
  }
};

//==================================
// Public funtions "IrReceiveController"
//==================================

void IrReceiveController::main() {
  state = states::IDLE;
  hwlib::wait_ms(100);
  cout << "START" << endl;
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
          hwlib::wait_us(3000);
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
          } else if(bitcount != 16){
            uint64_t time_now = hwlib::now_us();
            resettime = time_now - start_low;
            if (resettime > 4000+offset) {
              cout << "reset bitch: " << resettime << '-' << bitcount << endl;
              cout << "now: " << time_now << " reset: " << start_low << endl;
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