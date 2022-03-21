; file ps.as
.entry LIST

.extern LIST
MAIN: add r3, LIST
LOOP: prn #48
macro m1
 inc r6
 mov r3, W
endm
 lea STR, r6
macro m2
 inc r6
 mov #3, r3
endm
.extern LIST
m2
 m1
 sub r1, r4
 bne END
 cmp val1   , #-6
 bne END[r15]
 dec K
.entry val1
.entry MAIN
 sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
 .data -100
.entry KK
.entry K
K: .data 31