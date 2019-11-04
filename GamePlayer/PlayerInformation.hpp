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
    }
    uint8_t getWeapon(){
        return weapon;
    }
    void setHealthPoints(uint16_t i){
        health_points = i;
    }
    uint16_t getHealthPoints(){
        return health_points;
    }
    void setPlayerNumber(uint8_t i){
        player_number = i;
    }
    uint8_t getPlayerNumber(){
        return player_number;
    }
    void addHit(uint8_t player_number, uint8_t damage){
        if(index_array == N-1){hwlib::cout << "ERROR: PlayerInformation.addHit(): Out of bounce!\n";return;}
        std::array<uint8_t, 2> array = {player_number, damage};
        hits[index_array] = array;
        index_array++;
    }
    std::array<std::array<uint8_t, 2>, N> getAllHits(){
        return hits;
    }

    friend hwlib::ostream& operator<<(hwlib::ostream& out, const PlayerInformation & playerInformation){
		out << "Weapon: " << playerInformation.weapon << "\n";
        out << "Health: " << playerInformation.health_points << "\n";
        out << "Player: " << playerInformation.player_number << "\n";
        out << "Hits: { ";
        for(std::array<uint8_t,2> shot : playerInformation.hits){
            if(shot[0] == 0){break;}
            out << "[" << shot[0] << ", " << shot[1] << "] ";
        }
        out << "}";
		return out;
	}
};


#endif //PLAYERINFORMATION_HPP