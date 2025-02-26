package com.chino;

import java.util.*;

// 稀疏矩阵类
public class SparseMatrix {

    // 使用HashMap存储稀疏矩阵的非零元素
    private HashMap<Integer, HashMap<Integer, Float>> matrix;
    private int rows, cols; // 矩阵的行数和列数

    // 构造函数，初始化矩阵的行数和列数
    public SparseMatrix(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
        this.matrix = new HashMap<>();
    }

    // 添加非零元素到矩阵中
    public void addValue(int row, int col, float value) {
        if (value != 0) { // 仅存储非零元素
            matrix.computeIfAbsent(row, k -> new HashMap<>()).put(col, value);
        }
    }

    // 矩阵加法方法
    public SparseMatrix add(SparseMatrix other) {
        // 确保两个矩阵的维度一致
        if (this.rows != other.rows || this.cols != other.cols) {
            throw new IllegalArgumentException("矩阵维度不匹配，无法进行加法运算");
        }

        SparseMatrix result = new SparseMatrix(this.rows, this.cols);
        
        // 遍历当前矩阵，将值添加到结果矩阵
        for (int row : this.matrix.keySet()) {
            for (int col : this.matrix.get(row).keySet()) {
                result.addValue(row, col, this.matrix.get(row).get(col));
            }
        }

        // 遍历另一个矩阵，将值添加到结果矩阵
        for (int row : other.matrix.keySet()) {
            for (int col : other.matrix.get(row).keySet()) {
                result.addValue(row, col, result.matrix.getOrDefault(row, new HashMap<>()).getOrDefault(col, 0f) + other.matrix.get(row).get(col));
            }
        }

        return result;
    }

    // 矩阵乘法方法
    public SparseMatrix multiply(SparseMatrix other) {
        // 确保当前矩阵的列数等于另一个矩阵的行数
        if (this.cols != other.rows) {
            System.out.println("矩阵维度不匹配，无法进行乘法运算");
        }

        SparseMatrix result = new SparseMatrix(this.rows, other.cols);

        // 遍历当前矩阵的所有非零元素
        for (int row : this.matrix.keySet()) {
            for (int col : this.matrix.get(row).keySet()) {
                float valueA = this.matrix.get(row).get(col);
                
                // 如果另一矩阵在对应位置有值
                if (other.matrix.containsKey(col)) {
                    for (int otherCol : other.matrix.get(col).keySet()) {
                        float valueB = other.matrix.get(col).get(otherCol);
                        // 结果矩阵元素累加
                        result.addValue(row, otherCol, result.matrix.getOrDefault(row, new HashMap<>()).getOrDefault(otherCol, 0f) + valueA * valueB);
                    }
                }
            }
        }

        return result;
    }

    // 打印矩阵的非零元素，以三元组的形式输出
    public void print() {
        for (int row : matrix.keySet()) {
            for (int col : matrix.get(row).keySet()) {
                System.out.printf("%d %d %.2f\n", row, col, matrix.get(row).get(col));
            }
        }
    }

    public static void main(String[] args) {
        
        // 初始化第一个矩阵
        SparseMatrix matrix1 = new SparseMatrix(3, 4);
        matrix1.addValue(0, 0, 1.2f);
        matrix1.addValue(1, 2, 3.1f);
        matrix1.addValue(2, 3, 2.2f);

        // 初始化第二个矩阵
        SparseMatrix matrix2 = new SparseMatrix(4, 5);
        matrix2.addValue(0, 1, 1.1f);
        matrix2.addValue(0, 2, 1.5f);
        matrix2.addValue(2, 2, 3.2f);
        matrix2.addValue(2, 4, -3.2f);
        matrix2.addValue(3, 0, -1.0f);
        matrix2.addValue(3, 1, 6.2f);

        // 矩阵乘法
        System.out.println("矩阵乘法结果：");
        SparseMatrix resultMul = matrix1.multiply(matrix2);
        resultMul.print();

        // 初始化第三个矩阵
        SparseMatrix matrix3 = new SparseMatrix(3, 4);
        matrix3.addValue(0, 0, 1.2f);
        matrix3.addValue(0, 1, -3f);
        matrix3.addValue(0, 2, -5.3f);
        matrix3.addValue(1, 1, 1f);
        matrix3.addValue(1, 2, 0.1f);
        matrix3.addValue(1, 3, -0.4f);
        matrix3.addValue(2, 0, 2f);
        matrix3.addValue(2, 1, 2f);
        matrix3.addValue(2, 2, 1f);
        matrix3.addValue(2, 3, 0.2f);

        // 矩阵加法
        System.out.println("\n矩阵加法结果：");
        SparseMatrix resultAdd = matrix1.add(matrix3);
        resultAdd.print();
    }
}
