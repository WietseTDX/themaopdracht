#ifndef SEND_HPP
#define SEND_HPP

#include "hwlib.hpp"
using hwlib::cout;
using hwlib::endl;
// using hwlib::target::d2_36kHz;

class d2_38kHz : public hwlib::pin_out {
   public:

   /// create the 36kHz output
   d2_38kHz(): {
      
      // this sets the main clock to 84 MHz 
      hwlib::wait_ms( 1 );      
      
      // enable the clock to port B
      PMC->PMC_PCER0 = 1 << ID_PIOB;
	
      // disable PIO Control on PB25 and set up for Peripheral B TIOA0
	  PIOB->PIO_PDR = PIO_PB25; 
	  PIOB->PIO_ABSR |= PIO_PB25; 
	
	  // enable output on B25
	  PIOB->PIO_OER = PIO_PB25; 
   
      // enable the clock to the TC0 (peripheral # 27)
      PMC->PMC_PCER0 = 1 << ID_TC0;
   
      // configure TC0 channel 0 for WAVSEL=10 mode 
      // using the main clock / 2 as source
      // and reset on C match 
      TC0->TC_CHANNEL[ 0 ].TC_CMR =  
         TC_CMR_WAVE 
         | TC_CMR_TCCLKS_TIMER_CLOCK1 
         | TC_CMR_WAVSEL_UP_RC;   

      // configure pin A match clear
      TC0->TC_CHANNEL[ 0 ].TC_CMR |= TC_CMR_ACPC_SET | TC_CMR_ACPA_CLEAR; 
   
	   // set C match/clear for 36 kHz  
	   TC0->TC_CHANNEL[ 0 ].TC_RC = 41'000'000 / 38'000;
      
      // set 50% duty cycle
	   TC0->TC_CHANNEL[ 0 ].TC_RA = TC0->TC_CHANNEL[ 0 ].TC_RC / 2;
   
       write( 0 );
   
	   // enable and start 
	   TC0->TC_CHANNEL[ 0 ].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;      
   }
   
   /// enable or disable the 36 kHz output
   //
   /// Calling set( 1 ) enables the 36 kHz output, calling set( 0 )
   /// disables the output and makes the output low.
   void write( bool b ) override {
      if( b ){
         // eable C match -> pin set
         TC0->TC_CHANNEL[ 0 ].TC_CMR |= TC_CMR_ACPC_SET;         
      } else {
         // remove pin C match action
         TC0->TC_CHANNEL[ 0 ].TC_CMR &= ~ ( 0x3 << 18);  
      }
   }
   
   void flush() override {}
   
}; // class d2_36kHz

class sender {
 private:
  d2_38kHz ir_led;

 public:
  sender() : ir_led(d2_38kHz()) {}
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
					// ir_led.flush();
					hwlib::wait_us(1600);
					ir_led.write(0);
					// ir_led.flush();
					hwlib::wait_us(800);
				} else{
					ir_led.write(1);
					// ir_led.flush();
					hwlib::wait_us(800);
					ir_led.write(0);
					// ir_led.flush();
					hwlib::wait_us(1600);		
				}
      }
			hwlib::wait_ms(3);
    }
  }
};

#endif