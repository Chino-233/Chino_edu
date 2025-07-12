package com.chino;

// 实现 Spaniard 类
public class Spaniard implements Human {
    private String name;

    public Spaniard(String name) {
        this.name = name;
    }

    @Override
    public void sayHello() {
        System.out.println("Hola, me llamo " + name);
    }
}
