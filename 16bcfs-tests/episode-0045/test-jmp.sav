;;===================================================================================================================
;; test-jmp.s: Execute an unconditional jmp to an address
;;
;;  This code will execute half of the Larson Scanner (Knight Riger effect) from episode-0042.  This will be done
;;  by executing an unconditional jump back to the start of the program half-way through the scanner.  This will
;;  make the scanner look like it's wrapping from one side to the other.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2023-Mar-26  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================


    .org    0x0000

entry:
    mov     r1,0x0001
    mov     r1,0x0002
    mov     r1,0x0004
    mov     r1,0x0008
    mov     r1,0x0010
    mov     r1,0x0020
    mov     r1,0x0040
    mov     r1,0x0080
    mov     r1,0x0100
    mov     r1,0x0200
    mov     r1,0x0400
    mov     r1,0x0800
    mov     r1,0x1000
    mov     r1,0x2000
    mov     r1,0x4000
    mov     r1,0x8000

    jmp     0

    mov     r1,0x8000
    mov     r1,0x4000
    mov     r1,0x2000
    mov     r1,0x1000
    mov     r1,0x0800
    mov     r1,0x0400
    mov     r1,0x0200
    mov     r1,0x0100
    mov     r1,0x0080
    mov     r1,0x0040
    mov     r1,0x0020
    mov     r1,0x0010
    mov     r1,0x0008
    mov     r1,0x0004
    mov     r1,0x0002
    mov     r1,0x0001
