package com.chino;

import java.io.*;

class FileAccept implements FilenameFilter {
    String str = null;

    FileAccept(String s) {
        str = "." + s;
    }

    public boolean accept(File dir, String name) {
        return name.endsWith(str);
    }
}
public class test {
    public static void main(String[] args) {
        File file= new File("D:\\JAVA\\src\\com\\chino\\input.txt");
        try {
            file
        } catch (Exception e) {
        }
        System.out.println();
    }
}