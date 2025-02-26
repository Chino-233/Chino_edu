package com.chino;

class TaskBank implements Runnable {
    private int money = 0; // 账户余额
    String name1, name2; // 线程名称

    // 构造方法，初始化线程名称
    TaskBank(String s1, String s2) {
        name1 = s1;
        name2 = s2;
    }

    // 设置账户余额
    public void setMoney(int mount) {
        money = mount;
    }

    public void run() {
        while (true) {
            money = money - 10;
            // 判断当前线程名称，输出账户余额
            if (Thread.currentThread().getName().equals(name1)) {
                System.out.println(name1 + ": " + money);
                // 如果账户余额小于等于100，输出完成信息并结束线程
                if (money <= 100) {
                    System.out.println(name1 + ": Finished");
                    return;
                }
            } else if (Thread.currentThread().getName().equals(name2)) {
                System.out.println(name2 + ": " + money);
                // 如果账户余额小于等于60，输出完成信息并结束线程
                if (money <= 60) {
                    System.out.println(name2 + ": Finished");
                    return;
                }
            }
            try {
                // 暂停线程800毫秒
                Thread.sleep(800);
            } catch (InterruptedException e) {
                // 捕获并忽略中断异常
            }
        }
    }
}

public class Example8_4 {
    public static void main(String args[]) {
        String s1 = "treasurer zhang"; // 会计
        String s2 = "cashier cheng"; // 出纳
        TaskBank taskBank = new TaskBank(s1, s2);
        taskBank.setMoney(120);
        Thread zhang;
        Thread cheng;
        zhang = new Thread(taskBank); // 目标对象bank
        cheng = new Thread(taskBank); // 目标对象bank
        zhang.setName(s1);
        cheng.setName(s2);
        zhang.start();
        cheng.start();
    }
}
