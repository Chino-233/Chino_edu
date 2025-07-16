.ORIG x3000
    LD R6,SPRT ;
    LD R1,POS ;
    LD R2,VECTOR ;
    STR R1,R2,0;
    LD R3,IE ;
    STI R3,KBSR ;初始化操作
BEGIN
    LD R4,SIX
OUT1 
    LD R0,I ;
    OUT ;
    JSR WAIT
    LD R0,C ;
    OUT ;
    JSR WAIT ;
    LD R0,S ;
    OUT ;
    JSR WAIT ;
    LEA R0,SPACE
    PUTS ;
    ADD R4,R4,-1;
    BRP OUT1 ;
    LD R0,NEXTLINE
    OUT
    LD R4,SIX ;
    ADD R4,R4,-1;
    BRNZP OUT2
OUT2 
    LEA R0,SPACE
    PUTS ;
    LD R0,I ;
    OUT ;
    JSR WAIT
    LD R0,C ;
    OUT ;
    JSR WAIT ;
    LD R0,S ;
    OUT ;
    JSR WAIT ;
    ADD R4,R4,-1;
    BRP OUT2 ; 
    LD R0,NEXTLINE
    OUT   
    LD R4,SIX ;
    BRNZP OUT1 ;
WAIT
    ST R1,STONER1 ;
    LD R1,WAITNUM ;
AGAIN 
    ADD R1,R1,-1;
    BRP AGAIN ;
    LD R1,STONER1 ;
    RET
    HALT
SPRT		.FILL x4000 ;栈位置
POS		    .FILL x2000	;中断程序入口
VECTOR		.FILL x180	;中断矢量
IE			.FILL x4000	;中断允许标志
KBSR		.FILL xFE00
KBDR		.FILL xFE02
I			.FILL 73
C			.FILL 67
S			.FILL 83
SPACE		.STRINGZ "   "
SIX		    .FILL 6
WAITNUM		.FILL x2fff
STONER7     .BLKW 1
STONER1		.BLKW 1
NEXTLINE	.FILL 10
.END



