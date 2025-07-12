package com.chino;

import java.util.ArrayList;
import java.util.Random;

public class FloatGenerator {
    public static void main(String[] args) {
        ArrayList<Float> floatList = new ArrayList<>();  // 创建一个Float类型的ArrayList
        Object lock = new Object();  // 共享锁对象用于线程通信

        // 线程1：生成浮点数
        Runnable generatorTask = new Runnable() {
            public void run() {
                Random random = new Random();
                while (true) {
                    synchronized (lock) {
                        float num = random.nextFloat();  // 生成随机浮点数
                        floatList.add(num);
                        System.out.println("Generated: " + num);

                        // 生成5个数后通知计算线程
                        if (floatList.size() % 5 == 0) {
                            lock.notify();  // 通知计算线程
                            try {
                                lock.wait();  // 生成线程等待
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                    }

                    // 限制生成速度
                    try {
                        Thread.sleep(300);  // 暂停300ms
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        };

        // 线程2：计算和与平均值
        Runnable calculatorTask = new Runnable() {
            public void run() {
                while (true) {
                    synchronized (lock) {
                        try {
                            // 等待生成线程通知
                            lock.wait();

                            // 计算最新5个浮点数的和与平均值
                            int size = floatList.size();
                            float sum = 0;
                            for (int i = size - 5; i < size; i++) {
                                sum += floatList.get(i);
                            }
                            float average = sum / 5;
                            System.out.println("Sum: " + sum + ", Average: " + average);

                            // 通知生成线程继续生成
                            lock.notify();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        };

        // 启动线程
        new Thread(generatorTask).start();  // 启动生成线程
        new Thread(calculatorTask).start(); // 启动计算线程
    }
}
