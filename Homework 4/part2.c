#include "mbed.h"
#define BUTTON_DEBOUNCE_TIME_MS 20

typedef struct clock_buffer{
    float buff[10] = {0};
    int index = 0;
};

Timer t;
PwmOut led(p7);
InterruptIn button(p5);
InterruptIn button2(p6);
clock_buffer b1;
float brightness;

float set_brightness(struct clock_buffer* cb);

void button_interrupt(){
    button.rise(NULL);
    b1.buff[b1.index] = t.read();
    b1.index++;
    if(b1.index >= 10) b1.index = 0;
    wait_ms(BUTTON_DEBOUNCE_TIME_MS);
    button.rise(&button_interrupt);
    brightness = set_brightness(&b1);
}

void button_print(){
    for(int i = 0; i < 10; i++)
        printf("%f ", b1.buff[i]);
    printf("\n ");
}

float set_brightness(struct clock_buffer* cb){
    int count = 0;
    float b;
    float timestamp = t.read();
    button.rise(NULL);
    for(int i = 0; i < 10; i++){
        if(abs(timestamp - 5.0) <= cb->buff[i])
            count++;
    }
    button.rise(&button_interrupt);
    if(count == 0)
        return -1.0;
    else{
        b = (count - 1) * 0.11;
        led = b;
        return b;
    }
}

int main() {
    button.rise(&button_interrupt);
    button2.rise(&button_print);
    t.start();
    
    while (1) {
        wait_ms(20);
        if(brightness >= 0){
            printf("Led Value = %f \n",brightness);
        }
    }
}