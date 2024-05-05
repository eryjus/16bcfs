//===================================================================================================================
//  test-ctrl.s: Test the new Control ROMs 2 & 3
//
//  This test will iterate through the available control signals from CTRL2 and CTRL3 at the time of this
//  video.
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2023-Sep-08  Initial  v0.0.1   ADCL  Initial version
//
//===================================================================================================================

%arch       16bcfs.arch

    .org    0x0000

entry:
    mov     r1,0x8000               // fetch assert main; r1 load & latch; inc pc X 2
    mov     r2,r1                   // r1 assert main; r2 load & latch; inc pc
    mov     r2,0x0001               // fetch assert main; r2 load & latch; inc pc X 2
    mov     r1,r2                   // r2 assert main; r1 load & latch; inc pc

    jmp     0x0000                  // fetch assert main; pc load & latch; inc pc

