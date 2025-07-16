package com.chino;

import java.math.BigInteger;
import java.util.Random;

public class Chino {
    public static void main(String[] args) {
        // 创建一个Random对象用于生成随机数
        Random random = new Random();

        // 生成5个21位数
        for (int i = 0; i < 5; i++) {
            // 生成一个21位的随机数
            BigInteger randomNumber = new BigInteger(210, random).mod(BigInteger.TEN.pow(21)).add(BigInteger.TEN.pow(20));
            // 获取逆序数,即将数字反过来
            BigInteger reversedNumber = new BigInteger(new StringBuilder(randomNumber.toString()).reverse().toString());
            // 通过翻转前后是否相同判断是否是回文
            boolean isPalindrome = randomNumber.equals(reversedNumber);
            // 输出随机数、逆序数和是否是回文
            System.out.println("随机数: " + randomNumber);
            System.out.println("是否是回文: " + isPalindrome);
        }
    }
}