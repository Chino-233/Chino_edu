package com.chino;

class TaskAddSub implements Runnable {
    String s1, s2; // 线程名称

    TaskAddSub(String s1, String s2) {
        this.s1 = s1;
        this.s2 = s2;
    }

    public void run() {
        // 定义一个变量i，初始值为0
        int i = 0;
        // 无限循环
        while (true) {
            // 如果当前线程的名字等于s1
            if (Thread.currentThread().getName().equals(s1)) {
                // i加1
                i = i + 1;
                // 输出s1:i
                System.out.println(s1 + ":" + i);
                // 如果i大于等于4
                if (i >= 4) {
                    // 输出s1Finished
                    System.out.println(s1 + "Finished");
                    // 返回
                    return;
                }
            // 如果当前线程的名字等于s2
            } else if (Thread.currentThread().getName().equals(s2)) {
                // i减1
                i = i - 1;
                // 输出s2:i
                System.out.println(s2 + ":" + i);
                if (i <= -4) {
                    System.out.println(s2 + "Finished");
                    return;
                }
            }
            try {
                Thread.sleep(800);
            } catch (InterruptedException e) {
                // 捕获并忽略中断异常
            }
        }
    }
}

// 定义一个类Example8_6
public class Example8_6 {
    // 主方法
    public static void main(String args[]) {
        // 定义两个字符串变量s1和s2
        String s1 = "ZHANG San";
        String s2 = "LI Si";
        // 创建一个TaskAddSub对象，传入s1和s2
        TaskAddSub taskAddSub = new TaskAddSub(s1, s2);

        // 定义两个Thread对象zhang和li
        Thread zhang, li;
        // 创建两个Thread对象，传入taskAddSub
        zhang = new Thread(taskAddSub);
        li = new Thread(taskAddSub);
        // 设置zhang和li的名称为s1和s2
        zhang.setName(s1);
        li.setName(s2);
        // 启动zhang和li线程
        zhang.start();
        li.start();
    }
}
    