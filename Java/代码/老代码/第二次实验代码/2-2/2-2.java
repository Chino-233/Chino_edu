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

class Office {
    private String name;
    private String head;

    public Office(String name, String head) {
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

class Lab {
    private String name;
    private String head;

    public Lab(String name, String head) {
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

class JournalEditorial {
    private String name;
    private String head;

    public JournalEditorial(String name, String head) {
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
    private List<Office> offices;
    private List<Lab> labs;
    private List<JournalEditorial> journalEditorials;

    public CSSE() {
        this.institutes = new ArrayList<>();
        this.departments = new ArrayList<>();
        this.offices = new ArrayList<>();
        this.labs = new ArrayList<>();
        this.journalEditorials = new ArrayList<>();
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

    // 添加行政办公室
    public void setOffice() {
        Office office = new Office("党务工作", "杨国洪");
        offices.add(office);
        office = new Office("教学业务", "胡沛");
        offices.add(office);
        office = new Office("实验中心", "林佳利");
        offices.add(office);
        office = new Office("辅导员", "黄晓聪");
        offices.add(office);
        office = new Office("科研外事", "何文锋");
        offices.add(office);
        office = new Office("综合业务", "刘晔");
        offices.add(office);
    }

    // 添加实验中心
    public void setLab() {
        Lab lab = new Lab("实验中心", "黄慧");
        labs.add(lab);
    }

    // 添加期刊编辑部
    public void setJournalEditorial() {
        JournalEditorial journalEditorial = new JournalEditorial("机器学习与控制论国际期刊", "王熙照");
        journalEditorials.add(journalEditorial);
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

    // 获取行政办公室的名字列表
    public List<String> getOfficeNames() {
        List<String> names = new ArrayList<>();
        for (Office office : offices) {
            names.add(office.getName() + " - " + office.getHead());
        }
        return names;
    }

    // 获取实验中心的名字列表
    public List<String> getLabNames() {
        List<String> names = new ArrayList<>();
        for (Lab lab : labs) {
            names.add(lab.getName() + " - " + lab.getHead());
        }
        return names;
    }

    // 获取期刊编辑部名字列表
    public List<String> getJournalEditorialNames() {
        List<String> names = new ArrayList<>();
        for (JournalEditorial journalEditorial : journalEditorials) {
            names.add(journalEditorial.getName() + " - " + journalEditorial.getHead());
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

    // 获取行政办公室的数量
    public int getOfficeNumber() {
        return offices.size();
    }

    // 获取实验中心的数量
    public int getLabNumber() {
        return labs.size();
    }

    // 获取期刊编辑部数量
    public int getJournalEditorialNumber() {
        return journalEditorials.size();
    }
}

public class Chino {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        CSSE csse = new CSSE();
        csse.setInstitute();
        csse.setDepartment();
        csse.setOffice();
        csse.setLab();
        csse.setJournalEditorial();
        // 输出研究所/中心和教学系的信息
        System.out.println("\n研究所/中心:");
        for (String name : csse.getInstituteNames()) {
            System.out.println(name);
        }

        System.out.println("\n教学系:");
        for (String name : csse.getDepartmentNames()) {
            System.out.println(name);
        }

        System.out.println("\n行政办公室:");
        for (String name : csse.getOfficeNames()) {
            System.out.println(name);
        }

        System.out.println("\n实验中心:");
        for (String name : csse.getLabNames()) {
            System.out.println(name);
        }

        System.out.println("\n期刊编辑部:");
        for (String name : csse.getJournalEditorialNames()) {
            System.out.println(name);
        }

        System.out.println("\n研究所/中心数量: " + csse.getInstituteNumber());
        System.out.println("教学系数量: " + csse.getDepartmentNumber());
        System.out.println("行政办公室数量: " + csse.getOfficeNumber());
        System.out.println("实验中心数量: " + csse.getLabNumber());
        System.out.println("期刊编辑部数量: " + csse.getJournalEditorialNumber());
    }
}
