/*** 
CMPE3221 LAB#5 - C LANGUAGE EXERCISE                       
----------------------------------------------- 
November 2010	NAME:
----------------------------------------------- 
This program is the starting point for LAB#5
It enables the hex display and creates a counter.
-----------------------------------------------
***/

#include <stdio.h>

// Function to output a character to the Altera Monitor terminal
void outchar(char ch) {
    printf("outchar: %c\n", ch); // Debug print
    volatile int *JTAG_UART = (int *)0x10001000;
    while (!(*(JTAG_UART + 1) & 0xFFFF)); // Wait for ready status
    *JTAG_UART = ch; // Send character
}

// Function to convert a 4-bit value to ASCII hex character
char bin2hex(char N) {
    printf("bin2hex: Input = %X\n", N); // Debug print
    N &= 0x0F; // Mask lower 4 bits
    char result = (N < 10) ? (N + '0') : (N - 10 + 'A');
    printf("bin2hex: Output = %c\n", result); // Debug print
    return result;
}

// Function to display a byte as two ASCII hex characters
void outhex(char N) {
    printf("outhex: Input = %X\n", N); // Debug print
    outchar(bin2hex((N >> 4) & 0x0F)); // Upper nibble
    outchar(bin2hex(N & 0x0F));       // Lower nibble
}

int main() {
    volatile int   *const HEXDISP = (int   *)0x000088A0; // Display hex digits 
    volatile short *const HEXCTRL = (short *)0x000088B0; // Hex control register   
    int count; // Define a counter

    *HEXCTRL = 0x01FF; // Enable eight hex digits

    count = 0; // Initial count value

    while (1) {
        printf("Counter: %d\n", count); // Debug print
        *HEXDISP = count; // Send count to the hex display
        count = count + 1; // Increment the count
    } // End while

    return 0;
} // End main
