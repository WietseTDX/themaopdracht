#include "hwlib.hpp"
#include "rtos.hpp"

//============================
#include "ButtonHandeler.cpp"
#include "IrSend.cpp"
#include "OLEDController.cpp"
//============================

struct StructData {
	StructData(int to_change, uint16_t data) : to_change(to_change), data(data) {};
	int to_change;
	uint16_t data;
};

class MainController : public rtos::task<> {
	rtos::flag ButtonPressedFlag;
	rtos::pool<int> ButtonIDPool;
	rtos::channel<StructData, 4> CommandChannel; //4 Omdat de Receiver kan elke 40 ms een bericht ontvangen en je kan niet sneller dan 10x per seconde een kopje indrukken en van de keyboard controller krijgt die een bericht per seconde omdat sneller wel heel lastig is. De main heeft een deadline van 50 ms neem ik aan dat voor de 40 ms van de receiver neemt 2 plekken in en de button een halve plek dat kan niet dus 1 en voor de keyborad ook 1, 1tje er bij voor de veiligheid dus 4 plekken
	rtos::timer ShotTimer;
	rtos::timer BeenShotTimer;
	rtos::timer BuzzerTimer;
	rtos::clock PeriodFlag;

	hwlib::window w = hwlib::window();

	hwlib::target::pin_out buzzer = hwlib::target::pin_out(hwlib::target::pins::d5);
	IrSendController IrSend;
	PlayerInformation<100> info = PlayerInformation<100>();
	WindowController Window(w, info);
	Button button;
	int count_down;
	int keyboard;
	bool shot = false;
	bool been_shot = false;

	enum class states {WAIT_FOR_COMMAND, WAIT_FOR_START_GAME, WAIT_FOR_PC, COUNTDOWN};
	states state = states::WAIT_FOR_START_GAME;
	enum buttons {shoot_trigger = 0};
	public:
	/// \brief
	/// Constructor
	/// \details
	/// To initiate the the rtos objects and fill the button in the button handeler
	MainController(InputHandeler& handeler);

	/// \brief
	/// The main
	/// \details
	/// The main functions with all the states
	void main() override;

	/// \brief
	/// Inherted from the ButtonLister interface function
	/// \details
	/// This function is called when a button is pressed
	void buttonPressed(int button);

	/// \brief
	/// translate Command
	/// \details
	/// It puts the StructData in the Channel
	void translateCommand(StructData data);

	// interne functies
	private:
	/// \brief
	/// Time to wait for next shot
	/// \details
	/// It calculates the time to wait for another shot depending on the weapon type
	int timeWaitShot();

	/// \brief
	/// Calculate the damage
	/// \details
	/// Calculates the damage that had been done by the shooter based on the weapon
	int calculateDamage(int data);
	/// \brief
	/// Run command
	/// \details
	/// Decode the Strutct data and start the right processes When the game is started
	void runCommand(StructData command);

	/// \brief
	/// Start game
	/// \details
	/// It checkes the StructData for a start command from the Receiver
	bool startGame();
};