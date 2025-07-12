package com.chino; // 定义包名为 com.chino，引入包

class Ladder {
    double above, bottom, height; // 定义三个双精度浮点型变量，分别表示梯形的上底、下底和高
    Ladder() {} // 无参构造方法
    Ladder(double a, double b, double h) { // 有参构造方法，初始化上底、下底和高
        above = a; // 初始化上底
        bottom = b; // 初始化下底
        height = h; // 初始化高
    }
    public void setAbove(double a) { // 设置上底的方法
        above = a; // 设置上底
    }
    public void setBottom(double b) { // 设置下底的方法
        bottom = b; // 设置下底
    }
    public void setHeight(double h) { // 设置高的方法
        height = h; // 设置高
    }
    double computeArea() { // 计算梯形面积的方法
        return (above + bottom) * height / 2.0; // 返回梯形的面积
    }
}

public class Example4_1 {
    public static void main(String args[]) { // 主方法
        double area1 = 0, area2 = 0; // 定义两个双精度浮点型变量，分别表示两个梯形的面积
        Ladder ladderOne, ladderTwo; // 定义两个 Ladder 类的对象
        ladderOne = new Ladder(); // 使用无参构造方法创建第一个 Ladder 对象
        ladderTwo = new Ladder(10, 88, 20); // 使用有参构造方法并初始化上底、下底和高创建第二个 Ladder 对象
        ladderOne.setAbove(16); // 设置第一个梯形的上底为 16
        ladderOne.setBottom(26); // 设置第一个梯形的下底为 26
        ladderOne.setHeight(100); // 设置第一个梯形的高为 100
        ladderTwo.setAbove(300); // 设置第二个梯形的上底为 300
        ladderTwo.setBottom(500); // 设置第二个梯形的下底为 500
        area1 = ladderOne.computeArea(); // 计算第一个梯形的面积并赋值给 area1
        area2 = ladderTwo.computeArea(); // 计算第二个梯形的面积并赋值给 area2
        System.out.println(area1); // 输出第一个梯形的面积
        System.out.println(area2); // 输出第二个梯形的面积
    }
}