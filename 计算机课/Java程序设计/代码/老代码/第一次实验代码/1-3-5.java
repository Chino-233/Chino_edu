package com.chino; // 定义包名为 com.chino，引入包
class Teacher{
    String name; // 定义姓名属性
    String title; // 定义职位属性
    String course; // 定义主讲课程
    String research; // 定义研究方向
    String office; //定义办公室

    void setInfo(String name, String title, String course, String research, String office){
        this.name = name; //设置name属性
        this.title = title; //设置title属性
        this.course = course; //设置course属性
        this.research = research; //设置research属性
        this.office = office; //设置office属性
    }
    void setname(String name){
        this.name = name; //设置name属性
    }
    void settitle(String title){
        this.title = title; //设置title属性
    }
    void setcourse(String course){
        this.course = course; //设置course属性
    }
    void setresearch(String research){
        this.research = research; //设置research属性
    }
    void setoffice(String office){
        this.office = office; //设置office属性
    }
    String getname(){
        return name; //返回name属性
    }
    String gettitle(){
        return title; //返回title属性
    }
    String getcourse(){
        return course; //返回course属性
    }
    String getresearch(){
        return research; //返回research属性
    }
    String getoffice(){
        return office; //返回office属性
    }

}
public class Chino {
    public static void main(String[] args) {
        Teacher teacher = new Teacher(); // 创建Teacher对象
        teacher.setInfo("张三", "教授", "Java", "计算机", "L3-1204");
         // 设置教师信息
        System.out.println("姓名：" + teacher.getname()); // 输出姓名
        System.out.println("职位：" + teacher.gettitle()); // 输出职位
        System.out.println("主讲课程：" + teacher.getcourse()); // 输出主讲课程
        System.out.println("研究方向：" + teacher.getresearch()); // 输出研究方向
        System.out.println("办公室：" + teacher.getoffice()); // 输出办公室
        teacher.setname("智乃");
        teacher.settitle("讲师");
        teacher.setcourse("coffee");
        teacher.setresearch("rabbit of coffee");
        teacher.setoffice("rabbit house");
        System.out.println("姓名：" + teacher.getname()); // 输出姓名
        System.out.println("职位：" + teacher.gettitle()); // 输出职位
        System.out.println("主讲课程：" + teacher.getcourse()); // 输出主讲课程
        System.out.println("研究方向：" + teacher.getresearch()); // 输出研究方向
        System.out.println("办公室：" + teacher.getoffice()); // 输出办公室
    }
}
