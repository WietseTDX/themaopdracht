#ifndef SEND_HPP
#define SEND_HPP

#include "hwlib.hpp"
using hwlib::cout;
using hwlib::endl;
using hwlib::target::d2_36kHz;

class sender {
 private:
  d2_36kHz ir_led;

 public:
  sender(d2_36kHz& ir) : ir_led(ir) {}
  /* MSB is printed first
   * N is bitlength of variable
   * F is the type of the variable
   */
  template <int N, typename F>
  void bitPrinter(const F& number) {
    cout << "bit representation: " << endl;
    for (int i = N - 1; i >= 0; i--) {
      bool p = number & (0x01 << i);
      cout << p;
      if (i % 4 == 0) cout << ' ';
    }
    cout << endl;
  }

  /* Player is a value between 0 & 31
   * Data is a value between 0 & 31
   */
  uint16_t generateMessage(uint8_t player, uint8_t data) {
    cout << "Player | Data: " << player << " | " << data << endl;
    if (player > 31) {
      cout << "ERROR : generateMessage\nPlayer number is too big" << endl;
      return -1;
    } else if (data > 31) {
      cout << "ERROR : generateMessage\ndata is too big" << endl;
      return -1;
    }
    uint16_t message = 0x8000;
    message |= (player << 10);
    message |= (data << 5);
    uint8_t xor_res = (player ^ data);
    message |= (xor_res & 0x1F);
    return message;
  }

  void sendMessage(const uint16_t& message) {
    for (int mess_repeat = 0; mess_repeat < 2; mess_repeat++) {
      for (int bit_send = 15; bit_send >= 0; bit_send--) {
        int mask = 0x0001 << bit_send;
				bool bit = message & mask;
				// cout << "bit: " << bit_send << '-' << bit << endl;
        if(bit){
					ir_led.write(1);
					ir_led.flush();
					hwlib::wait_us(1600);
					ir_led.write(0);
					ir_led.flush();
					hwlib::wait_us(800);
				} else{
					ir_led.write(1);
					ir_led.flush();
					hwlib::wait_us(800);
					ir_led.write(0);
					ir_led.flush();
					hwlib::wait_us(1600);		
				}
      }
			hwlib::wait_ms(3);
    }
  }
};

#endif