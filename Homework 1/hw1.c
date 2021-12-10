#include <REG51F.H>
#include <stdio.h>

#define ARRAY_SIZE 5

struct mf_list
{
		//These global variables can be seen in memory
    int arr[ARRAY_SIZE];
		int index;
    int currentSize;
};

int append_left(int input, struct mf_list *list)
{
    unsigned int i;
    if (list->currentSize == ARRAY_SIZE)
        return -1;
    for (i = list->currentSize; i > 0; i--)
    {
        list->arr[i] = list->arr[i - 1];
    }

    list->arr[0] = input;

    list->currentSize++;

    return list->currentSize;
}

int append_right(int input, struct mf_list *list)
{
    if (list->currentSize == ARRAY_SIZE)
        return -1;

    list->arr[list->currentSize] = input;

    list->currentSize++;

    return list->currentSize;
}

int pop_left(struct mf_list *list)
{
    unsigned int i;
    if (list->currentSize == 0)
        return -1;

    for (i = 0; i < (list->currentSize - 1); i++)
    {
        list->arr[i] = list->arr[i + 1];
    }

    list->arr[--list->currentSize] = 0;

    return list->currentSize;
}

int pop_right(struct mf_list *list)
{
    if (list->currentSize == 0)
        return -1;

    list->arr[--list->currentSize] = 0;

    return list->currentSize;
}

int length(struct mf_list *list)
{
    return list->currentSize;
}

int seek_left(struct mf_list *list)
{
    if (list->currentSize == 0)
        return -1;

    list->index--;

    return list->currentSize;
}

int seek_right(struct mf_list *list)
{
    if (list->index == (list->currentSize - 1) || list->currentSize == 0)
        return -1;

    list->index++;

    return list->currentSize;
}

unsigned char display_on_led(unsigned int i){
	unsigned char led;
	switch(i){
		case 0:
			led = 0xc0;
			break;
		case 1:
			led = 0xf9;
			break;
		case 2:
			led = 0xa4;
			break;
		case 3:
			led = 0xb0;
			break;
		case 4:
			led = 0x99;
			break;
		case 5:
			led = 0x92;
			break;
		case 6:
			led = 0x82;
			break;
		case 7:
			led = 0xf8;
			break;
		case 8:
			led = 0x80;
			break;
		case 9:	
			led = 0x90;		
			break;
		default:
			led = 0x00;
	}
		
		return led;
}

int main(void)
{
		unsigned int i;
		unsigned int x;
		
		//Initialization of the list
    struct mf_list d1;
    d1.currentSize = 0;
    d1.index = 0;

		//Final list after these operations should be [1,3,5,7,9]
    append_left(3, &d1);
		append_right(0xAA, &d1);
		pop_right(&d1);
		append_left(1, &d1);
    append_right(5, &d1);
    append_right(7, &d1);
    append_right(9, &d1);
		pop_left(&d1);
		append_left(1, &d1);
	

		//Final index must be (0 + 1 + 1 + 1 - 1) = 2. Can also be seen in memory
    seek_right(&d1);
    seek_right(&d1);
		seek_right(&d1);
		seek_left(&d1);


    //Displays the array in order
		P1 = 0x00;

		while(1){
			for( i = 0; i < length(&d1); i++){
				P1=display_on_led(d1.arr[i]);
				for(x = 0;  x < 20000; x++){}	
			}
		}
}

	
		
