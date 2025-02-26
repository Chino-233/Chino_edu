package com.chino; // 定义包名为 com.chino，引入包

class Ladder {
    double above, height; // 实例变量
    static double bottom; // 静态变量
    void setAbove(double a) {
        above = a; //设置实例变量 above的值
    }
    void setBottom(double b) {
        bottom = b; //设置静态变量 bottom的值
    }
    double getAbove() {
        return above; //获取实例变量 above的值
    }
    double getBottom() {
        return bottom; //获取静态变量 bottom的值
    }
}

public class Example4_2 {
    public static void main(String args[]) {
        Ladder.bottom = 60; // 设置静态变量 bottom 的值为 60
        Ladder ladderOne, ladderTwo; // 声明两个 Ladder 类的对象
        System.out.println(Ladder.bottom); // 输出静态变量 bottom 的值
        ladderOne = new Ladder(); // 创建 ladderOne 对象
        ladderTwo = new Ladder(); // 创建 ladderTwo 对象
        System.out.println(ladderOne.getBottom()); // 输出 ladderOne 对象的静态变量 bottom 的值
        System.out.println(ladderTwo.getBottom()); // 输出 ladderTwo 对象的静态变量 bottom 的值
        ladderOne.setAbove(11); // 设置 ladderOne 对象的实例变量 above 的值为 11
        ladderTwo.setAbove(22); // 设置 ladderTwo 对象的实例变量 above 的值为 22
        ladderTwo.setBottom(100); // 设置静态变量 bottom 的值为 100
        System.out.println(Ladder.bottom); // 输出静态变量 bottom 的值
        System.out.println(ladderOne.getAbove()); // 输出 ladderOne 对象的实例变量 above 的值
        System.out.println(ladderTwo.getAbove()); // 输出 ladderTwo 对象的实例变量 above 的值
    }
}
