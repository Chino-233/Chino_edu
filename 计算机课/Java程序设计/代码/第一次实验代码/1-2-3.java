package com.chino;
public class Chino {
    public static void main(String[] args) {
        int count=0;
        for(int i=0;i<10000;i++){
            if(isPrime(i)){
                count++;
                System.err.print(i);
                System.err.print(" ");
            }
        }
        System.err.println("一共有"+count+"个素数");
    }
    public static boolean isPrime(int num){
        if(num==0||num==1){
            return false;
        }
        for(int i=2;i<=Math.sqrt(num);i++){
            if(num%i==0){
                return false;
            }
        }
        return true;
    }
}
