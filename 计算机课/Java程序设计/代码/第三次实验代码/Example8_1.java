package com.chino;

// 定义一个继承自Thread的类WriteWordThread
class WriteWordThread extends Thread {
    // 构造方法，传入一个字符串参数，用于设置线程名称
    WriteWordThread(String s) {
        setName(s);
    }

    // 重写run方法，定义线程要执行的任务
    public void run() {
        for (int i = 0; i < 10; i++) {
            System.out.println("Thread: "+getName());
        }
    }
}
/*
 javac com/chino/Example8_2.java
java com.chino.Example8_2
 */
// 主类Example8_1
public class Example8_1{
    // 主方法
    public static void main(String[] args) {
        // 定义两个WriteWordThread对象
        WriteWordThread zhang,wang;
        // 创建zhang对象，并设置线程名称为"Zhang"
        zhang = new WriteWordThread("Zhang");
        // 创建wang对象，并设置线程名称为"Wang"
        wang = new WriteWordThread("Wang");
        // 启动zhang线程
        zhang.start();
        // 主线程输出3次"Main Thread"
        for(int i=1;i<=3;i++){
            System.out.println("Main Thread");
        }
        // 启动wang线程
        wang.start();
    }
}
