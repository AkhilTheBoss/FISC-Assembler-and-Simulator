start:  not r0 r1
        and r0 r0 r1
        not r1 r0
        add r1 r1 r1
        not r1 r1
loop:   and r3 r0 r0
        add r0 r1 r3
        bnz endloop   ; New line: Break out of the loop if r0 is not zero
        and r3 r3 r3
        not r2 r3
        and r1 r2 r3
endloop: add r0 r0 r0
