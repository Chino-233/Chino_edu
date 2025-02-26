package com.chino;

public class Chino {
    public static void main(String[] args) {
        Vector v1 = new Vector(3, 9, 2, 7);
        Vector v2 = new Vector(2, -8, -1, 6);

        System.out.println("v1: " + v1);
        System.out.println("v2: " + v2);

        System.out.println("v1 + v2: " + v1.add(v2));
        System.out.println("v1 - v2: " + v1.minus(v2));
        System.out.println("v1 * v2: " + v1.elementwiseProduct(v2));
        System.out.println("Inner product of v1: " + v1.innerProduct());
        System.out.println("Norm of v1: " + v1.norm());
        System.out.println("Norm of v2: " + v2.norm());
        System.out.println("Comparison of v1 and v2: " + v1.compare(v2));
    }
}
