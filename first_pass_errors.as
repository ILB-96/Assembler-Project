 sub r1, r4
 bne END
 cmp val1, #-6
 bne END[r15]
 dec K
.entry MAIN
 sub LOOP[r10] ,r14
END: stop
STR: .string
LIST: .data 6, -9
 .data -100 
.entry K
.extern
K: .data 31
r3: stop K
endm: .data
.extern K
.extern val1
