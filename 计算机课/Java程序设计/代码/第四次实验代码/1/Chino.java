package com.chino;

import java.io.*;
import java.nio.file.*;
import java.util.*;

public class Chino {
    public static void main(String[] args) throws IOException {
        // 读取Watches.txt文件的所有行
        List<String> lines = Files.readAllLines(Paths.get("D:\\JAVA\\src\\com\\chino\\Watches.txt"));

        // 用于存储userId和productId的列表
        List<String[]> userProductPairs = new ArrayList<>();

        // 临时变量，用于存储当前的productId和userId
        String currentProductId = null;
        String currentUserId = null;

        // 遍历每一行，提取productId和userId
        // 遍历每一行
        for (String line : lines) {
            // 如果行以"product/productId: "开头
            if (line.startsWith("product/productId: ")) {
                // 获取productId
                currentProductId = line.substring(18).trim();
            // 如果行以"review/userId: "开头
            } else if (line.startsWith("review/userId: ")) {
                // 获取userId
                currentUserId = line.substring(15).trim();
                // 如果userId不是unknown且productId不为空
                if (!"unknown".equals(currentUserId) && currentProductId != null) {
                    // 将userId和productId添加到userProductPairs中
                    userProductPairs.add(new String[]{currentUserId, currentProductId});
                }
            }
        }

        // 按照userId和productId排序
        userProductPairs.sort(Comparator.comparing((String[] pair) -> pair[0])
                .thenComparing(pair -> pair[1]));

        // 创建一个BufferedWriter对象，用于写入review.txt文件
        try (BufferedWriter writer = Files.newBufferedWriter(Paths.get("D:\\JAVA\\src\\com\\chino\\review.txt"))) {
            // 遍历过滤后的行，将每行的userId和productId写入review.txt文件
            for (String[] pair : userProductPairs) {
                writer.write(pair[0] + ";" + pair[1]);
                writer.newLine();
            }
        }
    }
}
