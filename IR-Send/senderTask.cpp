#include "senderTask.hpp"

//====================================
// Private functions IRSendController
//====================================
void IRSendController::generateMessage() {
  uint8_t player = playerPool.read();
  uint8_t data = dataPool.read();
  // cout << "Player | Data: " << player << " | " << data << endl;
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
// Public functions IRSendController
//====================================
void IRSendController::sendMessage(uint8_t player_num, uint8_t data) {
  playerPool.write(player_num);
  dataPool.write(data);
  newMessageFlag.set();
}

void IRSendController::repeatSend() { repeatFlag.set();}

void IRSendController::main() {
  state = states::WAIT_FOR_FLAG;
  for (;;) {
    switch (state) {
      case states::WAIT_FOR_FLAG: {
        auto wait_trigger = wait(newMessageFlag + repeatFlag);
        if (wait_trigger == newMessageFlag) {
					cout << "GENERATE" << endl;
          generateMessage();
          messageBitPrinter<16, uint16_t>();  // For debugging
        }
        bit_send = 16;
        mess_repeat = 0;
        state = states::SEND_MESSAGE;
        hwlib::wait_us(100);
        break;
      }
      case states::SEND_MESSAGE:
        bit_send--;
        mask = 0x0001 << bit_send;
        bit = message & mask;
        if (bit_send < 0) {
          mess_repeat++;
          if (mess_repeat == 2) {
            state = states::WAIT_FOR_FLAG;
          } else {
            bit_send = 16;
            signalTimer.set(3000);
            wait(signalTimer);
            state = states::SEND_MESSAGE;
          }
        } else {
          if (bit) {
            state = states::BIT_1_HIGH;
          } else {
            state = states::BIT_0_HIGH;
          }
        }
        break;
      case states::BIT_1_HIGH:
        ir_led.write(1);
        signalTimer.set(1600);
        wait(signalTimer);
        state = states::BIT_1_LOW;
        break;
      case states::BIT_1_LOW:
        ir_led.write(0);
        signalTimer.set(800);
        wait(signalTimer);
        state = states::SEND_MESSAGE;
        break;
      case states::BIT_0_HIGH:
        ir_led.write(1);
        signalTimer.set(800);
        wait(signalTimer);
        state = states::BIT_0_LOW;
        break;
      case states::BIT_0_LOW:
        ir_led.write(0);
        signalTimer.set(1600);
        wait(signalTimer);
        state = states::SEND_MESSAGE;
        break;
      default: cout << "ERROR: IR Sender switch failed."; break;
    }
  }
}