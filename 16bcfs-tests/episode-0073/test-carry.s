//===================================================================================================================
//  test-carry.s: Test the ALU Carry Flag and conditional execution
//
//  This test will test the ability to manipulate the Carry Flag directly (STC/CLC) and some conditional execution
//  based on the status of the carry flag.
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2023-Sep-16  Initial  v0.0.1   ADCL  Initial version
//
//===================================================================================================================

%arch       16bcfs.arch

    .org    0x0000

entry:
    stc                             // set the carry flag
    nop
    nop

    clc
    nop
    nop


    mov     r1,0x0000               // force R1 to be clear
    mov-cc  r1,0xffff               // Carry should be clear, this should execute 100% of the time
    mov-cs  r1,0x5555               // Carry should be clear, this should never execute

    stc
    mov-cs  r1,0xaaaa               // this is a conditional move immediately after a change in C.  it should exec

    clc
    mov-cs  r1,0x5a5a               // this is a conditional move immediately after a change.  this should not exec

    //
    // -- now for a string of conditionals which will all succeed
    //    -------------------------------------------------------

    stc
    mov-cs  r2,r1
    mov-cs  r2,0x0000
    clc-cs

    // -- and these should all fail (check that the instruction register is correct after each one!)
    mov-cs  r2,r1
    mov-cs  r2,0x0000
    stc-cs

    jmp-cs  0x0000
    jmp-cc  0x0000

    jmp     0x0000                  // fetch assert main; pc load & latch; inc pc

