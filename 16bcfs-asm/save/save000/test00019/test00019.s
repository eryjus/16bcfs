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