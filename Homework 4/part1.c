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

void button_interrupt(){
    button.rise(NULL);
    b1.buff[b1.index] = t.read();
    b1.index++;
    if(b1.index >= 10) b1.index = 0;
    wait_ms(BUTTON_DEBOUNCE_TIME_MS);
    button.rise(&button_interrupt);
}

void button_print(){
    for(int i = 0; i < 10; i++)
        printf("%f ", b1.buff[i]);
    printf("\n ");
}

int main() {
    button.rise(&button_interrupt);
    button2.rise(&button_print);
    t.start();
    
    /*
    while (1) {
        
        //printf("Hello World!\n");
        wait_ms(10);
        //t.reset();
    }
    */

    while(1) {
        led = led + 0.10;
        wait(0.2);
        if(led == 1.0) {
            led = 0;
        }
    }
}