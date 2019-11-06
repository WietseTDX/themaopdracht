#ifndef PLAYERINFORMATION_HPP
#define PLAYERINFORMATION_HPP

#include <array>
#include "hwlib.hpp"

///@file

///\brief
/// PlayerInformation
///\details
/// Contains all the information about the player.
/// Used in MainController and WindowController
template <unsigned int N>
class PlayerInformation {
 private:
  uint8_t weapon = 0;
  uint16_t health_points = N;
  uint8_t player_number = 0;
  std::array<std::array<uint8_t, 2>, N> hits;
  uint8_t index_array = 0;
  int time = 0;

 public:
  ///\brief
  /// setWeapon
  ///\details
  /// max weapon = 9. everything above 9 gets set to 9.
  void setWeapon(uint8_t i) {
    if (i > 9) i = 9;
    weapon = i;
  }

  ///\brief
  /// getWeapon
  ///\details
  /// return weapon
  uint8_t getWeapon() { return weapon; }

  ///\brief
  /// setHealthPoints
  ///\details
  ///  max health_points = N. everything above N gets set to N.
  void setHealthPoints(uint16_t i) {
    if (i > N) i = N;
    health_points = i;
  }

  ///\brief
  /// getHeathPoints
  ///\details
  /// return health_points
  uint16_t getHealthPoints() { return health_points; }

  ///\brief
  /// setPlayerNumber
  ///\details
  /// max player_number = 9. everything above 9 gets set to 9.
  void setPlayerNumber(uint8_t i) {
    if (i > 9) i = 9;
    if (i == 0) i = 1;
    player_number = i;
  }

  ///\brief
  /// getPlayerNumber
  ///\details
  /// return player_number
  uint8_t getPlayerNumber() { return player_number; }

  ///\brief
  /// addHit
  ///\details
  /// add a hit to hits. Hit is based on player_number and damage
  void addHit(uint8_t player_number, uint8_t damage) {
    if (index_array == N - 1) {
      hwlib::cout << "ERROR: PlayerInformation.addHit(): Out of bounce!\n";
      return;
    }
    std::array<uint8_t, 2> array = {player_number, damage};
    hits[index_array] = array;
    index_array++;
  }

  /// \brief
  /// getAllHits
  /// \details
  /// return the hits array
  std::array<std::array<uint8_t, 2>, N> getAllHits() { return hits; }

  /// \brief
  /// <<operator
  /// \details
  /// write PlayerInformation to hwlib::cout
  friend hwlib::ostream& operator<<(hwlib::ostream& out, const PlayerInformation& playerInformation) {
    out << "=========================================\n";
    out << "Weapon: " << playerInformation.weapon << "\n";
    out << "Health: " << playerInformation.health_points << "\n";
    out << "Player: " << playerInformation.player_number << "\n";
    out << "Hits:\n";
    int player_points[9] = {0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < playerInformation.index_array; i++) {
      std::array<uint8_t, 2> shot = playerInformation.hits[i];
      player_points[shot[0]-1] += shot[1]+1;
    }
    for(int i = 0; i < 8; i++){
      out << "  Speler " << i+1 << " heeft in totaal " << player_points[i] << " damage gedaan!\n";
    }
    out << "=========================================";
    return out;
  }

  /// \brief
  /// getTime
  /// \details
  /// return the time
  int getTime() { return time; }

  /// \brief
  /// setTime
  /// \details
  /// set the time
  void setTime(int i) { time = i; }

  /// \brief
  /// restPlayer
  /// \details
  /// this function resets the PlayerInformation class
  void resetPlayer() {
    weapon = 0;
    health_points = N;
    player_number = 0;
    hits = {};
    index_array = 0;
    time = 0;
  }
};

#endif  // PLAYERINFORMATION_HPP