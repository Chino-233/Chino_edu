package com.chino;

import java.util.Random;

public class Chino {
    public static void main(String[] args) {
        Random random = new Random();
        String upperCaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        String lowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
        String numbers = "0123456789";
        String otherCharacters = "!@#$%^&*()_+-=[]{}|;:,.<>?";

        for (int i = 0; i < 5; i++) {
            StringBuilder randomString = new StringBuilder();
            int length = 20; // 随机字符串长度
            for (int j = 0; j < length; j++) {
                int type = random.nextInt(4); // 随机选择字符类型
                switch (type) {
                    case 0:
                        randomString.append(upperCaseLetters.charAt(random.nextInt(upperCaseLetters.length())));
                        break;
                    case 1:
                        randomString.append(lowerCaseLetters.charAt(random.nextInt(lowerCaseLetters.length())));
                        break;
                    case 2:
                        randomString.append(numbers.charAt(random.nextInt(numbers.length())));
                        break;
                    case 3:
                        randomString.append(otherCharacters.charAt(random.nextInt(otherCharacters.length())));
                        break;
                }
            }

            System.out.println("随机生成的字符串: " + randomString.toString());
            parseAndPrint(randomString.toString());
            System.out.println();
        }
    }

    private static void parseAndPrint(String input) {
        StringBuilder upperCase = new StringBuilder();
        StringBuilder lowerCase = new StringBuilder();
        StringBuilder numbers = new StringBuilder();
        StringBuilder others = new StringBuilder();

        for (char c : input.toCharArray()) {
            if (Character.isUpperCase(c)) {
                upperCase.append(c);
            } else if (Character.isLowerCase(c)) {
                lowerCase.append(c);
            } else if (Character.isDigit(c)) {
                numbers.append(c);
            } else {
                others.append(c);
            }
        }

        System.out.println("大写英文字母: " + upperCase.toString());
        System.out.println("小写英文字母: " + lowerCase.toString());
        System.out.println("数字: " + numbers.toString());
        System.out.println("其他字符: " + others.toString());
    }
}
