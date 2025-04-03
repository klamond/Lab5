# include <stdio.h> // include standard I/O library


/*** 
CMPE3221 LAB#5 - C LANGUAGE EXERCISE                       
----------------------------------------------- 
November 2010	NAME: 
----------------------------------------------- 
This program is the starting point for LAB#5
It enables the hex display and creates a counter.
-----------------------------------------------
***/

volatile int   *const HEXDISP = (int   *)0x000088A0; // hex data register 
volatile short *const HEXCTRL = (short *)0x000088B0; // hex control register
volatile char  *const PS2COM  = (char  *)0x000088F0; // PS2 communication register
volatile short *const REDLED  = (short *)0x00008880; // Red LED register


void outchar(char ch){ // send character to UART
  volatile int *const UART = (int *)0x00008840; // UART register  
  *UART = ch; // send character to UART
}

char bin2hex(char N){
// take 4 LSB of N, convert to hex, return   
  N &= 0x0F; // mask to get 4 LSB
  if (N < 10) {

    printf("bin2hex: Output = %c\n", N + 0x30); // Debug print

    return N + '0'; // convert to ASCII for digits 0-9
  } 
  else {

    printf("bin2hex: Output = %c\n", N + 0x37); // Debug print

    return N - 10 + 'A'; // convert to ASCII for letters A-F
  }
}

void outhex(char N){ // send hex value to UART
  outchar(bin2hex((N & 0xF0) >> 4)); // send first hex digit
  outchar(bin2hex(N & 0x0F)); // send first hex digit
}

char read_clock(){
    return (*PS2COM) & 0x02 >> 1; // read clock signal & right shift to LSB
}

char falling_edge(){
    char prev_clock = read_clock(); // read clock signal
    char cur_clock;

    while(1){
        cur_clock = read_clock(); // read clock signal

        if(prev_clock == 1 && cur_clock == 0){
            return 1; // falling edge detected
        }
        else return 0; // no falling edge detected
    }
}

char read_data(){
    return (*PS2COM) & 0x01; // read data signal & right shift to LSB
}

char get_key(){
    int i = 0; // index for buffer
    char buffer[10]; // buffer to store data
    while(i < 10){
        if((read_data == 0 && falling_edge == 1)){
            if (falling_edge == 1){
                buffer[i] = read_data(); // read data from PS2COM
                i++; // increment index
            }
        }
    }
    // print first 8 bits of buffer to hex display
    short boofer = 0;
    for (int j = 0; j < 8; j++) {
        boofer |= (buffer[j] << j);  // Shift the current bits in the buffer to the correct position
    }

}
/*
void main2() {
    *HEXCTRL = 0x01FF;   		// enable eight hex digits
    int	 count = 0;	 		// define a counter

// loop read_clock until prev_clock is 1 and clock is 0
    while(1){
        if(falling_edge == 1){
            count++; // increment the count
            *HEXDISP = count;	// send count to the hex display
        }

    }

} // end main
*/

void main3() {
// wait until falling edge == 1 && data == 0, count 10 falling edges putting data into 'buffer'
int i = 0; // index for buffer
char buffer[10]; // buffer to store data

while(1){
    while(i < 10){
        if((read_data == 0 && falling_edge == 1)){
            if (falling_edge == 1){
                buffer[i] = read_data(); // read data from PS2COM
                i++; // increment index
            }
        }
    }
    // print first 9 bits of buffer to hex display
    short boofer = 0;
    for (int j = 0; j < 9; j++) {
        boofer |= (buffer[j] << j);  // Shift the current bits in the buffer to the correct position
        printf("buffer[%d] = %d\n", j, buffer[j]); // Debug print
    }
    *HEXDISP = boofer; // send buffer to hex display
    
}
} // end main











/*
void main4() { 
    while(1){
        *HEXDISP = get_key; // clear hex display
    }
} // end main

void main5() {
    int recent_keys[4] = {0}; // array to store 4 most recent keys
    int index = 0; // index for the array

    while (1) {
        recent_keys[index] = get_key(); // get the key and store it in the array
        index = (index + 1) % 4; // update index in a circular manner

        int four_scan = 0;
        for (int i = 0; i < 4; i++) {
            four_scan |= (recent_keys[i] << (8 * (3 - i))); // shift and combine keys
        }

        *HEXDISP = four_scan; // send the combined value to the hex display
    }
}

void main6() {
    int recent_keys[4] = {0}; // array to store 4 most recent keys
    int index = 0; // index for the array

    while (1) {
        recent_keys[index] = get_key(); // get the key and store it in the array
        outhex(recent_keys[index]); // send the key to the hex display
        index = (index + 1) % 4; // update index in a circular manner

        int four_scan = 0;
        for (int i = 0; i < 4; i++) {
            four_scan |= (recent_keys[i] << (8 * (3 - i))); // shift and combine keys
        }

        *HEXDISP = four_scan; // send the combined value to the hex display
    }
}

void main7() {
    // if L or R shift pushed (0xF0), ignore immediate next scancode
    // tug of war with red leds
    // read key, if 

}
*/