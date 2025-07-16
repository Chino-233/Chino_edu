package com.chino;

public class Chino {
    public static void main(String[] args) {
        Human chinese = new Chinese("张三");
        chinese.sayHello();

        Human spaniard = new Spaniard("Juan");
        spaniard.sayHello();

        Human italian = new Italian("Mario");
        italian.sayHello();
    }
}
