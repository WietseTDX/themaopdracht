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
    void setWeapon(uint8_t i){
        weapon = i;
        hwlib::cout << "setWeapon " << i << hwlib::endl;
    }
    uint8_t getWeapon(){
        hwlib::cout << "getWeapon" << hwlib::endl;
        return weapon;
    }
    void setHealthPoints(uint16_t i){
        health_points = i;
        hwlib::cout << "setHealthPoints " << i << hwlib::endl;
    }
    uint16_t getHealthPoints(){
        hwlib::cout << "getHealthPoints" << hwlib::endl;
        return health_points;
    }
    void setPlayerNumber(uint8_t i){
        player_number = i;
        hwlib::cout << "setPlayernumber " << i << hwlib::endl;
    }
    uint8_t getPlayerNumber(){
        hwlib::cout << "Get playerNumber" << hwlib::endl;
        return player_number;
    }
    void addHit(uint8_t player_number, uint8_t damage){
        if(index_array == 99){hwlib::cout << "ERROR: PlayerInformation.addHit(): Out of bounce!\n";return;}
        std::array<uint8_t, 2> array = {player_number, damage};
        hits[index_array] = array;
        index_array++;
        hwlib::cout << "addHit" << hwlib::endl;
    }
    std::array<std::array<uint8_t, 2>, N> hi(){
        hwlib::cout << "hi" << hwlib::endl;
        return hits;
    }
};


#endif //PLAYERINFORMATION_HPP