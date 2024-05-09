//===================================================================================================================
// test-pc.s: Test the Program Counter PCB
//
//  This test will will jump to the end of address space, which should turn on all LEDs in the PC.
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2023-Jun-04  Initial  v0.0.1   ADCL  Initial version
//
//===================================================================================================================

%arch       16bcfs.arch

    .org    0x0000

entry:
    nop
    nop

    jmp     0xffff              // loop back and execute the test again


