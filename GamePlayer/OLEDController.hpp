#ifndef OLEDCONTROLER_HPP
#define OLEDCONTROLLER_HPP

#include "hwlib.hpp"
#include "../../rtos/rtos.hpp"


/// \brief OLEDController task
/// \details
/// This task controlls everything what is displayed on the OLED.
/// Oledcontroller has 4 window parts. If a Window part needs to be updateted,
/// another class can call 1 of the 4 different functions. The flags en pool will be set.
/// The corresponding state will be activated and updates it corresponding window part.
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

    /// \brief Update Time
    /// \details
    /// This fucntions sets updateTimeFlag and writes the given parameter to the OledPool.\n
    /// State UPDATE_TIME will activate in the main loop
    void updateTime(int time);
    /// \brief Update Health
    /// \details
    /// This fucntions sets updateHealthFlag and writes the given parameter to the OledPool.\n
    /// State UPDATE_HEALTH will activate in the main loop
    void updateHealth(int health_points);
    /// \brief Update Player Number
    /// \details
    /// This fucntions sets updatePlayerFlag and writes the given parameter to the OledPool.\n
    /// State UPDATE_PLAYER_INFORMATION will activate in the main loop
    void updatePlayerNumber(int player_number);
    /// \brief Update Weapon
    /// \details
    /// This fucntions sets updateWeaponFlag and writes the given parameter to the OledPool.\n
    /// State UPDATE_WEAPON will activate in the main loop
    void updateWeapon(int weapon);
    /// \brief main task loop of OLEDController
    /// \details
    /// state WAIT_FOR_CHANGE waits for a flag to be set.\n
    /// If a flag is set, the corresponding state will be activated.\n
    /// That state updates the corresponding window with the value in the OledPool.
    void main() override;
};

#endif