package 互联网编程.实验二;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Date;

/**
 * 客户端连接处理接口
 * 统一处理客户端请求的逻辑
 */
public class ClientHandler implements Runnable {
    private final Socket clientSocket;
    private final Logger logger;
    private final String clientIp;
    private final int clientPort;
    
    public ClientHandler(Socket socket, Logger logger) {
        this.clientSocket = socket;
        this.logger = logger;
        this.clientIp = socket.getInetAddress().getHostAddress();
        this.clientPort = socket.getPort();
    }
    
    @Override
    public void run() {
        logger.logClientConnected(clientIp, clientPort);
        
        try (
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)
        ) {
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                // 简单的回显服务
                logger.log("收到来自 " + clientIp + ":" + clientPort + " 的消息: " + inputLine);
                
                // 处理客户端发来的消息
                String response = processMessage(inputLine);
                
                // 发送响应
                out.println(response);
                logger.log("回复 " + clientIp + ":" + clientPort + " 消息: " + response);
                
                // 如果客户端发送"exit"，结束对话
                if ("exit".equalsIgnoreCase(inputLine)) {
                    break;
                }
            }
        } catch (IOException e) {
            logger.logException(e);
        } finally {
            try {
                clientSocket.close();
                logger.logClientDisconnected(clientIp, clientPort);
            } catch (IOException e) {
                logger.logException(e);
            }
        }
    }
    
    /**
     * 处理客户端消息的逻辑
     * @param message 客户端发送的消息
     * @return 服务器的响应
     */
    private String processMessage(String message) {
        // 简单的回显处理，可以根据需要扩展
        if ("time".equalsIgnoreCase(message)) {
            return "服务器时间: " + new Date();
        } else if ("hello".equalsIgnoreCase(message)) {
            return "你好，客户端!";
        } else if ("exit".equalsIgnoreCase(message)) {
            return "再见!";
        }
        return "服务器回声: " + message;
    }
}