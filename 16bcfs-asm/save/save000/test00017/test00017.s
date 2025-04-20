; check for intermediate labels, jumping forwards

    .org    0x0000

entry:
    nop
    jmp     .1

    nop

.1:
    jmp     entry
