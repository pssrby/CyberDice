#include<stdint.h>
#include "mpu6050_i2c.h"
#include "oled.h"
#include "bmp.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>
typedef enum mode{
    reset, 
    sleep, 
    clock, 
    count_down, 
    dice_roll, 
    dice_stop
}mode_t;

typedef enum faceup{
    XU, XD, YU, YD, ZU, ZD
}faceup_t;

typedef struct dice_state* dice_state_t;
struct dice_state{
    int16_t accl[3];
    int16_t gyro[3];
    int16_t tmp;
};
void mpu6050_read_state(dice_state_t current_state);
faceup_t which_side(dice_state_t state);

uint8_t dice_roll_handler(uint8_t dice_num, int16_t x_degree);
void dice_stop_handler(uint8_t dice_num);

void mpu6050_read_state(dice_state_t current_state){
    mpu6050_read_raw(current_state->accl, current_state->gyro, &(current_state->tmp));
}

faceup_t which_side(dice_state_t state){
    return XD;
}

uint8_t dice_roll_handler(uint8_t dice_num, int16_t x_degree){
    // display a number, 
    // either the previous displayed or increased
    x_degree = x_degree > 0 ? x_degree : -x_degree;
    dice_num = dice_num + x_degree / 100;
    dice_num = dice_num % 10;
    // MAX7219_disp_num(dice_num);
    OLED_display_num(dice_num);
    return dice_num;
}
void dice_stop_handler(uint8_t dice_num){
    // MAX7219_disp_num(dice_num);
    OLED_display_num(dice_num);
}

void core1_main(){

}

int main(void){
    stdio_init_all();
    OLED_GPIO_INIT();
 	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示LED

    static struct dice_state dice_state_0, dice_state_1;
    dice_state_t current_state = &dice_state_0;
    dice_state_t previous_state = &dice_state_1;
    dice_state_t temp_state;
    static mode_t currunt_mode, next_mode;
    next_mode = reset;
    MPU6050_INIT();
    mpu6050_read_state(current_state);
    int16_t x_degree;
    uint8_t dice_num = 0;
    while(1){
        currunt_mode = next_mode;
        temp_state = previous_state;
        previous_state = current_state;
        current_state = temp_state;
        mpu6050_read_state(current_state);
        x_degree = current_state->gyro[0] - previous_state->gyro[0];
        switch(currunt_mode){
            case reset:
                next_mode = dice_stop;
            break;
            case sleep:
            break;
            case clock:
            break;
            case count_down:
            break;
            case dice_stop:
                dice_stop_handler(dice_num);
                if(x_degree < 1000 && x_degree > -1000) next_mode = dice_stop;
                else next_mode = dice_roll;
            break;
            case dice_roll:
                dice_num = dice_roll_handler(dice_num, x_degree);
                if(x_degree < 1000 && x_degree > -1000) next_mode = dice_stop;
                else next_mode = dice_roll;
            break;
        }
    }
}