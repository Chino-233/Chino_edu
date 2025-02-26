package com.chino;

// 实现 Italian 类
public class Italian implements Human {
    private String name;

    public Italian(String name) {
        this.name = name;
    }

    @Override
    public void sayHello() {
        System.out.println("Ciao, mi chiamo " + name);
    }
}
