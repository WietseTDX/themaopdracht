#ifndef OLEDCONTROLER_HPP
#define OLEDCONTROLLER_HPP

#include "hwlib.hpp"
#include "../../rtos/rtos.hpp"

class OLEDController : public rtos::task<> {
private:
    hwlib::glcd_oled oled;
    hwlib::window_part_t timeWindow, playerNumberWindow, healthWindow, weaponWindow;
    char textdisplay[10] = { '0','1','2','3','4','5','6','7','8','9'};
    rtos::flag updateTimeFlag, updateHealthFlag, updatePlayerFlag, updateWeaponFlag;
    rtos::pool< int > OledPool;
    enum class states { WAIT_FOR_CHANGE, UPDATE_TIME, UPDATE_HEALTH, UPDATE_PLAYER_INFORMATION, UPDATE_WEAPON };
    states state;
    hwlib::font_default_8x8     font8           = hwlib::font_default_8x8();
    hwlib::font_default_16x16   font16          = hwlib::font_default_16x16();
    hwlib::terminal_from        displayTime     = hwlib::terminal_from( timeWindow,         font16 );
    hwlib::terminal_from        displayHealth   = hwlib::terminal_from( healthWindow,       font8  );
    hwlib::terminal_from        displayPlayer   = hwlib::terminal_from( playerNumberWindow, font16 );
    hwlib::terminal_from        displayWeapon   = hwlib::terminal_from( weaponWindow,       font16 );
    void drawBox(hwlib::window_part_t & window, int x, int y);

public:
    OLEDController(hwlib::glcd_oled & oled):
    oled(oled),
    timeWindow(hwlib::window_part_t(oled, hwlib::xy(0,0), hwlib::xy(80, 17))),
    playerNumberWindow(hwlib::window_part_t(oled, hwlib::xy(81,0), hwlib::xy(127, 17))),
    healthWindow(hwlib::window_part_t(oled, hwlib::xy(0,18), hwlib::xy(64, 63))),
    weaponWindow(hwlib::window_part_t(oled, hwlib::xy(65,18), hwlib::xy(127, 63))),
    updateTimeFlag(this, "updateTimeFlag"),
    updateHealthFlag(this, "updateHealthFlag"),
    updatePlayerFlag(this, "updatePlayerFlag"),
    updateWeaponFlag(this, "updateWeaponFlag"),
    OledPool("OledPool")
    {
        oled.clear();
        oled.flush();
    }

    void updateTime(int time);
    void updateHealth(int health_points);
    void updatePlayerNumber(int player_number);
    void updateWeapon(int weapon);
    void main() override;

};

#endif