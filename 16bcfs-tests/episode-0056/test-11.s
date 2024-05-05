;;===================================================================================================================
;; test-11.s: Test the first suspect conditional instruction.
;;
;;  This test will execute a single-byte instruction which prepares the condition, a 2-byte conditional instruction
;;  which will not pass the condition, replacing the instruction with a NOP, and then followed by a 1-byte
;;  instruction which is not conditional.
;;
;;  This test is required to pass, but is expected to fail at this point in the video series.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2023-May-07  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================

%arch       16bcfs.arch

    .org    0x0000

entry:
    stc                     ;; Set the carry flag
    mov-cc  r1,0x0013       ;; If carry is clear, set r1 (the value here is carefully chosen as mov r1,<imm16>)
    clc                     ;; Clear the carry flag again

    nop
    nop

    jmp     0               ;; loop back and execute the test again


