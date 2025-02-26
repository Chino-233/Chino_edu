package com.chino;

// 实现 Chinese 类
public class Chinese implements Human {
    private String name;

    public Chinese(String name) {
        this.name = name;
    }

    @Override
    public void sayHello() {
        System.out.println("你好，我叫 " + name);
    }
}
