package com.chino;

// 定义一个Left类，继承自Thread类
class Left extends Thread {
    int n = 0; // 计数器

    // 重写run方法
    public void run() {
        while (true) {
            n++;
            System.out.println(n + "Left");
            try {
                // 随机休眠一段时间
                sleep((int) (Math.random() * 100));
            } catch (InterruptedException e) {
                // 捕获并忽略中断异常
            }
        }
    }
}

// 定义一个Right类，继承自Thread类
class Right extends Thread {
    int n = 0; // 计数器

    // 重写run方法
    public void run() {
        while (true) {
            n++;
            System.out.println(n + "Right");
            try {
                // 随机休眠一段时间
                sleep((int) (Math.random() * 100));
            } catch (InterruptedException e) {
                // 捕获并忽略中断异常
            }
        }
    }
}

// 定义一个Example8_3类
public class Example8_3 {
    public static void main(String args[]) {
        // 创建Left和Right对象
        Left left = new Left();
        Right right = new Right();
        // 启动Left和Right线程
        left.start();
        right.start();
        while (true) {
            try {
                // 主线程休眠100毫秒
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            // 如果Left或Right的计数器大于等于8，则输出计数器的值并退出程序
            if (left.n >= 8 || right.n >= 8) {
                System.out.println(left.n + "," + right.n);
                System.exit(0);
            }
        }
    }
}
