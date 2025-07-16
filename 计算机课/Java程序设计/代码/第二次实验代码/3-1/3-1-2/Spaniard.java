package com.chino;

public class Spaniard extends Human {
    public Spaniard(String name) {
        super(name);
    }

    @Override
    public void sayHello() {
        System.out.println("Hola, mi nombre es " + name + ".");
    }
}
