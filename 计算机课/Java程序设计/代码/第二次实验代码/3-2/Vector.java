package com.chino;

public class Vector implements Computable {
    private int x;
    private int y;
    private int z;
    private int w;

    public Vector(int x, int y, int z, int w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    @Override
    public Vector add(Vector other) {
        return new Vector(this.x + other.x, this.y + other.y, this.z + other.z, this.w + other.w);
    }

    @Override
    public Vector minus(Vector other) {
        return new Vector(this.x - other.x, this.y - other.y, this.z - other.z, this.w - other.w);
    }

    @Override
    public Vector elementwiseProduct(Vector other) {
        return new Vector(this.x * other.x, this.y * other.y, this.z * other.z, this.w * other.w);
    }

    @Override
    public int innerProduct() {
        return this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w;
    }

    @Override
    public double norm() {
        return Math.sqrt(innerProduct());
    }

    @Override
    public int compare(Vector other) {
        return Double.compare(this.norm(), other.norm());
    }

    @Override
    public String toString() {
        return "[" + x + ", " + y + ", " + z + ", " + w + "]";
    }
}
