package com.chino;

import cn.edu.szu.CSSE;
import java.util.Scanner;

public class Chino {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        CSSE csse = new CSSE();
        csse.setInstitute();
        csse.setDepartment();
        csse.setOffice();
        csse.setLab();
        csse.setJournalEditorial();

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