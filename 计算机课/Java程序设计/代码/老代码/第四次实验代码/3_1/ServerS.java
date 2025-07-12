

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashSet;

public class ServerS {
    private static final int PORT = 12345; // 定义服务器端口号
    private static HashSet<PrintWriter> writers = new HashSet<>(); // 定义一个HashSet，用于存储PrintWriter对象

    public static void main(String[] args) throws Exception {
        System.out.println("聊天服务器已启动..."); // 输出服务器已启动的信息
        ServerSocket listener = new ServerSocket(PORT); // 创建ServerSocket对象，监听指定端口
        try {
            while (true) {
                new Handler(listener.accept()).start(); // 接受客户端连接，并创建一个新的Handler对象处理连接
            }
        } finally {
            listener.close(); // 关闭ServerSocket对象
        }
    }

    private static class Handler extends Thread {
        private Socket socket; // 声明一个Socket对象
        private PrintWriter out; // 声明一个PrintWriter对象
        private BufferedReader in; // 声明一个BufferedReader对象
        private String name; // 声明一个String类型的name变量

        public Handler(Socket socket) { // 构造函数，传入一个Socket对象
            this.socket = socket; // 将传入的Socket对象赋值给成员变量socket
        }

        public void run() {
            try {
                // 创建输入流
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                // 创建输出流
                out = new PrintWriter(socket.getOutputStream(), true);
                
                // 读取客户端发送的名称
                name = in.readLine();
                // 输出客户端名称
                System.out.println(name + " 已连接");
                // 同步代码块，保证线程安全
                synchronized (writers) {
                    // 将输出流添加到writers集合中
                    writers.add(out);
                }

                String input;
                while ((input = in.readLine()) != null) { // 读取一行输入，直到输入为空
                    for (PrintWriter writer : writers) { // 遍历所有输出流
                        if (writer != out) { // 不发送给发送者自己
                            writer.println(name + ": " + input);
                        }
                    }
                }
            } catch (IOException e) {
                // 捕获IO异常并打印异常信息
                System.out.println(e);
            } finally {
                // 如果输出流不为空，则从writers中移除
                if (out != null) {
                    writers.remove(out);
                }
                // 关闭socket连接
                try {
                    socket.close();
                } catch (IOException e) {}
            }
        }
    }
}
