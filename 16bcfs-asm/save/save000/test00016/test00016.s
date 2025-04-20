; check for intermediate labels, jumping backwards

    .org    0x0000

entry:
    nop
.1:
    jmp     .1
