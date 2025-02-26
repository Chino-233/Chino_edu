package com.chino;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.*;

public class Chino_1 {
    public static void main(String[] args) throws IOException, InterruptedException {
        // 读取review.txt文件的所有行
        List<String> lines = Files.readAllLines(Paths.get("D:\\JAVA\\src\\com\\chino\\review.txt"));

        // 用于存储用户-商品映射和商品-用户映射
        Map<String, Set<String>> userProducts = new HashMap<>();
        Map<String, Set<String>> productUsers = new HashMap<>();

        // 遍历每一行，构建用户-商品映射和商品-用户映射
        for (String line : lines) {
            String[] parts = line.split(";");
            String user = parts[0];
            String product = parts[1];

            userProducts.computeIfAbsent(user, k -> new HashSet<>()).add(product);
            productUsers.computeIfAbsent(product, k -> new HashSet<>()).add(user);
        }

        // 获取所有商品的列表
        List<String> products = new ArrayList<>(productUsers.keySet());
        // 用于存储每个商品的最相似的三个商品
        Map<String, List<String>> productNeighbors = new ConcurrentHashMap<>();

        // 创建一个线程池
        ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        List<Future<?>> futures = new ArrayList<>();

        // 遍历每个商品，计算其与其他商品的相似度
        for (String product : products) {
            futures.add(executor.submit(() -> {
                List<String> neighbors = products.stream()
                        .filter(other -> !other.equals(product))
                        .map(other -> new AbstractMap.SimpleEntry<>(other, jaccardIndex(productUsers.get(product), productUsers.get(other))))
                        .sorted(Comparator.comparing((AbstractMap.SimpleEntry<String, Double> entry) -> -entry.getValue())
                                .thenComparing(Map.Entry::getKey))
                        .limit(3)
                        .map(Map.Entry::getKey)
                        .collect(Collectors.toList());
                productNeighbors.put(product, neighbors);
            }));
        }

        // 等待所有任务完成，并处理可能的异常
        for (Future<?> future : futures) {
            try {
                future.get();
            } catch (ExecutionException e) {
                e.printStackTrace();
            }
        }

        // 关闭线程池
        executor.shutdown();

        // 创建一个BufferedWriter对象，用于写入productNeighborhood.txt文件
        try (BufferedWriter writer = Files.newBufferedWriter(Paths.get("D:\\JAVA\\src\\com\\chino\\productNeighborhood.txt"))) {
            // 按照productId排序，并将每个商品的最相似的三个商品写入productNeighborhood.txt文件
            for (String product : products.stream().sorted().collect(Collectors.toList())) {
                List<String> neighbors = productNeighbors.get(product);
                writer.write(product + ";" + String.join(";", neighbors));
                writer.newLine();
            }
        }
    }

    // 计算两个集合之间的Jaccard相似度
    private static double jaccardIndex(Set<String> set1, Set<String> set2) {
        Set<String> intersection = new HashSet<>(set1);
        intersection.retainAll(set2);
        Set<String> union = new HashSet<>(set1);
        union.addAll(set2);
        return (double) intersection.size() / union.size();
    }
}
