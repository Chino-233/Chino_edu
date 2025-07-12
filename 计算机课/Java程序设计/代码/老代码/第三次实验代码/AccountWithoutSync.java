package com.chino;

import java.util.concurrent.*;
// 定义一个没有同步的账户类
public class AccountWithoutSync{
    // 定义一个静态的账户对象
    private static Account account = new Account();

    public static void main(String[] args) throws InterruptedException {
        // 创建一个线程池
        ExecutorService executor = Executors.newCachedThreadPool();
        // 循环100次，向线程池中添加任务
        for (int i = 0; i < 100; i++) {
            executor.execute(new AddApennyTask());
        }
        // 关闭线程池
        executor.shutdown();
        // 等待线程池中的任务执行完毕
        while (!executor.isTerminated()) {
            
        }
        // 输出账户余额
        System.out.println("What is balance? " + account.getBalance());
    }
    // 定义一个添加一分钱的任务类
    private static class AddApennyTask implements Runnable {
        public void run() {
            // 调用账户对象的存款方法
            account.deposit(1);
        }
    }
    // 定义一个账户类
    private static class Account{
        // 定义账户余额
        private int balance = 0;
        
        // 获取账户余额
        public int getBalance(){
            return balance;
        }
        // 存款方法
        public void deposit(int amount){
            // 定义新的余额
            int newBalance = balance + amount;
            try {
                // 模拟耗时操作
                Thread.sleep(5);
            } catch (InterruptedException ex) {
            }
            // 更新余额
            balance = newBalance;
        }

    }

}
