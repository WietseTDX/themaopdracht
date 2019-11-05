#ifndef WINDOWCONTROLLER_HPP
#define WINDOWCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

#include "PlayerInformation.hpp"

/// \brief 
/// WindowController task
/// \details
/// This task controlls everything what is displayed on the OLED.
/// WindowController has 4 window parts. If a Window part needs to be updateted,
/// another class can call 1 of the 4 different functions. The flags en pool will be set.
/// The corresponding state will be activated and updates it corresponding window part.

class WindowController : public rtos::task<> {
private:
    hwlib::window & window;
    PlayerInformation<100> & info;
    hwlib::window_part_t time_window, player_number_window, health_window, weapon_window;
    char textdisplay[10] = { '0','1','2','3','4','5','6','7','8','9'};
    rtos::channel<int, 4> WindowChannel;
    enum class states { WAIT_FOR_CHANGE, UPDATE_TIME, UPDATE_HEALTH, UPDATE_PLAYER_INFORMATION, UPDATE_WEAPON };
    states state;
    hwlib::font_default_8x8     font8           = hwlib::font_default_8x8();
    hwlib::font_default_16x16   font16          = hwlib::font_default_16x16();
    hwlib::terminal_from        display_time    = hwlib::terminal_from( time_window,          font16 );
    hwlib::terminal_from        display_health  = hwlib::terminal_from( health_window,        font8  );
    hwlib::terminal_from        display_player  = hwlib::terminal_from( player_number_window, font16 );
    hwlib::terminal_from        display_weapon  = hwlib::terminal_from( weapon_window,        font16 );
    void drawBox(hwlib::window_part_t & window, int x, int y);

public:
    WindowController(hwlib::window & window, PlayerInformation<100> & info):
    window(window),
    info(info),
    time_window(hwlib::window_part_t(window, hwlib::xy(0,0), hwlib::xy(80, 17))),
    player_number_window(hwlib::window_part_t(window, hwlib::xy(81,0), hwlib::xy(127, 17))),
    health_window(hwlib::window_part_t(window, hwlib::xy(0,18), hwlib::xy(64, 63))),
    weapon_window(hwlib::window_part_t(window, hwlib::xy(65,18), hwlib::xy(127, 63))),
    WindowChannel(this, "WindowChannel")
    {
        window.clear();
        for (int i=0; i<4; i++) {
			update(i);
		}
        hwlib::cout << "In constructor Windowcontroller" << hwlib::endl;
    }
    /// \brief 
    /// Update Window
    /// \details
    /// Update a window based on a entity class PlayerInformation\n
    /// window_number: 0=Time 1=PlayerNumber 2=Health 3=Weapon
    void update(int window_number);
    /// \brief 
    /// main task loop of WindowController
    /// \details
    /// state WAIT_FOR_CHANGE waits for a flag to be set.\n
    /// If a flag is set, the corresponding state will be activated.\n
    /// That state updates the corresponding window with the value in the OledPool.
    void main() override;
};

#endif