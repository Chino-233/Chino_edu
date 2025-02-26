package com.chino; // 定义包名为 com.chino，引入包

class Athlete {
    String name; // 姓名
    String gender; // 性别
    int age; // 年龄
    String item; // 最擅长的比赛项目
    int medal; // 在2024巴黎奥运会获得的奖牌数量

    // 定义一个构造函数，用于初始化 Athlete 对象
    public Athlete(String nameval, String genderval, int ageval, String itemval, int medalval) {
        name = nameval;
        gender = genderval;
        age = ageval;
        item = itemval;
        medal = medalval;
    }
    // 重写 Object 类的 toString() 方法
    @Override
    public String toString() {
        return "姓名：" + name + " 性别：" + gender + " 年龄：" + age + " 最擅长的比赛项目：" + item + " 奖牌数量：" + medal;
    }
}
// 定义一个名为 Chino 的公共类
public class Chino {
    // 主方法，程序的入口点
    public static void main(String[] args) {
        // 创建一个运动员对象，以2024巴黎奥运会中国队前三块金牌获得者为例
        Athlete athlete1 = new Athlete("黄雨婷", "女", 18, "射击", 2);
        // 调用运动员对象的 toString() 方法，输出运动员的详细信息
        System.out.println(athlete1.toString());
        Athlete athlete2 = new Athlete("陈艺文", "女", 25, "跳水", 2);
        System.out.println(athlete2.toString());
        Athlete athlete3 = new Athlete("谢  瑜", "男", 24, "射击", 1);
        System.out.println(athlete3.toString());
    }
}
