;;===================================================================================================================
;;  test-ctrl.s: Test each of the control bits for the currently implemented instruction set
;;
;;  This test will walk through each of the possible implemented instructions to allow each to be compared against
;;  the intended control signals to make sure they are being handled properly.
;;
;;  If this works (not including any conditional execution), we should be able to execute each of the other
;;  test programs and have them work in the emulator.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2024-Aug-22  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================


    .org    0x0000

entry:
;; -- no operation instruction (0x0000)
    nop
    nop
    nop
    nop


;; -- mov an immediate value into R1 (0x0001)
    mov     r1,0x1234
    nop
    nop
    nop


;; -- mov an immediate value into R2 (0x0002)
    mov     r2,0x4321
    nop
    nop
    nop


;; -- clear carry (0x000e)
    clc
    nop
    nop
    nop


;; -- set carry (0x000f)
    stc
    nop
    nop
    nop


;; -- mov rz into r1 (0x0011)
    mov     r1,rz
    nop
    nop
    nop


;; -- mov rz into r2 (0x0012)
    mov     r2,rz
    nop
    nop
    nop


;; -- mov r1 into r2 (0x0022)
    mov     r2,r1
    nop
    nop
    nop


;; -- mov r2 into r1 (0x0031)
    mov     r1,r2
    nop
    nop
    nop


;; -- jump to an immediate address (0x00f0)
    jmp     here
here:
    nop
    nop
    nop


;; -- jump to the address at R1 (0x00f1)
    jmp     r1
    nop
    nop
    nop


;; -- jump to the address at R2 (0x00f2)
    jmp     r2
    nop
    nop
    nop


;; -- add an immediate to R1 (0x0101)
    add     r1,0xaaaa
    nop
    nop
    nop


;; -- add an immediate to R2 (0x0102)
    add     r2,0xaaaa
    nop
    nop
    nop


;; -- add R1 to R1 (0x0110)
    add     r1,r1
    nop
    nop
    nop


;; -- add R2 to R1 (0x0111)
    add     r1,r2
    nop
    nop
    nop


;; -- add R1 to R2 (0x011c)
    add     r2,r1
    nop
    nop
    nop


;; -- add R2 to R2 (0x011d)
    add     r2,r2
    nop
    nop
    nop


;; -- add with carry an immediate to R1 (0x01b1)
    adc     r1,0x5555
    nop
    nop
    nop


;; -- add with carry an immediate to R2 (0x01b2)
    adc     r2,0x5555
    nop
    nop
    nop


;; -- add with carry R1 to R1 (0x01c0)
    adc     r1,r1
    nop
    nop
    nop


;; -- add with carry R2 to R1 (0x01c1)
    adc     r1,r2
    nop
    nop
    nop


;; -- add with carry R1 to R2 (0x01cc)
    adc     r2,r1
    nop
    nop
    nop


;; -- add with carry R2 to R2 (0x01cd)
    adc     r2,r2
    nop
    nop
    nop


;; -- increment r1 (0x0261)
    inc     r1
    nop
    nop
    nop


;; -- increment r1 (0x0262)
    inc     r2
    nop
    nop
    nop


;; -- increment r1 non-ALU (0x0ff0)
    incr    r1
    nop
    nop
    nop


;; -- increment r2 non-ALU (0x0ff1)
    incr    r2
    nop
    nop
    nop


;; -- decrement r1 non-ALU (0x0ff2)
    decr    r1
    nop
    nop
    nop


;; -- decrement r2 non-ALU (0x0ff3)
    decr    r2
    nop
    nop
    nop


    jmp     entry

