;;===================================================================================================================
;;  test.s: Basic test for the brk instruction
;;
;;  At this point I can once again support NOP instructions and I am adding a BRK insturction.
;;
;;  Before the end of the video, I will also be adding back a JMP <immed> instruction to the mix.  This test will
;;  exercise all of the instructions for the emulator and ultimately for the hardware build.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2025-Feb-27  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================


    .org    0x0000

entry:
    brk
    nop

loop:
    brk
    nop
    nop
    nop
    jmp     loop

.dw 0xffff


