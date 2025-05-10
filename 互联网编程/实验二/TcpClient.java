package 互联网编程.实验二;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;

/**
 * TCP客户端，用于与服务器通信和压力测试
 */
public class TcpClient implements Runnable {
    private final String serverAddress;
    private final int serverPort;
    private final int messageCount;
    private final CountDownLatch latch;
    private final boolean interactive;
    
    public TcpClient(String serverAddress, int serverPort) {
        this(serverAddress, serverPort, 1, null, true);
    }
    
    public TcpClient(String serverAddress, int serverPort, int messageCount, CountDownLatch latch) {
        this(serverAddress, serverPort, messageCount, latch, false);
    }
    
    public TcpClient(String serverAddress, int serverPort, int messageCount, 
                    CountDownLatch latch, boolean interactive) {
        this.serverAddress = serverAddress;
        this.serverPort = serverPort;
        this.messageCount = messageCount;
        this.latch = latch;
        this.interactive = interactive;
    }
    
    @Override
    public void run() {
        try (
            Socket socket = new Socket(serverAddress, serverPort);
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedReader stdIn = interactive ? new BufferedReader(new InputStreamReader(System.in)) : null
        ) {
            if (interactive) {
                System.out.println("已连接到服务器 " + serverAddress + ":" + serverPort);
                System.out.println("输入消息发送到服务器 (输入'exit'退出):");
                
                // 交互模式
                String userInput;
                while ((userInput = stdIn.readLine()) != null) {
                    out.println(userInput);
                    System.out.println("服务器响应: " + in.readLine());
                    
                    if ("exit".equalsIgnoreCase(userInput)) {
                        break;
                    }
                }
            } else {
                // 压力测试模式
                long startTime = System.currentTimeMillis();
                
                for (int i = 0; i < messageCount; i++) {
                    String message = "Test message " + i + " from " + Thread.currentThread().getName();
                    out.println(message);
                    in.readLine(); // 读取响应但不处理
                }
                
                long endTime = System.currentTimeMillis();
                System.out.println(Thread.currentThread().getName() + 
                                 " 完成 " + messageCount + " 个请求，耗时: " + 
                                 (endTime - startTime) + "ms");
            }
            
        } catch (IOException e) {
            System.err.println("客户端错误: " + e.getMessage());
        } finally {
            if (latch != null) {
                latch.countDown();
            }
        }
    }
    
    public static void main(String[] args) {
        // 默认参数
        String server = "localhost";
        int port = 1204;
        
        // 启动交互式客户端
        new TcpClient(server, port).run();
    }
}