#include "hwlib.hpp"
#include "rtos.hpp"
#include "CommandTranslator.hpp"

class Control : rtos::task<> {
    private:
    InputTranslator & C;
    public:
    Control(InputTranslator & C) : C(C) {}
    void main() {
        while (true) {
            char a[4] = "CA5";
            char b[4] = "CB2";
            hwlib::cout << ">> change player number" << hwlib::endl;
            C.convertCommand(a);
            hwlib::cout << ">> been shot" << hwlib::endl;
            C.convertIrInput(5, 1);
            hwlib::wait_ms(100);
            hwlib::cout << ">> change wapeon" << hwlib::endl;
            C.convertCommand(b);
            hwlib::cout << ">> player0 wel 5de bit" << hwlib::endl;
            C.convertIrInput(0b00100011, 0);
            hwlib::cout << ">> start game" << hwlib::endl;
            C.convertIrInput(0,0);
        }
    }
};

int main() {
    hwlib::wait_ms(1000);
    MainController m = MainController();
    InputTranslator i = InputTranslator(m);
    Control a = Control(i);
    
    rtos::run();
}