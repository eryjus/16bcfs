; This test contains a forward jump to an undeclared label

    .org    0x0000

entry:
    nop
    jmp     forward

    nop

forward:
    jmp     entry

