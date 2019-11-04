#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include "windowStruct.hpp"
#include "GameRules.hpp"
#include "WindowController.hpp"
#include "IRSendController.hpp"

class MainController : rtos::task<> {
    private:
        enum states {IDLE, COMMAND_SET};
        states state = IDLE;
        uint8_t data;
        char command[10];
        char set_command[10];
        bool first_send;
        windowStruct screen_data;
        WindowController window;
        IRSendController ir_sender;
        GameRules game_rules;
        rtos::flag CommandFlag;
        rtos::pool<char*> CommandPool;

        int charsToInt(char left, char right);

        void verifyCommand();
    
        void commandActions();

    public:
        MainController(hwlib::glcd_oled_i2c_128x64_buffered& oled, const hwlib::font& f):
            screen_data(),
            window(oled, f),
            ir_sender(),
            game_rules(),
            CommandFlag( this, "CommandFlag"),
            CommandPool( "CommandPool" )
        {}

        void setCmd(char* command);

        void main();
        
};
