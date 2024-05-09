//===================================================================================================================
//  test-adder.s: Test the ALU Adder ADD, ADC, and INC functions with flags
//
//  This test will test the ALU Adder with the ADD, ADC, and INC instructions.  Included with this suite of tests
//  will include architected tests which will manipulate all the flags, testing the condition for each.
//
//  This test is expected to take more than the currently addressable 64 word address space in ROM memory, so
//  it is anticipated that an expansion is needed.
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2023-Sep-25  Initial  v0.0.1   ADCL  Initial version
//
//===================================================================================================================

%arch       16bcfs.arch

    .org    0x0000

entry:
    // -- Test #1: Add 2 numbers together and store the result in R1
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xaaaa
    mov     r2,0x5555
    add     r1,r2                   // result 0xffff
    nop

    // -- Test #2: Add 2 numbers together and store the result in r2
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0x6666
    mov     r2,0x9999
    add     r2,r1                   // result 0xffff
    nop

    // -- Test #3: Add an immediate to R1
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xcccc
    add     r1,0x3333               // result 0xffff
    nop

    // -- Test #4: Add an immediate to R2
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r2,0x36c5
    add     r2,0xc93a               // result 0xffff

    // -- Test #5: Add 2 numbers together with carry (set) => R1
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xaaaa
    mov     r2,0x5555
    adc     r1,r2                   // result 0x0000 with C set
    nop

    // -- Test #6: Add 2 numbers together with carry (clear) => R1
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    clc
    mov     r1,0xaaaa
    mov     r2,0x5555
    adc     r1,r2                   // result 0xffff with C clear
    nop

    // -- Test #7: Add 2 numbers together with carry (set) => R2
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0x6666
    mov     r2,0x9999
    adc     r2,r1                   // result 0x0000 with C set
    nop

    // -- Test #8: Add 2 numbers together with carry (clear) => R2
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    clc
    mov     r1,0x6666
    mov     r2,0x9999
    adc     r2,r1                   // result 0xffff with C clear
    nop

    // -- Test #9: Add an immediate to R1 with carry set
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xcccc
    adc     r1,0x3333               // result 0x0000 with C set
    nop

    // -- Test #10: Add an immediate to R1 with carry clear
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    clc
    mov     r1,0x36c5
    adc     r1,0xc93a               // result 0xffff with C clear

    // -- Test #11: Add an immediate to R2 with carry set
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xcccc
    adc     r1,0x3333               // result 0x0000 with C set
    nop

    // -- Test #12: Add an immediate to R2 with carry clear
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    clc
    mov     r2,0x36c5
    add     r2,0xc93a               // result 0xffff with C clear

    // -- Test #13: Increment R1
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,rz                   // clear r1
    inc     r1
    inc     r1
    inc     r1
    nop

    // -- Test #14: Increment R2
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r2,rz                   // clear r2
    inc     r2
    inc     r2
    inc     r2
    nop

    // -- Test #15: Add R1 to itself as in shift left into C
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    clc
    mov     r1,0xaaaa
    add     r1,r1                   // result 0x5554 with C set
    nop

    // -- Test #16: Add R2 to itself as in shift left into C
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    clc
    mov     r2,0x5555
    add     r2,r2                   // result 0xaaa8 with C clear
    nop

    // -- Test #17: Add R1 to itself as in shift left into C with C set
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xaaaa
    adc     r1,r1                   // result 0x5555 with C set
    nop

    // -- Test #16: Add R2 to itself as in shift left into C with C set
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r2,0x5555
    adc     r2,r2                   // result 0xaaa9 with C clear
    nop

    //
    // -- From here on, we assume C is fully exercised
    //    --------------------------------------------

    // -- Test #17: Add 2 numbers which do not result in 0
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xaaaa
    add     r1,0x5555               // Z clear
    nop

    // -- Test #18: Add 2 numbers which result in 0
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xaaaa
    adc     r1,0x5555               // Z set
    nop

    // -- Test #19: Add 2 numbers do not result in a negative number
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0x2aaa
    add     r1,0x5555               // result is 0x7fff with N clear
    nop

    // -- Test #20: Add 2 numbers which result in a negative number
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    stc
    mov     r1,0xaaaa
    adc     r1,0x5555               // N set
    nop

    // -- Test #21: Add 2 numbers do not result in an Overflow
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    mov     r1,0x0aaa
    add     r1,0x0fff               // result is 0x1aa9 with V clear
    nop

    // -- Test #22: Add 2 numbers which result in an overflow
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    mov     r1,0x7fff
    inc     r1                      // result 0x8000 with V set
    nop

    // -- Test #23: Add 2 numbers do not result in Less Than (or where N = V)
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    mov     r1,0x7fff
    inc     r1                      // result 0x8000 with V, N set, L Clear
    nop

    // -- Test #24: Add 2 numbers which result in an overflow
    mov     r1,rz
    mov     r2,rz
    add     r1,r2
    mov     r1,0x7fff
    add     r1,0xffff               // result 0x0xfffe with V, L set, N clear
    nop


    jmp     0x0000                  // fetch assert main; pc load & latch; inc pc

