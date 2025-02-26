package com.chino;

public class Italian extends Human {
    public Italian(String name) {
        super(name);
    }

    @Override
    public void sayHello() {
        System.out.println("Ciao, " + name + "!");
    }
}