;;===================================================================================================================
;;  test-r1.s: Test the R1 Register PCB
;;
;;  This test will go through all the functionality of the R1 Register.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2023-Jun-14  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================


    .org    0x0000

entry:
    mov     r1,0xffff           ;; not sure what is in the register, ensure it is initialized
    decr    r1                  ;; decrement the R1 register without updating flags; value = 0xfffe
    incr    r1                  ;; increment the R1 register without updating flags; value = 0xffff
    incr    r1                  ;; increment the R1 register without updating flags; value = 0x0000
    incr    r1                  ;; increment the R1 register without updating flags; value = 0x0001
    decr    r1                  ;; decrement the R1 register without updating flags; value = 0x0000
    decr    r1                  ;; decrement the R1 register without updating flags; value = 0xffff

    jmp     r1                  ;; jump to the R1 register location


