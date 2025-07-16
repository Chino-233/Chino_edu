package com.chino;

import java.util.concurrent.locks.ReentrantLock;

public class BridgeCrossing {
    private static final int NUM_PEOPLE = 1000; // 定义过桥人数
    private static final ReentrantLock bridge = new ReentrantLock(true); // 使用公平锁
    private static int southCount = 0; // 记录南边过桥人数
    private static int northCount = 0; // 记录北边过桥人数

    public static void main(String[] args) {
        int southWins = 0; // 记录南边过桥胜利次数
        int northWins = 0; // 记录北边过桥胜利次数

        for (int run = 1; run <= 10; run++) { // 进行10次运行
            southCount = 0; // 每次运行前，将南边过桥人数和北边过桥人数重置为0
            northCount = 0;

            Thread[] southThreads = new Thread[NUM_PEOPLE]; // 创建南边过桥线程数组
            Thread[] northThreads = new Thread[NUM_PEOPLE]; // 创建北边过桥线程数组

            for (int i = 0; i < NUM_PEOPLE; i++) { // 创建南边和北边过桥线程
                southThreads[i] = new Thread(new Person("S" + (i + 1), "South"));
                northThreads[i] = new Thread(new Person("N" + (i + 1), "North"));
            }

            for (int i = 0; i < NUM_PEOPLE; i++) { // 启动南边和北边过桥线程
                southThreads[i].start();
                northThreads[i].start();
            }

            try {
                for (int i = 0; i < NUM_PEOPLE; i++) { // 等待南边和北边过桥线程执行完毕
                    southThreads[i].join();
                    northThreads[i].join();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            String winner = southCount > northCount ? "南边" : "北边"; // 判断哪边先完成过桥
            System.out.printf("第%d次运行，%s先完成过桥%n", run, winner); // 输出哪边先完成过桥

            if (southCount > northCount) { // 记录哪边先完成过桥的次数
                southWins++;
            } else {
                northWins++;
            }
        }

        System.out.printf("南边先完成过桥次数: %d%n", southWins); // 输出南边先完成过桥的次数
        System.out.printf("北边先完成过桥次数: %d%n", northWins); // 输出北边先完成过桥的次数
    }

    static class Person implements Runnable {
        private final String name; // 人的名字
        private final String direction; // 过桥方向

        public Person(String name, String direction) {
            this.name = name;
            this.direction = direction;
        }

        @Override
        public void run() {
            try {
                bridge.lock(); // 获取锁
                System.out.println(name + " 正在过桥 (" + direction + ")");
                Thread.sleep(0); // 模拟过桥时间
                if (direction.equals("South")) { // 如果是南边过桥
                    southCount++;
                } else { // 如果是北边过桥
                    northCount++;
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                bridge.unlock(); // 释放锁
            }
        }
    }
}
