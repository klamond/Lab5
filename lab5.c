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

void outchar(char ch){
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

void outhex(char N){
  outchar(bin2hex((N & 0xF0) >> 4)); // send first hex digit
  outchar(bin2hex(N & 0x0F)); // send first hex digit
}

void main() {
  volatile int   *const HEXDISP = (int   *)0x000088A0; // hex data register 
  volatile short *const HEXCTRL = (short *)0x000088B0; // hex control register


    int	 count;	 		// define a counter

//    int test = 120;	// test value for hex display
//    outhex(test);	// test hex display

    *HEXCTRL = 0x01FF;   		// enable eight hex digits

    count = 0;			// initial count value

    while( 1 ) { 			// create an infinite loop

  *HEXDISP = count;		// send count to the hex display

    	count = count + 1;	// increment the count

    } // end while


/*  while loop condenced to one line
    while(1) {*HEXDISP = count++;} 
*/


} // end main
