/*** 
CMPE3221 LAB#5 - C LANGUAGE EXERCISE                       
----------------------------------------------- 
November 2010	NAME:
----------------------------------------------- 
This program is the starting point for LAB#5
It enables the hex display and creates a counter.
-----------------------------------------------
***/

main ( ) {

volatile int   *const HEXDISP = (int   *)0x000088A0; // display hex digits 
volatile short *const HEXCTRL = (short *)0x000088B0; // hex control register  	
  int	 count;	 		// define a counter

  *HEXCTRL = 0x01FF;   		// enable eight hex digits

  count = 0;			// initial count value

  while( 1 ) { 			// create an infinite loop

*HEXDISP = count;		// send count to the hex display

  	count = count + 1;	// increment the count

  } // end while

} // end main
