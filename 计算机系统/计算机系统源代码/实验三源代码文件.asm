.ORIG x3000
    AND R7,R7,0;
    ADD R7,R7,15;
AGAIN
    AND R0,R0,0;
    AND R1,R1,0;
    AND R2,R2,0;
    AND R3,R3,0;
    AND R4,R4,0;
    AND R5,R5,0;
    AND R6,R6,0;
    LD R1,POSSTART ;R1为起始位置
    ADD R2,R2,-1;
LOOPOUT 
    ADD R2,R2,1; R2为i
    ADD R6,R2,-15;
    BRZ NEXT
    AND R6,R6,0;
    ADD R3,R2,0; R3为j
LOOPIN
    ADD R0,R1,R3; R0充当j与地址的中间寄存器
    LDR R4,R0,0; R4=ARR[J];
    ADD R3,R3,1; J=J+1
    ADD R0,R1,R3;
    LDR R5,R0,0; R5=ARR[J+1] 
    NOT R5,R5;
    ADD R5,R5,1;
    ADD R5,R5,R4;R5=R4-R5;
    BRNZ NOSWAP ;R4<=R5
    ADD R5,R4,0; R5=R4
    ADD R0,R1,R3;
    LDR R4,R0,0; R4=R5
    ADD R0,R1,R3;
    STR R5,R0,0; 
    ADD R3,R3,-1;
    ADD R0,R1,R3;
    STR R4,R0,0;
    ADD R3,R3,1;
NOSWAP
    ADD R6,R3,-15;
    BRZ LOOPOUT
    BRNZP LOOPIN
NEXT
    ADD R7,R7,-1;
    BRNP AGAIN
    AND R0,R0,0;
    AND R1,R1,0;
    AND R2,R2,0;
    AND R3,R3,0;
    AND R4,R4,0;
    AND R5,R5,0;
    AND R6,R6,0;
    LD R1,POSSTART ;R1为起始位置
    ADD R3,R3,10;
    ADD R3,R3,6;
GETNUMBER
    ADD R3,R3,0;
    BRZ STONE
    LDR R2,R1,0; R2放置判断数;
    BRNZP JUDGE
JUDGE
;A
    ADD R6,R3,-4;
    BRP B
    ADD R6,R2,-15;
    ADD R6,R6,-15;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-5;
    BRN B
    ADD R4,R4,1;
    ADD R1,R1,1;
    ADD R3,R3,-1;
    BRNZP GETNUMBER
B
    ADD R6,R3,-8;
    BRP C
    ADD R6,R2,-15;
    ADD R6,R6,-15;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-10;
    ADD R6,R6,-5;
    BRN C
    ADD R5,R5,1;
    ADD R1,R1,1;
    ADD R3,R3,-1;
    BRNZP GETNUMBER
C
    ADD R1,R1,1;
    ADD R3,R3,-1;
    BRNZP GETNUMBER
STONE
    STI R4,POSSTONE
    LD R4,POSSTONE
    ADD R4,R4,1;
    STR R5,R4,0;
    HALT
POSSTART .FILL X4000
POSSTONE .FILL X5100
.END