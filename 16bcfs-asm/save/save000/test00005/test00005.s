; This test contains a jump back to the first instruction

    .org    0

entry:
    nop
    jmp     entry

