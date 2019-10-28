#include "receiver.hpp"
using hwlib::cout;
using hwlib::endl;
//==================================
// Private functions "IrReceive"
//==================================

void IrReceive::printMessage() {
  cout << "player: " << player << endl;
  cout << "data: " << data << endl;
};

//==================================
// Public funtions "IrReceive"
//==================================

void IrReceive::main() {
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
          state = states::CHECKING;
          break;
        }

        if (signal && !signal_high) {
          start_high = hwlib::now_us();
          if (bitcount == 0) {
            first_low_time = hwlib::now_us() - first_high_end;
            if (first_low_time > 700 && first_low_time < 900) {
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
            if (high_time > 700 && high_time < 900) {
              lastmessage = (lastmessage << 1);
            } else if (high_time > 1500 && high_time < 1700) {
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
          }

          else {
            resettime = hwlib::now_us() - start_low;
            if (resettime >= 4000) {
              cout << "reset";
              first_low_time = 0;
              high_time = 0;
              bitcount = 0;
              signal_high = false;
              resettime = 0;
              state = states::IDLE;
              break;
            }
          }
        }
        break;

      case CHECKING:
        cout << "last: " << lastmessage << endl;
        cout << "check: " << checkmessage << endl;
        if (lastmessage == checkmessage) {
          uint16_t shiftcheck = checkmessage;
          uint8_t message = (shiftcheck >> 10);
          player = (message & 0x001F);
          shiftcheck = checkmessage;
          message = (shiftcheck >> 5);
          data = (message & 0x001F);
          cout << "1" << endl;
          printMessage();
        } else {
          uint16_t shiftcheck = checkmessage;
          uint16_t shiftlast = lastmessage;

          uint8_t message = (shiftcheck >> 10);
          player = (message & 0x001F);
          shiftcheck = checkmessage;
          message = (shiftcheck >> 5);
          data = (message & 0x001F);
          checkMathxor = (data ^ player);

          message = (shiftlast >> 10);
          player = (message & 0x001F);
          shiftlast = lastmessage;
          message = (shiftlast >> 5);
          data = (message & 0x001F);

          lastMathxor = (data ^ player);

          lastxor = (lastmessage & 0x001F);
          checkxor = (checkmessage & 0x001F);
          if (lastMathxor == checkMathxor) {
            cout << "2" << endl;
            printMessage();
          } else {
            if (lastxor == lastMathxor) {
              shiftlast = lastmessage;
              message = (shiftlast >> 10);
              player = (message & 0x001F);
              shiftlast = lastmessage;
              message = (shiftlast >> 5);
              data = (message & 0x001F);
            } else if (checkxor == checkMathxor) {
              shiftcheck = checkmessage;
              message = (shiftcheck >> 10);
              player = (message & 0x001F);
              shiftcheck = checkmessage;
              message = (shiftcheck >> 5);
              data = (message & 0x001F);
            } else {
              high_time = 0;
              signal_high = false;
              state = states::IDLE;
              break;
            }
            cout << "3" << endl;
            printMessage();
          }
        }
        high_time = 0;
        signal_high = false;
        state = states::IDLE;
        break;
    }
  };
};