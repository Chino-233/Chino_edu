#
# 构建 btest 和其他辅助程序的 Makefile，用于 CS:APP 数据实验
# 
CC = gcc
CFLAGS = -O -Wall -m32
LIBS = -lm

all: btest fshow ishow

btest: btest.c bits.c decl.c tests.c btest.h bits.h
    $(CC) $(CFLAGS) $(LIBS) -o btest bits.c btest.c decl.c tests.c

fshow: fshow.c
    $(CC) $(CFLAGS) -o fshow fshow.c

ishow: ishow.c
    $(CC) $(CFLAGS) -o ishow ishow.c

# 强制重新编译。由驱动程序使用。
btestexplicit:
    $(CC) $(CFLAGS) $(LIBS) -o btest bits.c btest.c decl.c tests.c 

clean:
    rm -f *.o btest fshow ishow *~