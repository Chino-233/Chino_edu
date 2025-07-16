package com.chino;
class PrintChar implements Runnable{
    private  char charToPrint;
    private int times;
    public PrintChar(char c,int t){
        this.charToPrint = c;
        this.times = t;
    }
    @Override
    public void run(){
        for(int i=0;i<times;i++){
            System.out.print(charToPrint);
        }
    }
}
class PrintNum implements Runnable{
    private int lastNum;
    public PrintNum(int n){
        lastNum = n;
    }
    @Override
    public void run(){
        for(int i=1;i<=lastNum;i++){
            System.out.print(" "+i);
        }
    }
}

public class TaskThreadDemo{
    public static void main(String[] args) {
        Runnable PrintA= new PrintChar('a',100);
        Runnable PrintB= new PrintChar('b',100);
        Runnable Print100= new PrintNum(100);
        
        Thread thread1 = new Thread(PrintA);
        Thread thread2 = new Thread(PrintB);
        Thread thread3 = new Thread(Print100);

        thread1.start();
        thread2.start();
        thread3.start();
    }
}
