package com.chino;
import java.util.concurrent.*;

class PrintChar implements Runnable {
    private char charToPrint; // 要打印的字符
    private int times; // 打印次数

    // 构造函数，初始化要打印的字符和打印次数
    public PrintChar(char c, int t) {
        this.charToPrint = c;
        this.times = t;
    }

    // 实现Runnable接口的run方法，打印字符
    @Override
    public void run() {
        for (int i = 0; i < times; i++) {
            System.out.print(charToPrint);
        }
    }
}

class PrintNum implements Runnable {
    private int lastNum; // 要打印的最大数字

    // 构造函数，初始化要打印的最大数字
    public PrintNum(int n) {
        lastNum = n;
    }

    @Override
    public void run() {
        for (int i = 1; i <= lastNum; i++) {
            System.out.print(" " + i);
        }
    }
}

public class ExecutorDomo {
    public static void main(String[] args) {
        // 创建一个具有固定线程数的线程池
        ExecutorService executor = Executors.newFixedThreadPool(3);
        // 执行打印字符'a'的任务
        executor.execute(new PrintChar('a', 100));
        // 执行打印字符'b'的任务
        executor.execute(new PrintChar('b', 100));
        // 执行打印数字的任务
        executor.execute(new PrintNum(100));
        // 关闭线程池
        executor.shutdown();
    }
}
