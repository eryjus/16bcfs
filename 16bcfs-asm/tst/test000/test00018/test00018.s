;;===================================================================================================================
;; test-nop.s: initialize the Program memory (1 word) to be a `nop` instruction
;;
;;  This source is used to generate a single `nop` instruction, and then fill the rest of the program ROM with the
;;  default encoding.  The purpose is to hae a single address (2^0 bit address space) for executing a `nop`
;;  instruction.
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
    nop