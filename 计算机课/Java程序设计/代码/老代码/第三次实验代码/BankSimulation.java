package com.chino; // 定义包名

class BankAccount {
    private int balance = 100; // 初始余额为 100

    public void deposit(int amount) {
        synchronized (this) { // 同步块，确保线程安全
            balance += amount; // 增加余额
            System.out.println(Thread.currentThread().getName() + " 存款 " + amount + " 元，当前余额: " + balance + " 元"); // 输出存款信息
        }
    }

    public void withdraw(int amount) {
        synchronized (this) { // 同步块，确保线程安全
            if (balance >= amount) { // 检查余额是否足够
                balance -= amount; // 减少余额
                System.out.println(Thread.currentThread().getName() + " 取款 " + amount + " 元，当前余额: " + balance + " 元"); // 输出取款信息
            } else {
                System.out.println(Thread.currentThread().getName() + " 取款失败，余额不足！"); // 输出余额不足信息
            }
        }
    }
}

class DepositTask implements Runnable {
    private final BankAccount account; // 银行账户对象

    public DepositTask(BankAccount account) {
        this.account = account; // 初始化银行账户对象
    }

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) { // 循环 5 次
            account.deposit(10); // 每次存款 10 元
            try {
                Thread.sleep(100); // 模拟操作时间，暂停 100 毫秒
            } catch (InterruptedException e) {
                e.printStackTrace(); // 捕获并打印中断异常
            }
        }
    }
}

class WithdrawTask implements Runnable {
    private final BankAccount account; // 银行账户对象

    public WithdrawTask(BankAccount account) {
        this.account = account; // 初始化银行账户对象
    }

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) { // 循环 5 次
            account.withdraw(10); // 每次取款 10 元
            try {
                Thread.sleep(100); // 模拟操作时间，暂停 100 毫秒
            } catch (InterruptedException e) {
                e.printStackTrace(); // 捕获并打印中断异常
            }
        }
    }
}

public class BankSimulation {
    public static void main(String[] args) {
        BankAccount account = new BankAccount(); // 创建银行账户对象

        Thread withdrawThread1 = new Thread(new WithdrawTask(account), "取款线程1"); // 创建取款线程1
        Thread withdrawThread2 = new Thread(new WithdrawTask(account), "取款线程2"); // 创建取款线程2
        Thread withdrawThread3 = new Thread(new WithdrawTask(account), "取款线程3"); // 创建取款线程3

        Thread depositThread1 = new Thread(new DepositTask(account), "存款线程1"); // 创建存款线程1
        Thread depositThread2 = new Thread(new DepositTask(account), "存款线程2"); // 创建存款线程2

        withdrawThread1.start(); // 启动取款线程1
        withdrawThread2.start(); // 启动取款线程2
        withdrawThread3.start(); // 启动取款线程3
        depositThread1.start(); // 启动存款线程1
        depositThread2.start(); // 启动存款线程2
    }
}
