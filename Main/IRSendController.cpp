#include "IRSendController.hpp"

//====================================
// PRIVATE FUNCTIONS IRSendController
//====================================
void IRSendController::generateMessage() {
  uint8_t player = PlayerPool.read();
  uint8_t data = DataPool.read();
  if (player > 31) {
    cout << "ERROR : generateMessage\nPlayer number is too big" << endl;
    return;
  } else if (data > 31) {
    cout << "ERROR : generateMessage\ndata is too big" << endl;
    return;
  }
  message = 0x8000;
  message |= (player << 10);
  message |= (data << 5);
  uint8_t xor_res = (player ^ data);
  message |= (xor_res & 0x1F);
}

//====================================
// PUBLIC FUNCTIONS IRSendController
//====================================
void IRSendController::sendMessage(uint8_t player_num, uint8_t data) {
  PlayerPool.write(player_num);
  DataPool.write(data);
  NewMessageFlag.set();
}

void IRSendController::repeatSend() { RepeatFlag.set(); }

void IRSendController::main() {
  state = states::WAIT_FOR_FLAG;
  for (;;) {
    switch (state) {
      case states::WAIT_FOR_FLAG: {
        auto wait_trigger = wait(NewMessageFlag + RepeatFlag);
        if (wait_trigger == NewMessageFlag) {
          generateMessage();
        } else {
        }  // if (wait_trigger) & else
        mess_repeat = 0;
        state = states::TRANSMIT_MESSAGE;
        wait_us(100);
      }
      case states::TRANSMIT_MESSAGE:
        sending = true;
        bit_send = 16;
        transmit_state = states_transmit::HIGH_WAIT;
        while (sending) {
          switch (transmit_state) {
            case states_transmit::HIGH_WAIT:
              bit_send--;
              mask = 0x0001 << bit_send;
              bit = message & mask;
              ir_led.write(1);
              if (bit) {
                SignalTimer.set(long_wait);
              } else {
                SignalTimer.set(short_wait);
              }
              wait(SignalTimer);
              transmit_state = states_transmit::LOW_WAIT;
              break;
            case states_transmit::LOW_WAIT:
              ir_led.write(0);
              if (bit) {
                SignalTimer.set(short_wait);
              } else {
                SignalTimer.set(long_wait);
              }
              wait(SignalTimer);
              if (bit_send > 0) {
                transmit_state = states_transmit::HIGH_WAIT;
              } else {
                sending = false;
              }
              break;
            default: cout << "ERROR: IRSender transmitswitch failed."; break;
          }  // switch transmit_state
        }    // while (sending)
        mess_repeat++;
        if (mess_repeat == 2) {
          state = states::WAIT_FOR_FLAG;
        } else {
          SignalTimer.set(3000);
          wait(SignalTimer);
        }
        break;
      default: cout << "ERROR: IRSender switch failed."; break;
    }  // switch state
  }    // for(;;)
}  // main()