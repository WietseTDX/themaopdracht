#include "Maincontroller.hpp"

void MainController::main() {
  while (true) {
    switch (state) {
      case (states::WAIT_FOR_COMMAND) : {
        auto event = wait(PeriodeFlag + CommandChannel);
        if (event == PeriodeFlag) {
					if (hwlib::now_us() >= last_us + 1000000) {
						last_us += 1000000;
						CommandData Time = CommandData(0, last_us/1000000);
						oled.update(Time);
						if (time <= 0) {
							state = states::WAIT_FOR_PC;
						}
					}
					if (event == CommandChannel) {
						command = CommandChannel.read();
						runCommand(commnad);
					}
      	}
    	}
			case (states::WAIT_FOR_PC): {
				if (time <= 0) {
					hwlib::cin >> keyboard;
					if (keyboard == 111) {
						hwlib::cout << player_information;
						state = states::WAIT_FOR_START_GAME;
					}
				}
			}
			case (states::WAIT_FOR_START_GAME): {
				auto event = wait(CommandChannel);
				if (CommandChannel.read().type == 4) {
					state = states::WAIT_FOR_COMMAND;
				}
			}
  	}
	}
}

void MainController::buttonPressed(Button b) {
	ButtonIDPool.write(b);
	ButtonPressedFlag.set();
}

void MainController::translateCommand(CommandData data) {
	CommandChannel.write(data);
}

void MainController::runCommand(CommandData command) {
	if (command.type == 1){//change player number
		player_information.setPlayerNumber(command.data);
		oled.update(command);
	} else if (CommandData.type == 2) { //change health
		int weapon = command.data && 0x31;
		int player = command.data && 0x31 << 5;
		addHit(player, weapon);
		int damage = calculateDamage(weapon);
		int health = player_information.getHealth();
		health -= damage;
		if (health <= 0) {health = 0;}
		player_information.setHealth(health);
		command.data = health;
		oled.update(command);
	} else if (command.type == 3) { //change weapon
		player_information.setWeapon(command.data);
		oled.update(command);
	}
}