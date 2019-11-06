#ifndef GAMERULES_HPP
#define GAMERULES_HPP

class GameRules {
    private:
        int game_time = 0;
        int max_lives = 100;
    
    public:
        int getGameTime(){
            return game_time;
        }

        void setGameTime(int time){
            game_time = time;
        }

        int getMaxLives(){
            return max_lives;
        }

        void setMaxLives(int lives){
            max_lives = lives;
        }
};

#endif // GAMERULES_HPP