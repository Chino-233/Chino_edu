package com.chino;

// 定义一个继承自Thread的类WriteWordThread
class WriteWordThread extends Thread {
    int n = 0; // 线程休眠时间

    // 构造方法，接收线程名称和休眠时间
    public WriteWordThread(String s, int n) {
        setName(s); // 设置线程名称
        this.n = n; // 设置休眠时间
    }


    // 重写run方法，定义线程执行的任务
    public void run() {
        for (int i = 1; i <= 3; i++) { // 循环3次
            System.out.println("Thread: " + getName()); // 输出线程名称
            try {
                sleep(n); // 线程休眠指定时间
            } catch (InterruptedException e) {
                // 捕获并忽略中断异常
            }
        }
    }
}


public class Example8_2 {
    public static void main(String args[]) {
        WriteWordThread zhang, wang; // 声明两个WriteWordThread对象
        zhang = new WriteWordThread("Zhang", 200); // 创建线程对象zhang，休眠时间200毫秒
        wang = new WriteWordThread("Wang", 100); // 创建线程对象wang，休眠时间100毫秒
        zhang.start(); // 启动线程zhang
        wang.start(); // 启动线程wang
    }
}
