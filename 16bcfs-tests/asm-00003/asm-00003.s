;;===================================================================================================================
;; asm-00003.s: ensure that `.organization` comes first
;;
;;  Test case for
;;
;;  This source is expected to fail.  It does not have the organization clause first in the architecture.
;;
;;  -----------------------------------------------------------------------------------------------------------------
;;
;;     Date      Tracker  Version  Pgmr  Description
;;  -----------  -------  -------  ----  ---------------------------------------------------------------------------
;;  2024-Jan-13  Initial    #62    ADCL  Initial version
;;
;;===================================================================================================================

.arch               bad.arch
    .org            0x0000

entry:
    nop
