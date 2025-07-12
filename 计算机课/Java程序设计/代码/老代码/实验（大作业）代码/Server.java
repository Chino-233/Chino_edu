package com.chino;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashSet;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server {
    private static final int PORT = 12345; // 定义服务器端口号
    private static HashSet<PrintWriter> writers = new HashSet<>(); // 定义存储客户端输出流的集合
    private static ExecutorService executorService = Executors.newFixedThreadPool(10); // 创建一个固定大小为10的线程池

    public static void main(String[] args) throws Exception { // 主方法，程序入口
        System.out.println("AI询问服务器已启动..."); // 输出服务器启动信息
        ServerSocket listener = new ServerSocket(PORT); // 创建ServerSocket对象，监听指定端口
        try {
            while (true) { // 无限循环，持续监听客户端连接
                executorService.submit(new Handler(listener.accept())); // 接受客户端连接，并提交给线程池处理
            }
        } finally {
            listener.close(); // 关闭ServerSocket
        }
    }

    private static class Handler implements Runnable { // 定义Handler内部类，实现Runnable接口
        private Socket socket; // 定义Socket对象
        private PrintWriter out; // 定义PrintWriter对象
        private BufferedReader in; // 定义BufferedReader对象
        private String model; // 定义模型名称

        public Handler(Socket socket) { // 构造方法，初始化Socket对象
            this.socket = socket;
        }

        public void run() { // 实现run方法
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream())); // 初始化输入流
                out = new PrintWriter(socket.getOutputStream(), true); // 初始化输出流

                model = in.readLine(); // 读取客户端发送的模型名称
                synchronized (writers) { // 同步代码块，保证线程安全
                    writers.add(out); // 将输出流添加到集合中
                }

                String input; // 定义输入字符串
                while ((input = in.readLine()) != null) { // 循环读取客户端发送的消息
                    String response = handleAIResponse(input); // 调用handleAIResponse方法处理消息
                    System.err.println("用户: " + response);
                    for (PrintWriter writer : writers) { // 遍历输出流集合
                        writer.println(response); // 将响应发送给所有客户端
                        writer.println("END"); // 发送结束标志
                    }
                }
            } catch (IOException e) { // 捕获IO异常
                System.out.println(e); // 输出异常信息
            } finally {
                if (out != null) { // 如果输出流不为空
                    writers.remove(out); // 从集合中移除输出流
                }
                try {
                    socket.close(); // 关闭Socket
                } catch (IOException e) { // 捕获IO异常
                }
            }
        }

        private String handleAIResponse(String input) { // 定义处理AI响应的方法
            if ("腾讯混元".equals(model)) { // 如果模型是“腾讯混元”
                String response = TencentAIHandler.callTencentCloudAPI(input); // 调用腾讯云API处理消息
                System.err.println("腾讯混元: " + response);
                return response;
            } else if ("阿里通义千问".equals(model)) { // 如果模型是“阿里通义千问”
                try {
                    String response = AlibabaAIHandler.callWithMessage(input, "zh", "en"); // 调用阿里云API处理消息
                    System.err.println("阿里通义千问: " + response);
                    return response;
                } catch (Exception e) { // 捕获异常
                    e.printStackTrace(); // 打印异常堆栈信息
                    return "对话失败,请更换对话内容"; // 返回错误信息
                }
            }
            return "未知模型"; // 返回未知模型信息
        }
    }
}
