#include "MainController.hpp"

MainController::MainController(InputHandeler& handeler) 
: task("MainController"), ButtonPressedFlag(this, "ButtonPressedFlag"), ButtonIDPool("ButtonIDPool"), CommandChannel(this, "CommandChannel"), ShotTimer(this, "ShotTimer"), BeenShotTimer(this, "BeenShotTimer"), BuzzerTimer(this, "BuzzerTimer"), PeriodFlag(this, 1000000, "PeriodFlag"){
	handeler.addButton(&button);
}

void MainController::main() {
	while (true) {
		switch (state) {
			case (states::WAIT_FOR_START_GAME) : {
				auto event = wait(CommandChannel);
				if (startGame()) {
					count_down = 10;
					state = states::COUNTDOWN;
				}
			break;
			}
			case (states::COUNTDOWN) : {
				auto event = wait(PeriodFlag);
				count_down--;
				if (count_down <= 0) {
					state = states::WAIT_FOR_COMMAND;
				}
			break;
			}
			case (states::WAIT_FOR_COMMAND) : {
				auto event = wait(ShotTimer + BeenShotTimer + ButtonPressedFlag + BuzzerTimer + CommandChannel + PeriodFlag);
				if (event == ShotTimer) {
					shot = false;
				}
				if (event == BeenShotTimer) {
					been_shot = false;
				}
				if (event == ButtonPressedFlag) {
					if (ButtonIDPool.read() == buttons::shoot_trigger && (!shot)) {
						IrSend.sendMessage(info.getPlayerNumber(), info.getWeapon());
						shot = true;
						ShotTimer.set(timeWaitShot());
						buzzer.write(1);
						BuzzerTimer.set(500000);
					}
				}
				if (event == BuzzerTimer) {
					buzzer.write(0);
				}
				if (event == CommandChannel) {
					StructData command = CommandChannel.read();
					runCommand(command);
					if (info.getHealthPoints() <= 0) {
						state = states::WAIT_FOR_PC;
					}
				}
				if (event == PeriodFlag) {
					info.setTime(info.getTime() - 1);
					Window.update(0);
					if (info.getTime <= 0) {
						state = states::WAIT_FOR_PC;
					}
				}
			break;
			}
			case (states::WAIT_FOR_PC) : {
				char keyboard;
				hwlib::cin >> keyboard;
				if (keyboard == 111) {
					hwlib::cout << info;
					state = states::WAIT_FOR_START;
				}
			break;
			}
		}
	}
}


void MainController::buttonPressed(int button) {
	ButtonIDPool.write(button);
	ButtonPressedFlag.set();
}
void MainController::translateCommand(StructData data) {
	CommandChannel.write(data);
}


//=====================//
//   interne functies  //
//=====================//
int MainController::timeWaitShot() {
	int weapon = info.getWeapon();
	int wait = 500000;
	wait +=500000 * weapon;
	return wait;
}

int MainController::calculateDamage(int data) {
	return data * 1;
}
void MainController::runCommand(StructData command) {
	if(command.to_change == 2){ //change health
	int player = (command.data >> 5) & 0x001F;
	if (player != 0 && (!been_shot)) {
		int data = command.data & 0x001F;
		buzzer.write(1);
		BuzzerTimer.set(1000000);
		info.addHit(player, data);
		BeenShotTimer.set(3000000);
		been_shot = true;
		int damage = calculateDamage(data);
		int health = info.getHealthPoints();
		health -= damage;
		if (health <= 0){ health = 0;}
		info.setHealthPoints(health);
		Window.update(2);
	} 
  }else if(command.to_change == 3){ //change weapon
    info.setWeapon(command.data);
    Window.update(3);
  }
}

bool MainController::startGame() {
	StructData command = CommandChannel.read();
	if (command.to_change == 2) { // change time or start game
		int data = command.data & 0x001F;
		int player = (command.data >> 5) & 0x001F;
		if (player == 0) {
			if (((data >> 5) & 0x01)) {
				info.setTime(data & 0b000001111);
				return false;
			}
			if (data == 0) {
				return true;
			}
		}
	}
	else if(command.to_change == 1){//change player number
		info.setPlayerNumber(command.data);
		Window.update(1);
	}
  return false;
}
