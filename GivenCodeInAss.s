.global _start
_start: br Start  # begin at the main program

# Constants
    .equ HEX_DISP, 0x88A0
    .equ HEX_CONT, 0x88B0

.org 0x0100

Start:
# r2 is the counter
    init:
        # enable all hex displays
        movia r3, HEX_CONT
        movi r4, 0xFF  # enable all hex displays
        stw r4, 0(r3)  # store to HEX_CONT

        movia r5, HEX_DISP  # r5 points to the first hex display
        movi r2, 0  # initialize counter to 0
    loop:
        stw r2, 0(r5)  # store the counter value to the hex display
        addi r2, r2, 1  # increment the counter
        br loop  # repeat the loop