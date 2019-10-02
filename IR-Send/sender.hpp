#ifndef SEND_HPP
#define SEND_HPP

#include "hwlib.hpp"
using hwlib::cout;
using hwlib::endl;
using hwlib::target::d2_36kHz;

class sender {
	private:
	d2_36kHz ir;
	public:
	sender(d2_36kHz& ir) : ir(ir){}
};

/* MSB is printed first
 * N is bitlength of variable
 * F is the type of the variable
 */
template<int N, typename F>
void bitPrinter(const F& number){
	cout << "bit representation: " << endl;
	for(int i = N-1; i >= 0; i--){
		bool p = number & (0x01 << i);
		cout << p;
		if(i % 4 == 0) cout << ' ';
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
    return 0;
  }
  uint16_t message = 0x8000;
  message |= (player << 10);
  message |= (data << 5);
  uint8_t xor_res = (player ^ data);
  message |= (xor_res & 0x1F);
	return message;
}

#endif