package com.chino; // 定义包名为 com.chino，引入包

class Box {
    int width; // 定义宽度属性
    int height; // 定义高度属性
    int depth; // 定义深度属性
    // 定义一个设置盒子信息的方法
    void setInfo(int w, int h, int d) {
        width = w; // 设置宽度
        height = h; // 设置高度
        depth = d; // 设置深度
    }
    // 定义一个计算体积的方法
    int volume() {
        return width * height * depth; // 返回体积
    }
    // 定义一个计算表面积的方法
    int area() {
        return 2 * (width * height + height * depth + width * depth); // 返回表面积
    }

    // 重写 toString 方法，返回盒子的尺寸信息
    @Override
    public String toString() {
        return "长：" + width + " 宽：" + height + " 高：" + depth; // 返回盒子的尺寸信息
    }
}
public class Chino {
    public static void main(String[] args) {
        Box box = new Box(); // 创建一个 Box 对象
        box.setInfo(11, 45, 14); // 设置盒子的宽、高、深
        System.out.println("长方体的体积：" + box.volume()); // 输出盒子的体积
        System.out.println("长方体的表面积：" + box.area()); // 输出盒子的表面积
        System.out.println("长方体的尺寸：" + box.toString()); // 输出盒子的尺寸信息
    }
}
