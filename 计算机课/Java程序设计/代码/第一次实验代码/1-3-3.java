package com.chino; // 定义包名为 com.chino，引入包
class PostGraduateStudent{
    int ID; // 定义学号属性
    String name; // 定义姓名属性
    int math; // 定义数学成绩属性
    int programming; // 定义编程成绩属性
    int english; // 定义英语成绩属性
    int comSum(){
        return math + programming + english; // 返回总成绩
    }
    int comAvg(){
        return (math + programming + english) / 3; // 返回平均成绩
    }
    int comMax(){
        return math > programming ? (math > english ? math : english) : (programming > english ? programming : english); // 返回最高成绩
    }
    boolean cmp(PostGraduateStudent s){
        return comSum() > s.comSum(); // 返回总成绩是否大于 s 的总成绩
    }
}
class Cone {
    int radius; // 定义半径属性
    int height; // 定义高度属性 
    // 定义一个设置盒子信息的方法
    void setInfo(int r, int h) {
        radius = r; // 设置半径
        height = h; // 设置高度
    }
    // 定义一个计算体积的方法
    int volume() {
        return (int)(Math.PI * radius * radius * height / 3); // 返回体积
    }
    // 定义一个计算表面积的方法
    int area() {
        return (int)(Math.PI * radius * (radius + Math.sqrt(radius * radius + height * height))); // 返回表面积
    }

    // 重写 toString 方法，返回盒子的尺寸信息
    @Override
    public String toString() {
        return "底面半径：" + radius + " 高：" + height; // 返回盒子的尺寸信息
    }
}
public class Chino {
    public static void main(String[] args) {
        Cone cone = new Cone(); // 创建一个 Cone 对象
        cone.setInfo(11, 45); // 设置圆锥的半径、高
        System.out.println("圆锥的体积：" + cone.volume()); // 输出盒子的体积
        System.out.println("圆锥的表面积：" + cone.area()); // 输出盒子的表面积
        System.out.println("圆锥的尺寸：" + cone.toString()); // 输出盒子的尺寸信息
    }
}
