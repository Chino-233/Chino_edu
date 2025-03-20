package 互联网编程.编程练习.chino;

import java.io.*;
import java.util.Scanner;

public class learn {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        String fileName = "D:\\chino_edu\\互联网编程\\编程练习\\chino\\save.txt";

        // 程序1：从键盘读取并写入文件
        writeToFile(fileName, scanner);

        // 程序2：从文件读取并去空格显示
        readFromFile(fileName);

        scanner.close();
    }

    // 程序1实现：从键盘读入多行字符串，写入文件并统计字符个数
    private static void writeToFile(String fileName, Scanner scanner) {
        StringBuilder content = new StringBuilder();

        System.out.println("请输入多行文本（输入单独一行的'END'表示结束）：");
        String line;
        while (!(line = scanner.nextLine()).equals("END")) {
            content.append(line).append("\n");
        }

        // 获取用户输入的内容
        String inputContent = content.toString();

        // 统计字符数
        int charCount = inputContent.length();

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName))) {
            // 写入用户输入的内容
            writer.write(inputContent);

            // 写入字符统计信息
            writer.write("This document contains " + charCount + " bytes in total.");

            System.out.println("成功写入文件：" + fileName);
        } catch (IOException e) {
            System.out.println("写入文件时发生错误：" + e.getMessage());
        }
    }

    // 程序2实现：从文件读取内容，去除空格后显示
    private static void readFromFile(String fileName) {
        System.out.println("\n从文件读取内容（去除空格后显示）：");

        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            String line;

            while ((line = reader.readLine()) != null) {
                // 去除空格，保留其他字符
                String noSpaces = line.replace(" ", "");
                System.out.println(noSpaces);
            }
        } catch (IOException e) {
            System.out.println("读取文件时发生错误：" + e.getMessage());
        }
    }
}