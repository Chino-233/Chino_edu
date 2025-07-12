package com.chino; // 定义包名为 com.chino，引入包
class Tom
{
    final int MAX=100; // 定义实例常量 MAX，值为 100
    final static int MIN=20; // 定义静态常量 MIN，值为 20
}
public class Example4_3
{
    public static void main(String args[])
    {
        System.out.println(Tom.MIN); // 输出静态常量 MIN 的值
        //System.out.println(Tom.MAX); // 错误：无法通过类名访问实例常量 MAX
        Tom cat = new Tom(); // 创建 Tom 类的实例 cat
        System.out.println(cat.MAX); // 输出实例常量 MAX 的值
    }
}
