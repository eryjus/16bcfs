;;===================================================================================================================
;; test-mov.s: Perform several move instructions to the R1 register, each an immediate load
;;
;;  This code will produce a Larson Scanner (Knight Riger effect) using a brute force load of each light into
;;  the R1 register in turn.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2023-Feb-28  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================


    .org    0x0000

entry:
    mov     r1,0x0001
    mov     r2,0x8000

    mov     r1,0x0002
    mov     r2,r1

    mov     r1,0x0004
    mov     r2,0x2000

    mov     r1,0x0008
    mov     r2,r1

    mov     r1,0x0010
    mov     r2,0x0800

    mov     r1,r2

    mov     r2,0x0000
    jmp     r2
