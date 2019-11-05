#ifndef _MAIN_CONTROLLER
#define _MAIN_CONTROLLER

#include "hwlib.hpp"
#include "rtos.hpp"

//============================
#include "IRSendController.hpp"
#include "WindowController.hpp"
#include "Button.hpp"
#include "StructData.cpp"
#include "ButtonHandeler.hpp"
#include "KeyboardControl.hpp"
//============================


class MainController : public rtos::task<>, public KeyboardListener, public ButtonListener {
	rtos::flag ButtonPressedFlag;
	rtos::pool<int> ButtonIDPool;
	rtos::channel<StructData, 4> CommandChannel; //4 Omdat de Receiver kan elke 40 ms een bericht ontvangen en je kan niet sneller dan 10x per seconde een kopje indrukken en van de keyboard controller krijgt die een bericht per seconde omdat sneller wel heel lastig is. De main heeft een deadline van 50 ms neem ik aan dat voor de 40 ms van de receiver neemt 2 plekken in en de button een halve plek dat kan niet dus 1 en voor de keyborad ook 1, 1tje er bij voor de veiligheid dus 4 plekken
	rtos::timer ShotTimer;
	rtos::timer BeenShotTimer;
	rtos::timer BuzzerTimer;
	rtos::clock PeriodFlag;

	PlayerInformation<100> info;
	WindowController Window;

	hwlib::target::pin_out buzzer = hwlib::target::pin_out(hwlib::target::pins::d5);
	
	IRSendController IrSend;

	InputHandler handler;

	int count_down;
	bool shot = false;
	bool been_shot = false;

	enum class states {WAIT_FOR_COMMAND, WAIT_FOR_START_GAME, WAIT_FOR_PC, COUNTDOWN};
	states state = states::WAIT_FOR_START_GAME;
	enum buttons {shoot_trigger = 0};
	enum keyboards {keypad = 0};
	hwlib::target::pin_in button_pin = hwlib::target::pin_in(hwlib::target::pins::d53);
	KeyboardController keyboard = KeyboardController(keyboards::keypad);
	Button button = Button(button_pin, buttons::shoot_trigger);
	public:
	/// \brief
	/// Constructor
	/// \details
	/// To initiate the the rtos objects and fill the button in the button handeler
	MainController(InputHandler &handler, hwlib::window & w) : 
		task("MainController"), 
		ButtonPressedFlag(this, "ButtonPressedFlag"), 
		ButtonIDPool("ButtonIDPool"), 
		CommandChannel(this, "CommandChannel"), 
		ShotTimer(this, "ShotTimer"), 
		BeenShotTimer(this, "BeenShotTimer"), 
		BuzzerTimer(this, "BuzzerTimer"), 
		PeriodFlag(this, 1000000, "PeriodFlag"),
		Window(w, info){
		handler.addKeyboard(&keyboard);
		handler.addButton(&button);
		keyboard.addKeyboardListener(this);
		button.addButtonListener(this);
	}

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
	/// It puts the StructData in the Channel <br> the id is by default 0 because with the keyboard he gets a id but with IrReceive not. <br> With 0 it will always pass thourt to the right state for the IRReciever
	void translateCmd(StructData data, int id = 0);

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

#endif // _MAIN_CONTROLLER