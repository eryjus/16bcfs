;;===================================================================================================================
;;  test-r2.s: Test the R2 Register PCB
;;
;;  This test will go through all the functionality of the R2 Register.  This code is used to test the new timing
;;  for INC/DEC/LOAD.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2023-Aug-22  Initial  v0.0.1   ADCL  Initial version
;;
;;===================================================================================================================


    .org    0x0000

entry:
    mov     r2,0xffff           ;; not sure what is in the register, ensure it is initialized
    decr    r2                  ;; decrement the R2 register without updating flags; value = 0xfffe
    incr    r2                  ;; increment the R2 register without updating flags; value = 0xffff
    incr    r2                  ;; increment the R2 register without updating flags; value = 0x0000
    incr    r2                  ;; increment the R2 register without updating flags; value = 0x0001
    decr    r2                  ;; decrement the R2 register without updating flags; value = 0x0000
    decr    r2                  ;; decrement the R2 register without updating flags; value = 0xffff

    jmp     r2                  ;; jump to the R2 register location


