#include "PlayerInformation.hpp"
#include "Button.hpp"
#include "../../rtos/rtos.hpp"

class ButtonHandeler : public rtos::task<> {
  //KeyboardControl &keyboard;
  rtos::clock PeriodFlag;
  int place = 0;
	Button *button[1];
  public:
  ButtonHandeler(): 
	task("ButtonHandeler"), 
	PeriodFlag(this, 100000, "PeriodFlag") 
	{}

  void main() override;
  void addButton(Button *object);
};

void ButtonHandeler::main() {
  while (true) {
    auto event = wait(PeriodFlag);
    // keyboard.update();
    for (int i=0; i<place; i++) {
      button[i]->update();
    }
  }
}


void ButtonHandeler::addButton(Button *object) {
  button[place] = object;
  place++;
}

class TestMain : rtos::task<>, ButtonListener{
private:
	ButtonHandeler handler;
	due::pin_in pin = due::pin_in( due::pins::d22 );
	Button button = Button(pin, 1);
public:
	TestMain(ButtonHandeler & handler):
	handler(handler)
	{
		button.addButtonListener(this); 
		handler.addButton(&button);
	}

	void buttonPressed(int id){
		hwlib::cout << "Pressed!\n";
	}

	void main() override{
		for(;;){
			hwlib::wait_ms(50);
		}
	}

};

int main(){
	hwlib::wait_ms(1000);

	auto handler = ButtonHandeler();
	auto main = TestMain(handler);

	rtos::run();
}