package com.chino;

public interface Computable {
    Vector add(Vector other);
    Vector minus(Vector other);
    Vector elementwiseProduct(Vector other);
    int innerProduct();
    double norm();
    int compare(Vector other);
}
