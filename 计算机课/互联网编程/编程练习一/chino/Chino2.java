package 互联网编程.编程练习.chino;

import java.io.*;

public class Chino2 {
    public static void main(String[] args) {
        String fileName = "D:\\chino_edu\\互联网编程\\编程练习\\chino\\save.txt";
        System.out.println("文件读取内容：");
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = reader.readLine()) != null) {
                // 去除空格
                String noSpaces = line.replace(" ", "");
                System.out.println(noSpaces);
            }
        } catch (IOException e) {
            System.out.println("读取文件时发生错误：" + e.getMessage());
        }
    }
}