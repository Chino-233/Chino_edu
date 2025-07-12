package com.chino; // 定义包名为 com.chino，引入包
class Circle // 定义一个名为 Circle 的类
{
    double radius; // 定义一个 double 类型的成员变量 radius，表示圆的半径
    double getArea() // 定义一个返回 double 类型的成员方法 getArea，用于计算圆的面积
    {
        double area = 3.14 * radius * radius; // 用公式 π*r^2计算圆的面积
        return area; // 返回计算得到的面积
    }
}

public class Example4_2 { // 定义一个公共类 Example4_2
    public static void main(String args[]) // 主方法，程序的入口
    {
        Circle circle; // 声明一个Circle类型的变量 circle
        circle = new Circle(); // 创建一个Circle对象并赋值给circle 变量
        circle.radius = 1; // 设置circle对象的半径为1
        double area = circle.getArea(); // 调用Circle类的getArea方法计算面积，并将结果赋值给area变量
        System.out.println(area); // 输出面积到控制台
    }
}