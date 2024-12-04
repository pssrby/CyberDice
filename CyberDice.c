#include "mpu6050_i2c.h"
#include "max7219.h"
enum fsm_state{
    reset, 
    sleep, 
    clock, 
    count_down, 
    dice
};

enum dice_state{
    XU, XD, YU, YD, ZU, ZD
};

typedef struct fsm fsm_t;
struct fsm{
    enum fsm_state current_state;
    enum fsm_state next_state;
};



int main(void){
    fsm_t fsm_i;
    fsm_i.next_state = dice;
    while(1){
        fsm_i.current_state = fsm_i.next_state;
        switch (fsm_i.current_state)
        {
        case reset:
            break;
        case dice:
            dice_mode_handler();
            break;
        default:
            break;
        }
    }
}