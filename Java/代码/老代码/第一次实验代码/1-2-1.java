package com.chino;

import java.util.Arrays;

public class Chino {
    public static void main(String[] args) {
        float[][][] array=new float[1000][1000][100];
        for(int i=0;i<1000;i++){
            for(int j=0;j<1000;j++){
                for(int k=0;k<100;k++){
                    array[i][j][k]=(float)Math.random();
                }
            }
        }
        float[] minarray=new float[15];
        Arrays.fill(minarray,Float.MAX_VALUE);
        for(int i=0;i<1000;i++){
            for(int j=0;j<1000;j++){
                for(int k=0;k<100;k++){
                    float temp=array[i][j][k];
                    if(temp<minarray[14]){
                        minarray[14]=temp;
                        Arrays.sort(minarray);
                    }
                }
            }
        }
        System.err.println("最小的15个数为：");
        for(float minnum:minarray){
            System.out.print(minnum);
            System.out.print(" ");
        }
    }
}
