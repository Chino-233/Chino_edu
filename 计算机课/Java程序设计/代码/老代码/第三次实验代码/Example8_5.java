package com.chino;

class Task implements Runnable {
    private int number = 0; // 共享变量

    // 设置number的值
    public synchronized void setNumber(int n) {
        number = n;
    }

    public void run() {
        // 无限循环
        while (true) {
            // 同步代码块，保证线程安全
            synchronized (this) {
                // 如果当前线程的名字是"add"，则number加1，并输出
                if (Thread.currentThread().getName().equals("add")) {
                    number++;
                    System.out.printf("%d\n", number);
                }
                // 如果当前线程的名字是"sub"，则number减1，并输出
                if (Thread.currentThread().getName().equals("sub")) {
                    number--;
                    System.out.printf("%12d\n", number);
                }
            }
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                // 捕获并忽略中断异常
            }
        }
    }
}

public class Example8_5 {
    public static void main(String args[]) {
        // 创建一个Task对象，并设置其number属性为10
        Task taskAdd = new Task();
        taskAdd.setNumber(10);
        // 创建一个Task对象，并设置其number属性为-10
        Task taskSub = new Task();
        taskSub.setNumber(-10);
        // 创建四个线程对象
        Thread threadA, threadB, threadC, threadD;
        // 将taskAdd对象作为参数传入线程对象
        threadA = new Thread(taskAdd);
        threadB = new Thread(taskAdd);
        // 设置线程A和线程B的名称为"add"
        threadA.setName("add");
        threadB.setName("add");
        // 将taskSub对象作为参数传入线程对象
        threadC = new Thread(taskSub);
        threadD = new Thread(taskSub);
        // 设置线程C和线程D的名称为"sub"
        threadC.setName("sub");
        threadD.setName("sub");
        // 启动四个线程
        threadA.start();
        threadB.start();
        threadC.start();
        threadD.start();
    }
}