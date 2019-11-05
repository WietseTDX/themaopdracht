#include "WindowController.hpp"
#include "PlayerInformation.hpp"
//========================================
//OTHER FUNCTIONS
//========================================

constexpr float power(float x, int power_int){ 
    if(power_int == 0){return 1;} 
    if(power_int < 0){return (1/power(x,power_int*-1));}
    float total = x;
    for(float i = 0; i < power_int-1; i++){
        total *= x;
    }
    return total;
}

constexpr float sqrt(float x){
    float sqrt = x/2;
    float temp = 0;
    int counter = 0;
    while(sqrt != temp){
        temp = sqrt;
        sqrt = (x / temp + temp) / 2;
        counter++;
        if(counter > 260000){sqrt = 0; break;}
    }
    return sqrt;
}

constexpr float makeabsolute(float x){
    if(x < 0){x *= -1;}
    return x;
}

constexpr hwlib::xy hearttop(int x){
    float calculate = makeabsolute(x * 0.1 - 2);
    calculate = power(calculate-1, 2);
    calculate = sqrt(1-calculate) * 10;
    int y = (makeabsolute(calculate-10)+0.5);
    return hwlib::xy(x+7, y+2);
}

constexpr hwlib::xy heartbottom(int x){
    float calculate = makeabsolute(x * 0.1 - 2);
    calculate = sqrt(calculate)/sqrt(2);
    calculate = -3 * sqrt(1-calculate) * 10 - 10;
    int y = (makeabsolute(calculate)+0.5);
    return hwlib::xy(x+7, y+2);
}

template< int N, typename T >
class lookup {
private:
    
public:
    T values[N];
    template< typename F >
    constexpr lookup( F function ):
        values()
    {
        for( int i = 0; i < N; ++i ){
            values[ i ] = function( i );
        }
    }
    constexpr T get( int n ) const {
        return values[ n ];    
    }
};

constexpr auto top_heart_values = lookup<42, hwlib::xy>(hearttop);
constexpr auto bottom_heart_values = lookup<42, hwlib::xy>(heartbottom);

//========================================
//PRIVATE FUNCTIONS WindowController
//========================================

void WindowController::drawBox(hwlib::window_part_t & window, int x, int y){
        hwlib::rectangle(hwlib::xy(0,0), hwlib::xy(x, y)).draw(window);
    }

//========================================
//PUBLIC FUNCTIONS WindowController
//========================================

void WindowController::update(int window_number){
    WindowChannel.write(window_number);
}

//========================================
//MAIN FUNCTION WindowController
//========================================

void WindowController::main() {
    state = states::WAIT_FOR_CHANGE;
    window.clear();
    while(true){
        switch( state ){
            case states::WAIT_FOR_CHANGE:
            {
                wait(WindowChannel);
                auto window_update = WindowChannel.read();
                if(window_update == 0){
                    state = states::UPDATE_TIME;
                }else if(window_update == 1){
                    state = states::UPDATE_PLAYER_INFORMATION;
                }else if(window_update == 2){
                    state = states::UPDATE_HEALTH;
                }else{
                    state = states::UPDATE_WEAPON;
                }
                break;
            }//case wait for change
            case states::UPDATE_TIME:
            {
                auto time = info.getTime();
                display_time << "\f" << textdisplay[time/600] << textdisplay[time/60%10] << ':' << textdisplay[time%60/10] << textdisplay[time%60%10];
                drawBox(time_window, 79,16);
                time_window.flush();
                state = states::WAIT_FOR_CHANGE;
                break;
            }//case update time
            case states::UPDATE_HEALTH:
            {
                int health_points = info.getHealthPoints();
                display_health << "\f" << "\t0202" << textdisplay[health_points/100] << textdisplay[health_points%100/10] << textdisplay[health_points%10];
                for(int i = 0; i < 41; i++){
                    hwlib::line(top_heart_values.get(i), top_heart_values.get(i+1)).draw(health_window);
                    hwlib::line(bottom_heart_values.get(i), bottom_heart_values.get(i+1)).draw(health_window);
                }
                drawBox(health_window, 63, 44);
                health_window.flush();
                state = states::WAIT_FOR_CHANGE;
                break;
            }//case update health
            case states::UPDATE_PLAYER_INFORMATION:
            {
                int player_number = info.getPlayerNumber();
                display_player << "\f" << "P:" << textdisplay[player_number];
                drawBox(player_number_window, 46, 16);
                player_number_window.flush();
                state = states::WAIT_FOR_CHANGE;
                break;
            }//case update player information
            case states::UPDATE_WEAPON:
            {
                drawBox(weapon_window, 62, 44);
                weapon_window.flush();
                state = states::WAIT_FOR_CHANGE;
                break;
            }//case update weapon
        }//switch state
    }//while true
}//main