package 互联网编程.编程练习.chino;

import java.io.*;
import java.util.Scanner;

public class Chino1 {
    public static void main(String[] args) {
        // 初始化输入流
        Scanner scanner = new Scanner(System.in);
        String fileName = "D:\\chino_edu\\互联网编程\\编程练习\\chino\\save.txt";
        // 从键盘读入多行字符串并统计
        StringBuilder content = new StringBuilder();
        System.out.println("请尽情输入文字（输入单独一行的'END'结束输入）：");
        String line;
        while (!(line = scanner.nextLine()).equals("END")) {
            content.append(line).append("\n");
        }
        // 获取用户输入的内容
        String inputContent = content.toString();
        int charCount = inputContent.length();
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName))) {
            // 写入用户输入的内容
            writer.write(inputContent);
            writer.write("This document contains " + charCount + " bytes in total.");
            System.out.println("成功写入文件：" + fileName);
        } catch (IOException e) {
            System.out.println("写入文件时发生错误：" + e.getMessage());
        }
        scanner.close();
    }
}
