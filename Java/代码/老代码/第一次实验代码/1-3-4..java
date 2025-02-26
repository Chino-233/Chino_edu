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
        return math > programming ? (math > english ? math :english):(programming > english ? programming : english);
        // 返回最高成绩
    }
    boolean cmp(PostGraduateStudent s){
        return comSum() > s.comSum(); // 返回总成绩是否大于 s 的总成绩
    }
}
public class Chino {
    public static void main(String[] args) {
        PostGraduateStudent student = new PostGraduateStudent(); // 创建一个 PostGraduateStudent 对象
        student.ID = 20241204; // 设置学号
        student.name = "张三"; // 设置姓名
        student.math = 11; // 设置数学成绩
        student.programming = 45; // 设置编程成绩
        student.english = 14; // 设置英语成绩
        PostGraduateStudent student2 = new PostGraduateStudent(); // 创建一个 PostGraduateStudent 对象
        student2.ID = 20241205; // 设置学号
        student2.name = "李四"; // 设置姓名
        student2.math = 19; // 设置数学成绩
        student2.programming = 19; // 设置编程成绩
        student2.english = 81; // 设置英语成绩
        if(student.cmp(student2)){
            System.out.println(student.name + "的总成绩大于" + student2.name); // 输出比较结果
        }
        else{
            System.out.println(student2.name + "的总成绩大于" + student.name); // 输出比较结果
        } // 比较两个学生的总成绩
        System.err.println("学号：" + student.ID + " 姓名：" + student.name + 
                   " 数学：" + student.math + " 编程：" + student.programming + 
                   " 英语：" + student.english + " 总分：" + student.comSum() + 
                   " 平均分：" + student.comAvg() + " 最高分：" + student.comMax()); // 输出学生信息

        System.err.println("学号：" + student2.ID + " 姓名：" + student2.name + 
                   " 数学：" + student2.math + " 编程：" + student2.programming + 
                   " 英语：" + student2.english + " 总分：" + student2.comSum() + 
                   " 平均分：" + student2.comAvg() + " 最高分：" + student2.comMax()); // 输出学生信息
    }
}
