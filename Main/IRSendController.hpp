#ifndef IRSENDCONTROLLER_HPP
#define IRSENDCONTROLLER_HPP

#include "PinPWMD2.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

using hwlib::cout;
using hwlib::endl;
using hwlib::wait_us;

/// @file

/// \brief
/// IRSendController
/// \details
/// This class creates an IRSendController object.
/// This class implements an IR protocol.
/// The full details about the protocol can be found in the casus lasertag.
/// The class can send 5-bit player and 5-bit data information.
/// The class can send a new message based on two parameters, or send the last message again.
class IRSendController : rtos::task<> {
 private:
  PinPWMD2<38000, 2> ir_led;
  int long_wait = 1600;
  int short_wait = 800;
  uint16_t message, mask;
  unsigned int mess_repeat;
  int bit_send;
  bool bit;
  bool sending = false;

  rtos::pool<uint8_t> PlayerPool;
  rtos::pool<uint8_t> DataPool;
  rtos::flag NewMessageFlag, RepeatFlag;
  rtos::timer SignalTimer;

  enum class states { WAIT_FOR_FLAG, TRANSMIT_MESSAGE };
  enum class states_transmit { HIGH_WAIT, LOW_WAIT };
  states state;
  states_transmit transmit_state;

  /// \brief
  /// Generate message
  /// \details
  /// This function generates the 16-bit message.
  /// The message is generated based on the Player- and DataPool.
  /// This function is called when the NewMessageFlag is set.
  void generateMessage();

 public:
  /// \brief
  /// Constructor IRSendController
  /// \details
  /// This function creater a IRSendController object.\n
  /// The constructor has no parameters.
  /// Call Example: auto sender = IRSendController();
  IRSendController()
      : task("IR-SendController"),
        ir_led(PinPWMD2<38000, 2>()),
        PlayerPool("PlayerPool"),
        DataPool("DataPool"),
        NewMessageFlag(this, "NewMessageFlag"),
        RepeatFlag(this, "RepeatFlag"),
        SignalTimer(this, "SignalTimer") {
    ir_led.write(0);
  }

  /// \brief
  /// Send 16-bit message
  /// \details
  /// This function sends a 16-bits message according to the IR-protocol.
  /// This function expects a player number and data. The max length of both parameters is 5-bits.
  /// The function generates a 16-bits message based on player_num and data.
  /// Call example: sender.sendMessage(0,4);
  void sendMessage(uint8_t player_num, uint8_t data);

  /// \brief
  /// Repeat 16-bit message
  /// \details
  /// This function sends a 16-bits message according to the IR-protocol.
  /// The function will send the message that is generated add the last sendMessage function call.
  /// It is advised to call sendMessage before repeatSend, because otherwise the message in unkhown.
  /// Call example: sender.repeatSend();
  void repeatSend();

  /// \brief
  /// Implementation main IRSendController
  /// \details
  /// The function main() is inherited from rtos::task<>.
  /// The main function says what the class will do in it possible states.
  /// The function contains all the behaviour off the IRSendController task.
  void main() override;
};
/// \brief
/// messageBitPrinter
/// \details
/// This function prints a parameter in bits.
/// The function is templated. The first template parameter is the amount bit in the variable type.
/// The second template variable is the type. This parameter is not obligated.
/// The function it self expects a variable of a type.
/// Call example: bitPrinter<16>( (uint16_t) message);
template <int N, typename F>
void bitPrinter(F to_print) {
  bool p;
  cout << "bit representation: " << endl;
  for (int i = N - 1; i >= 0; i--) {
    p = to_print & (0x01 << i);
    cout << p;
    if (i % 4 == 0) cout << ' ';
  }
  cout << endl;
}

#endif  // IRSENDCONTROLLER_HPP