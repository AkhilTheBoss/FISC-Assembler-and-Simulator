start:  not r0 r1
        and r0 r0 r1
        not r1 r0
        add r1 r1 r1
        not r1 r1
loop:   and r3 r0 r0
        add r0 r0 r1
        bnz loop
        and r3 r3 r3
        not r2 r3
        and r1 r2 r3
