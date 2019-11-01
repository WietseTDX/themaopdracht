#include "hwlib.hpp"
#include "rtos.hpp"

struct StructData {
	StructData(int to_change, uint16_t data) : to_change(to_change), data(data) {};
	int to_change;
	uint16_t data;
};

class MainController : public rtos::task<> {
	rtos::flag ButtonPressedFlag;
	rtos::pool<int> ButtonIDPool;
	rtos::channel<3, StructData> CommandChannel; //5 Omdat de Receiver kan elke 40 ms een bericht ontvangen en je kan niet sneller dan 10x per seconde een kopje indrukken en van de keyboard controller krijgt die een bericht per seconde omdat sneller wel heel lastig is. De main heeft een deadline van 50 ms neem ik aan dat voor de 40 ms van de receiver neemt 2 plekken in en de button een halve plek dat kan niet dus 1 en voor de keyborad ook 1 dus 3 plekken
	rtos::timer ShotTimer;
	rtos::timer BeenShotTimer;
	rtos::timer BuzzerTimer

	hwlib::window w;

	hwlib::due::pin_out(hwlib::due::pins::d5) buzzer;
	IrSendController IrSend;
	PlayerInformation<100> Info;
	WindowController Window(w, Info);
	Button button;
	int count_down;
	int keyboard;
	bool shot = false;
	bool been_shot = false;

	enum class states {WAIT_FOR_COMMAND, WAIT_FOR_START_GAME, WAIT_FOR_PC, COUNTDOWN};
	states state = states::WAIT_FOR_START_GAME;
	enum buttons {shoot_trigger = 0};
	public:
	MainController(InputHandeler& handeler);
	void main() override;
	void buttonPressed(int button);
	void translateCommand(StructData data);

	// interne functies
	private:
	int timeWaitShot();

	int calculateDamage(int data);
	void runCommand(StructData command);

	bool startGame();
}