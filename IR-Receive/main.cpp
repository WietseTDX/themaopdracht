#include "receiver.hpp"


int main() {
    hwlib::wait_ms(500);
    // auto tsop_gnd    = hwlib::target::pin_out( hwlib::target::pins::d9 );
    // auto tsop_vdd    = hwlib::target::pin_out( hwlib::target::pins::d10 );
    // tsop_gnd.write( 0 );
    // tsop_vdd.write( 1 );
    // tsop_gnd.flush();
    // tsop_vdd.flush();

    auto receiver = IrReceive(due::pins::d8, "receiver" );
    receiver.main();
};