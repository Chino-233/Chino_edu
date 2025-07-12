package com.chino; // 定义包名

import java.util.Arrays; // 导入Arrays类

public class Chino {
    public static void main(String[] args) {
        float[][][] array = new float[1000][1000][100]; // 创建一个三维浮点数组，大小为1000x1000x100
        for (int i = 0; i < 1000; i++) { // 遍历数组的第一维
            for (int j = 0; j < 1000; j++) { // 遍历数组的第二维
                for (int k = 0; k < 100; k++) { // 遍历数组的第三维
                    array[i][j][k] = (float) Math.random(); // 为数组中的每个元素赋随机浮点数值
                }
            }
        }
        float[] minarray = new float[15]; // 创建一个长度为15的浮点数组，用于存储最小的15个数
        Arrays.fill(minarray, Float.MAX_VALUE); // 将minarray数组的所有元素初始化为Float.MAX_VALUE
        for (int i = 0; i < 1000; i++) { // 遍历数组的第一维
            for (int j = 0; j < 1000; j++) { // 遍历数组的第二维
                for (int k = 0; k < 100; k++) { // 遍历数组的第三维
                    float temp = array[i][j][k]; // 获取当前元素的值
                    if (temp < minarray[14]) { // 如果当前元素小于minarray数组中的最大值
                        minarray[14] = temp; // 将当前元素赋值给minarray数组中的最大值
                        Arrays.sort(minarray); // 对minarray数组进行排序
                    }
                }
            }
        }
        System.err.println("最小的15个数为："); // 输出结果的提示信息
        for (float minnum : minarray) { // 遍历minarray数组
            System.out.print(minnum); // 输出数组中的每个元素
            System.out.print(" "); // 输出空格分隔符
        }
    }
}
