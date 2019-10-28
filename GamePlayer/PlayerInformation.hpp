#ifndef PLAYERINFORMATION_HPP
#define PLAYERINFORMATION_HPP

#include "hwlib.hpp"
#include <array>

template< unsigned int N >
class PlayerInformation{
private:
    uint8_t weapon  = 0;
    uint16_t health_points = N;
    uint8_t player_number = 0;
    std::array<std::array<uint8_t, 2>, N> hits;
    uint8_t index_array = 0;
public:
    void PlayerInformation::setWeapon(uint8_t i){
        weapon = i;
    }
    uint8_t PlayerInformation::getWeapon(){
        return weapon;
    }
    void PlayerInformation::setHealthPoints(uint16_t i){
        health_points = i;
    }
    uint16_t PlayerInformation::getHealthPoints(){
        return health_points;
    }
    void PlayerInformation::setPlayerNumber(uint8_t i){
        player_number = i;
    }
    uint8_t PlayerInformation::getPlayerNumber(){
        return player_number;
    }
    void PlayerInformation::addHit(uint8_t player_number, uint8_t damage){
        if(index_array == 99){hwlib::cout << "ERROR: PlayerInformation.addHit(): Out of bounce!\n";return;}
        std::array<uint8_t, 2> array = {player_number, damage};
        hits[index_array] = array;
        index_array++;
    }
    std::array<std::array<uint8_t, 2>, N>(){
        return hits;
    }
};


#endif //PLAYERINFORMATION_HPP