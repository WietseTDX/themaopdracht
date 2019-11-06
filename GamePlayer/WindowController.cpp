#include "WindowController.hpp"
#include "PlayerInformation.hpp"
//========================================
// OTHER FUNCTIONS
//========================================

constexpr float power(float x, int power_int) {
  if (power_int == 0) {
    return 1;
  }
  if (power_int < 0) {
    return (1 / power(x, power_int * -1));
  }
  float total = x;
  for (float i = 0; i < power_int - 1; i++) {
    total *= x;
  }
  return total;
}

constexpr float sqrt(float x) {
  float sqrt = x / 2;
  float temp = 0;
  int counter = 0;
  while (sqrt != temp) {
    temp = sqrt;
    sqrt = (x / temp + temp) / 2;
    counter++;
    if (counter > 260000) {
      sqrt = 0;
      break;
    }
  }
  return sqrt;
}

constexpr float makeabsolute(float x) {
  if (x < 0) {
    x *= -1;
  }
  return x;
}

constexpr hwlib::xy hearttop(int x) {
  float calculate = makeabsolute(x * 0.1 - 2);
  calculate = power(calculate - 1, 2);
  calculate = sqrt(1 - calculate) * 10;
  int y = (makeabsolute(calculate - 10) + 0.5);
  return hwlib::xy(x + 7, y + 2);
}

constexpr hwlib::xy heartbottom(int x) {
  float calculate = makeabsolute(x * 0.1 - 2);
  calculate = sqrt(calculate) / sqrt(2);
  calculate = -3 * sqrt(1 - calculate) * 10 - 10;
  int y = (makeabsolute(calculate) + 0.5);
  return hwlib::xy(x + 7, y + 2);
}

template <int N, typename T>
class lookup {
 private:
 public:
  T values[N];
  template <typename F>
  constexpr lookup(F function) : values() {
    for (int i = 0; i < N; ++i) {
      values[i] = function(i);
    }
  }
  constexpr T get(int n) const { return values[n]; }
};

constexpr auto top_heart_values = lookup<42, hwlib::xy>(hearttop);
constexpr auto bottom_heart_values = lookup<42, hwlib::xy>(heartbottom);

//========================================
// PRIVATE FUNCTIONS WindowController
//========================================
void WindowController::drawBox(hwlib::window_part_t& window, int x, int y) {
  hwlib::rectangle(hwlib::xy(0, 0), hwlib::xy(x, y)).draw(window);
}

//========================================
// PUBLIC FUNCTIONS WindowController
//========================================
void WindowController::update(int window_number) { WindowChannel.write(window_number); }

void WindowController::main() {
  state = states::WAIT_FOR_CHANGE;
  window.clear();
  while (true) {
    switch (state) {
      case states::WAIT_FOR_CHANGE: {
        wait(WindowChannel);
        auto window_update = WindowChannel.read();
        if (window_update == 0) {
          state = states::UPDATE_TIME;
        } else if (window_update == 1) {
          state = states::UPDATE_PLAYER_INFORMATION;
        } else if (window_update == 2) {
          state = states::UPDATE_HEALTH;
        } else {
          state = states::UPDATE_WEAPON;
        }
        break;
      }  // case wait for change
      case states::UPDATE_TIME: {
        auto time = info.getTime();
        if (time == 0 && info.getPlayerNumber() != 0) {
          display_time << "\f"
                       << " END ";
          drawBox(time_window, 79, 16);
          time_window.flush();
          break;
        }
        display_time << "\f" << textdisplay[time / 600] << textdisplay[time / 60 % 10] << ':'
                     << textdisplay[time % 60 / 10] << textdisplay[time % 60 % 10];
        drawBox(time_window, 79, 16);
        time_window.flush();
        state = states::WAIT_FOR_CHANGE;
        break;
      }  // case update time
      case states::UPDATE_HEALTH: {
        int health_points = info.getHealthPoints();
        display_health << "\f"
                       << "\t0202" << textdisplay[health_points / 100] << textdisplay[health_points % 100 / 10]
                       << textdisplay[health_points % 10];
        for (int i = 0; i < 41; i++) {
          hwlib::line(top_heart_values.get(i), top_heart_values.get(i + 1)).draw(health_window);
          hwlib::line(bottom_heart_values.get(i), bottom_heart_values.get(i + 1)).draw(health_window);
        }
        drawBox(health_window, 63, 44);
        health_window.flush();
        state = states::WAIT_FOR_CHANGE;
        break;
      }  // case update health
      case states::UPDATE_PLAYER_INFORMATION: {
        int player_number = info.getPlayerNumber();
        display_player << "\f"
                       << "P:" << textdisplay[player_number];
        drawBox(player_number_window, 46, 16);
        player_number_window.flush();
        state = states::WAIT_FOR_CHANGE;
        break;
      }  // case update player information
      case states::UPDATE_WEAPON: {
        int weapon_number = info.getWeapon();
        if (weapon_number <= 3 && current_weapon != 0) {  // ak
          weapon_window.clear();
          hwlib::line ak_array[33] = {
              hwlib::line(hwlib::xy(5, 7), hwlib::xy(5, 11)),    hwlib::line(hwlib::xy(6, 9), hwlib::xy(6, 11)),
              hwlib::line(hwlib::xy(5, 10), hwlib::xy(50, 10)),  hwlib::line(hwlib::xy(5, 11), hwlib::xy(54, 11)),
              hwlib::line(hwlib::xy(17, 7), hwlib::xy(43, 7)),   hwlib::line(hwlib::xy(16, 8), hwlib::xy(45, 8)),
              hwlib::line(hwlib::xy(15, 9), hwlib::xy(17, 9)),   hwlib::line(hwlib::xy(24, 9), hwlib::xy(47, 9)),
              hwlib::line(hwlib::xy(17, 12), hwlib::xy(58, 12)), hwlib::line(hwlib::xy(22, 21), hwlib::xy(22, 22)),
              hwlib::line(hwlib::xy(23, 20), hwlib::xy(23, 23)), hwlib::line(hwlib::xy(24, 18), hwlib::xy(24, 23)),
              hwlib::line(hwlib::xy(25, 16), hwlib::xy(25, 24)), hwlib::line(hwlib::xy(26, 13), hwlib::xy(26, 24)),
              hwlib::line(hwlib::xy(27, 13), hwlib::xy(27, 23)), hwlib::line(hwlib::xy(28, 13), hwlib::xy(28, 21)),
              hwlib::line(hwlib::xy(29, 13), hwlib::xy(29, 19)), hwlib::line(hwlib::xy(30, 13), hwlib::xy(30, 17)),
              hwlib::line(hwlib::xy(31, 13), hwlib::xy(31, 14)), hwlib::line(hwlib::xy(34, 13), hwlib::xy(34, 14)),
              hwlib::line(hwlib::xy(36, 13), hwlib::xy(36, 14)), hwlib::line(hwlib::xy(38, 13), hwlib::xy(38, 15)),
              hwlib::line(hwlib::xy(35, 15), hwlib::xy(37, 15)), hwlib::line(hwlib::xy(39, 13), hwlib::xy(39, 18)),
              hwlib::line(hwlib::xy(40, 13), hwlib::xy(40, 19)), hwlib::line(hwlib::xy(41, 13), hwlib::xy(41, 19)),
              hwlib::line(hwlib::xy(42, 15), hwlib::xy(42, 18)), hwlib::line(hwlib::xy(46, 13), hwlib::xy(58, 13)),
              hwlib::line(hwlib::xy(47, 14), hwlib::xy(58, 14)), hwlib::line(hwlib::xy(49, 15), hwlib::xy(57, 15)),
              hwlib::line(hwlib::xy(51, 16), hwlib::xy(57, 16)), hwlib::line(hwlib::xy(53, 17), hwlib::xy(57, 17)),
              hwlib::line(hwlib::xy(55, 18), hwlib::xy(57, 18))};
          for (hwlib::line l : ak_array) {
            l.draw(weapon_window);
          }
          current_weapon = 0;
        } else if (weapon_number <= 6 && current_weapon != 1) {  // deagle
          weapon_window.clear();
          hwlib::line deagle_array[31] = {
              hwlib::line(hwlib::xy(8, 4), hwlib::xy(8, 8)),     hwlib::line(hwlib::xy(9, 4), hwlib::xy(9, 11)),
              hwlib::line(hwlib::xy(38, 4), hwlib::xy(43, 4)),   hwlib::line(hwlib::xy(10, 5), hwlib::xy(47, 5)),
              hwlib::line(hwlib::xy(10, 6), hwlib::xy(48, 6)),   hwlib::line(hwlib::xy(50, 6), hwlib::xy(52, 6)),
              hwlib::line(hwlib::xy(10, 7), hwlib::xy(52, 7)),   hwlib::line(hwlib::xy(10, 8), hwlib::xy(51, 8)),
              hwlib::line(hwlib::xy(10, 9), hwlib::xy(50, 9)),   hwlib::line(hwlib::xy(10, 10), hwlib::xy(49, 10)),
              hwlib::line(hwlib::xy(10, 11), hwlib::xy(49, 11)), hwlib::line(hwlib::xy(11, 12), hwlib::xy(51, 12)),
              hwlib::line(hwlib::xy(35, 13), hwlib::xy(53, 13)), hwlib::line(hwlib::xy(35, 14), hwlib::xy(35, 15)),
              hwlib::line(hwlib::xy(31, 13), hwlib::xy(31, 16)), hwlib::line(hwlib::xy(32, 13), hwlib::xy(32, 17)),
              hwlib::line(hwlib::xy(33, 16), hwlib::xy(33, 18)), hwlib::line(hwlib::xy(38, 14), hwlib::xy(50, 14)),
              hwlib::line(hwlib::xy(39, 15), hwlib::xy(48, 15)), hwlib::line(hwlib::xy(39, 16), hwlib::xy(47, 16)),
              hwlib::line(hwlib::xy(34, 17), hwlib::xy(47, 17)), hwlib::line(hwlib::xy(34, 18), hwlib::xy(47, 18)),
              hwlib::line(hwlib::xy(39, 19), hwlib::xy(48, 19)), hwlib::line(hwlib::xy(39, 20), hwlib::xy(48, 20)),
              hwlib::line(hwlib::xy(39, 21), hwlib::xy(48, 21)), hwlib::line(hwlib::xy(40, 22), hwlib::xy(48, 22)),
              hwlib::line(hwlib::xy(40, 23), hwlib::xy(48, 23)), hwlib::line(hwlib::xy(40, 24), hwlib::xy(49, 24)),
              hwlib::line(hwlib::xy(40, 25), hwlib::xy(49, 25)), hwlib::line(hwlib::xy(40, 26), hwlib::xy(49, 26)),
              hwlib::line(hwlib::xy(41, 27), hwlib::xy(48, 27))};
          for (hwlib::line l : deagle_array) {
            l.draw(weapon_window);
          }
          current_weapon = 1;
        } else if (weapon_number <= 9 && current_weapon != 2) {
          hwlib::line deagle_array[31] = {
              hwlib::line(hwlib::xy(8, 4), hwlib::xy(8, 8)),     hwlib::line(hwlib::xy(9, 4), hwlib::xy(9, 11)),
              hwlib::line(hwlib::xy(38, 4), hwlib::xy(43, 4)),   hwlib::line(hwlib::xy(10, 5), hwlib::xy(47, 5)),
              hwlib::line(hwlib::xy(10, 6), hwlib::xy(48, 6)),   hwlib::line(hwlib::xy(50, 6), hwlib::xy(52, 6)),
              hwlib::line(hwlib::xy(10, 7), hwlib::xy(52, 7)),   hwlib::line(hwlib::xy(10, 8), hwlib::xy(51, 8)),
              hwlib::line(hwlib::xy(10, 9), hwlib::xy(50, 9)),   hwlib::line(hwlib::xy(10, 10), hwlib::xy(49, 10)),
              hwlib::line(hwlib::xy(10, 11), hwlib::xy(49, 11)), hwlib::line(hwlib::xy(11, 12), hwlib::xy(51, 12)),
              hwlib::line(hwlib::xy(35, 13), hwlib::xy(53, 13)), hwlib::line(hwlib::xy(35, 14), hwlib::xy(35, 15)),
              hwlib::line(hwlib::xy(31, 13), hwlib::xy(31, 16)), hwlib::line(hwlib::xy(32, 13), hwlib::xy(32, 17)),
              hwlib::line(hwlib::xy(33, 16), hwlib::xy(33, 18)), hwlib::line(hwlib::xy(38, 14), hwlib::xy(50, 14)),
              hwlib::line(hwlib::xy(39, 15), hwlib::xy(48, 15)), hwlib::line(hwlib::xy(39, 16), hwlib::xy(47, 16)),
              hwlib::line(hwlib::xy(34, 17), hwlib::xy(47, 17)), hwlib::line(hwlib::xy(34, 18), hwlib::xy(47, 18)),
              hwlib::line(hwlib::xy(39, 19), hwlib::xy(48, 19)), hwlib::line(hwlib::xy(39, 20), hwlib::xy(48, 20)),
              hwlib::line(hwlib::xy(39, 21), hwlib::xy(48, 21)), hwlib::line(hwlib::xy(40, 22), hwlib::xy(48, 22)),
              hwlib::line(hwlib::xy(40, 23), hwlib::xy(48, 23)), hwlib::line(hwlib::xy(40, 24), hwlib::xy(49, 24)),
              hwlib::line(hwlib::xy(40, 25), hwlib::xy(49, 25)), hwlib::line(hwlib::xy(40, 26), hwlib::xy(49, 26)),
              hwlib::line(hwlib::xy(41, 27), hwlib::xy(48, 27))};
          for (hwlib::line l : deagle_array) {
            l.draw(weapon_window);
          }
          current_weapon = 2;
        }
        drawBox(weapon_window, 62, 44);
        display_weapon << "\t0304" << textdisplay[weapon_number];
        weapon_window.flush();
        state = states::WAIT_FOR_CHANGE;
        break;
      }  // case update weapon
    }    // switch state
  }      // while true
}  // main