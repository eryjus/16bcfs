//===================================================================================================================
// asm-00007.s: test labels
//
//  Test case for jumping to a label.
//
//  This source is expected to succeed.
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2024-May-05  Initial      #34  ADCL  Initial version
//
//===================================================================================================================

%arch       16bcfs.arch
    .org    0x0000

entry:
    nop
    jmp     .1

    nop

.1:
    jmp     entry
