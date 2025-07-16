package com.chino;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

// 定义研究所/中心类
class Institute {
    private String name;
    private String head;

    // 构造函数，初始化研究所/中心的名字和负责人
    public Institute(String name, String head) {
        this.name = name;
        this.head = head;
    }

    // 获取研究所/中心的名字
    public String getName() {
        return name;
    }

    public String getHead() {
        return head;
    }
}

class Department {
    private String name;
    private String head;

    public Department(String name, String head) {
        this.name = name;
        this.head = head;
    }

    public String getName() {
        return name;
    }

    public String getHead() {
        return head;
    }
}

class CSSE {
    private List<Institute> institutes;
    private List<Department> departments;

    public CSSE() {
        this.institutes = new ArrayList<>();
        this.institutes = new ArrayList<>();
    }

    // 添加研究所/中心
    public void setInstitute() {
        Institute institute = new Institute("高性能计算研究所", "陈国良");
        institutes.add(institute);
        institute = new Institute("大数据技术与应用研究所", "黄哲学");
        institutes.add(institute);
        institute = new Institute("未来媒体技术与计算研究所", "江健民");
        institutes.add(institute);
        institute = new Institute("网络与信息安全研究所", "李坚强");
        institutes.add(institute);
        institute = new Institute("计算机视觉研究所", "沈琳琳");
        institutes.add(institute);
        institute = new Institute("可视计算研究中心", "黄慧");
        institutes.add(institute);
        institute = new Institute("智能服务计算研究中心","张良杰");
        institutes.add(institute);
        institute = new Institute("智能技术与系统集成研究所", "朱安民");
        institutes.add(institute);
        institute = new Institute("软件工程研究中心", "明仲");
        institutes.add(institute);
    }

    // 添加教学系
    public void setDepartment() {
        Department department = new Department("计算机科学与技术系", "潘微科");
        departments.add(department);
        department = new Department("软件工程系", "张良杰");
        departments.add(department);
        department = new Department("人工智能系", "王熙熙");
        departments.add(department);
    }

    // 获取研究所/中心的名字列表
    public List<String> getInstituteNames() {
        List<String> names = new ArrayList<>();
        for (Institute institute : institutes) {
            names.add(institute.getName() + " - " + institute.getHead());
        }
        return names;
    }

    // 获取教学系的名字列表
    public List<String> getDepartmentNames() {
        List<String> names = new ArrayList<>();
        for (Department department : departments) {
            names.add(department.getName() + " - " + department.getHead());
        }
        return names;
    }

    // 获取研究所/中心的数量
    public int getInstituteNumber() {
        return institutes.size();
    }

    // 获取教学系的数量
    public int getDepartmentNumber() {
        return departments.size();
    }
}

public class Chino {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        CSSE csse = new CSSE();
        csse.setInstitute();
        csse.setDepartment();
        // 输出研究所/中心和教学系的信息
        System.out.println("\nInstitutes:");
        for (String name : csse.getInstituteNames()) {
            System.out.println(name);
        }

        System.out.println("\nDepartments:");
        for (String name : csse.getDepartmentNames()) {
            System.out.println(name);
        }

        System.out.println("\nNumber of Institutes: " + csse.getInstituteNumber());
        System.out.println("Number of Departments: " + csse.getDepartmentNumber());
    }
}
