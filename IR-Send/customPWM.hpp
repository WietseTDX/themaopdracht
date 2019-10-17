#ifndef CUSTOMPWM_HPP
#define CUSTOMPWM_HPP

#include "hwlib.hpp"

/* This template classes is based on 36Khz_d2 class found in hwlib.
 * The template gives a pin if set to 1 it has a freq that is given.
 * You need to fill in the freq in Hz. so 38 kHz = 38000 Hz
 * The second template variabele sets the duty cycle.
 * if you fill in 2 duty cyle = 50%, 4 = 25%, etc
 */
template <signed int M, signed int N>
class PinPWMD2 : public hwlib::pin_out {
 public:
  /// create the 36kHz output
  PinPWMD2(){
    // this sets the main clock to 84 MHz
    hwlib::wait_ms(1);

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
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVSEL_UP_RC;

    // configure pin A match clear
    TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_ACPC_SET | TC_CMR_ACPA_CLEAR;

    // set C match/clear for 36 kHz
    TC0->TC_CHANNEL[0].TC_RC = 41'000'000 / M;

    // set duty cycle based on N
    TC0->TC_CHANNEL[0].TC_RA = TC0->TC_CHANNEL[0].TC_RC / N;

    write(0);

    // enable and start
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  }

  /// enable or disable the 36 kHz output
  //
  /// Calling set( 1 ) enables the 36 kHz output, calling set( 0 )
  /// disables the output and makes the output low.
  void write(bool b) override {
    if (b) {
      // eable C match -> pin set
      TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_ACPC_SET;
    } else {
      // remove pin C match action
      TC0->TC_CHANNEL[0].TC_CMR &= ~(0x3 << 18);
    }
  }

  void flush() override {}
};

#endif