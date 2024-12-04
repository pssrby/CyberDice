#include<stdint.h>
#include "mpu6050_i2c.h"
#include "max7219.h"
enum fsm_state{
    reset, 
    sleep, 
    clock, 
    count_down, 
    dice
};

typedef enum faceup{
    XU, XD, YU, YD, ZU, ZD
}faceup_t;

typedef struct dice_state* dice_state_t;
struct dice_state{
    int16_t accl[3];
    int16_t gyro[3];
    int16_t tmp;
};

typedef struct fsm* fsm_t;
struct fsm{
    enum fsm_state current_state;
    enum fsm_state next_state;
};

void mpu6050_read_state(dice_state_t current_state){
    mpu6050_read_raw(current_state->accl, current_state->gyro, &(current_state->tmp));
}

faceup_t which_side(dice_state_t state){
    return XD;
}

int dice_mode_handler(dice_state_t current_state, dice_state_t previous_state){
    // display a number, 
    // either the previous displayed or increased
    int num = 0;
    MAX7219_disp_num(num);
    return num;
}

int main(void){
    static struct dice_state dice_state_0, dice_state_1;
    dice_state_t current_state = &dice_state_0;
    dice_state_t previous_state = &dice_state_1;
    dice_state_t temp_state;
    while(1){
        temp_state = previous_state;
        previous_state = current_state;
        current_state = temp_state;
        mpu6050_read_state(current_state);
        dice_mode_handler(current_state, previous_state);
    }
}