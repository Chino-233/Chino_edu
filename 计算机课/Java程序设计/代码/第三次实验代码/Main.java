package com.chino;

import java.text.DecimalFormat;
import java.util.*;

class Country implements Comparable<Country> {
    private String name;
    private double GDP2023;
    private int Olympics2024;

    // 构造函数，初始化国家名称、GDP和奥运会奖牌的数量
    public Country(String name, double GDP2023, int Olympics2024) {
        this.name = name;
        this.GDP2023 = GDP2023;
        this.Olympics2024 = Olympics2024;
    }

    // 获取国家名称
    public String getName() {
        return name;
    }

    // 获取GDP
    public double getGDP2023() {
        return GDP2023;
    }

    // 获取奥运会的数量
    public int getOlympics2024() {
        return Olympics2024;
    }

    // 1. 按照Olympics2024从大到小排序
    @Override
    public int compareTo(Country o) {
        return Integer.compare(o.Olympics2024, this.Olympics2024);
    }

    @Override
    public String toString() {
        DecimalFormat df = new DecimalFormat("#,###");
        return name + ", GDP2023: " + df.format(GDP2023) + "百万美元, Olympics2024: " + Olympics2024;
    }

    // 2. Comparator实现：按GDP2023从大到小排序
    public static class GDPComparator implements Comparator<Country> {
        @Override
        public int compare(Country c1, Country c2) {
            return Double.compare(c2.getGDP2023(), c1.getGDP2023());
        }
    }

    // 3. Comparator实现：按Olympics2024从大到小排序
    public static class OlympicsComparator implements Comparator<Country> {
        @Override
        public int compare(Country c1, Country c2) {
            return Integer.compare(c2.getOlympics2024(), c1.getOlympics2024());
        }
    }
}

public class Main {
    public static void main(String[] args) {
        // 创建按奥运会奖牌数量排序的TreeSet
        TreeSet<Country> setByOlympics = new TreeSet<>(new Country.OlympicsComparator());
        // 创建按GDP排序的TreeSet
        TreeSet<Country> setByGDP = new TreeSet<>(new Country.GDPComparator());

        // 数据输入
        List<Country> countries = Arrays.asList(
                new Country("美国", 27361000, 126),
                new Country("中华人民共和国", 17795000, 91),
                new Country("德国", 4456000, 33),
                new Country("日本", 4213000, 45),
                new Country("印度", 3550000, 6),
                new Country("英国", 3340000, 65),
                new Country("法国", 3031000, 64),
                new Country("意大利", 2255000, 40),
                new Country("加拿大", 2140000, 27),
                new Country("韩国", 1713000, 32),
                new Country("以色列", 5100000, 7),
                new Country("俄罗斯", 2021000, 0)
        );

        // (i) 按Olympics2024排序
        setByOlympics.addAll(countries);
        System.out.println("Olympics2024排序 (从大到小):");
        for (Country country : setByOlympics) {
            System.out.println(country);
        }

        // (ii) 按GDP2023排序
        setByGDP.addAll(countries);
        System.out.println("\nGDP2023排序 (从大到小):");
        for (Country country : setByGDP) {
            System.out.println(country);
        }
    }
}
