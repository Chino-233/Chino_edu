package com.chino;

public class Chinese extends Human {
    public Chinese(String name) {
        super(name);
    }

    @Override
    public void sayHello() {
        System.out.println("你好，" + name + "！");
    }
}