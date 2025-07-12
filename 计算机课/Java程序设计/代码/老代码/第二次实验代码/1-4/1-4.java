package com.chino; // 定义包名为 com.chino，引入包
class Complex{
    double realPart; // 定义一个double型变量，用于存储实数部分
    double imagePart; // 定义一个double型变量，用于存储虚数部分
    public Complex(){ // 构造方法，将实数部分和虚数部分都置为0
        realPart = 0; // 将实数部分置为0    
        imagePart = 0; // 将虚数部分置为0
    }
    public Complex(double r, double i){ // 构造方法，将实数部分和虚数部分分别置为r和i

        realPart = r; // 将实数部分置为r
        imagePart = i; // 将虚数部分置为i
    }
    public Complex complexSub(Complex c){ // 将当前复数对象与形参复数对象相减
        Complex result = new Complex(); // 创建一个Complex对象，用于存储相减的结果
        result.realPart = this.realPart - c.realPart; // 将实数部分相减
        result.imagePart = this.imagePart - c.imagePart; // 将虚数部分相减
        return result; // 返回相减的结果
    }
    public Complex complexMult(Complex c){ // 将当前复数对象与形参复数对象相乘
        Complex result = new Complex(); // 创建一个Complex对象，用于存储相乘的结果
        result.realPart = this.realPart * c.realPart - this.imagePart * c.imagePart; // 将实数部分相乘
        result.imagePart = this.realPart * c.imagePart + this.imagePart * c.realPart; // 将虚数部分相乘
        return result; // 返回相乘的结果
    }
    public String toString(){ // 将当前复数对象的实数部分和虚数部分组合成a+bi的字符串形式
        return this.realPart + "+" + this.imagePart + "i"; // 返回字符串
    }
}
// 定义一个名为 Chino 的公共类
public class Chino {
    // 主方法，程序的入口点
    public static void main(String[] args) {
        Complex c1 = new Complex(3, 5); // 创建一个Complex对象，实数部分为3，虚数部分为5
        Complex c2 = new Complex(2, 7); // 创建一个Complex对象，实数部分为2，虚数部分为7
        Complex c3 = c1.complexSub(c2); // 将c1和c2相减，结果存储在c3中
        Complex c4 = c1.complexMult(c2); // 将c1和c2相乘，结果存储在c4中
        System.out.println("c1 - c2 = " + c3.toString()); // 输出c1 - c2的结果
        System.out.println("c1 * c2 = " + c4.toString()); // 输出c1 * c2的结果

    }
}


