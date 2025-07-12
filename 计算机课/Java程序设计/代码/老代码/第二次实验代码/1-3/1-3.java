package com.chino; // 定义包名为 com.chino，引入包

class Queue{
    double[] elements; // 定义一个数组，用于存储double型数据
    int size; // 定义一个整数，用于表示存储的double型数据的个数
    public Queue(){ // 构造方法，初始化队列的容量为32
        elements = new double[32]; // 初始化数组
    }
    public void enQueue(double v){ // 方法，往队列中添加一个double型数据
        elements[size++] = v; // 将数据添加到数组中，并将size加1
    }
    public double deQueue(){ // 方法，从队列中删除并返回一个double型数据
        return elements[--size]; // 将数组中的最后一个元素删除，并将size减1，返回该元素
    }
    public double getHead(){ // 方法，返回队列中的第一个元素
        return elements[0]; // 返回数组中的第一个元素
    }
    public double getTail(){ // 方法，返回队列中的最后一个元素
        return elements[size-1]; // 返回数组中的最后一个元素
    }
    public boolean isEmpty(){ // 方法，判断队列是否为空
        return size == 0; // 如果size为0，则队列为空，返回true；否则返回false
    }
    public boolean isFull(){ // 方法，判断队列是否为满
        return size == elements.length; // 如果size等于数组长度，则队列已满，返回true；否则返回false
    }
    public int getSize(){ // 方法，返回队列的大小
        return size; // 返回size
    }
}
// 定义一个名为 Chino 的公共类
public class Chino {
    // 主方法，程序的入口点
    public static void main(String[] args) {
        Queue queue = new Queue(); // 创建一个队列对象
        for(int i = 0; i < 32; i++){ // 循环32次，往队列中添加数据
            queue.enQueue(i+0.01); // 往队列中添加数据
        }
        System.out.println(queue.getHead()); // 输出队列中的第一个元素
        System.out.println(queue.getTail()); // 输出队列中的最后一个元素
        System.out.println(queue.isEmpty()); // 输出队列是否为空
        System.out.println(queue.isFull()); // 输出队列是否为满
        queue.deQueue(); // 从队列中删除一个元素
        System.out.println(queue.getSize()); // 输出队列的大小
    }
}
