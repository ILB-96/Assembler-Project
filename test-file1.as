; file ps.as
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
macro m1
 inc r6  
 mov r3, W
endm
 lea STR, r6
 m1
 sub r1, r4
 bne END
 cmp val1, #-6
 bne END[r15]
 dec K
.entry MAIN
 sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, -9
 .data -100
.entry K
K: .data 31
.extern val1
