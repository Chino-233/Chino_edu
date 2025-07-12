package com.chino;

public class HumanTest {
    public static void main(String[] args) {
        // 创建包含3个 Human 对象的数组
        Human[] humans = new Human[3];
        humans[0] = new Chinese("张三");
        humans[1] = new Spaniard("Juan");
        humans[2] = new Italian("Mario");

        // 循环调用数组中元素的 sayHello() 方法
        for (Human human : humans) {
            human.sayHello();
        }
    }
}
