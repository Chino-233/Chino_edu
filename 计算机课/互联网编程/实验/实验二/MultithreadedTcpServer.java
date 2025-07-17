package 互联网编程.实验二;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class MultithreadedTcpServer {
    private final int port;
    private boolean running;
    private ServerSocket serverSocket;
    private final Logger logger;
    
    public MultithreadedTcpServer(int port) {
        this.port = port;
        this.logger = Logger.getInstance("server_multi_thread.log");
    }
    
    public void start() {
        try {
            serverSocket = new ServerSocket(port);
            running = true;
            logger.log("多线程TCP服务器启动，监听端口: " + port);
            
            // 主循环等待客户端连接
            while (running) {
                try {
                    Socket clientSocket = serverSocket.accept();
                    // 为每个客户端创建一个新线程
                    Thread clientThread = new Thread(new ClientHandler(clientSocket, logger));
                    clientThread.start();
                    logger.log("创建新线程处理客户端连接: " + clientThread.getName());
                } catch (IOException e) {
                    if (running) {
                        logger.logException(e);
                    }
                }
            }
        } catch (IOException e) {
            logger.logException(e);
        } finally {
            stop();
        }
    }
    
    public void stop() {
        running = false;
        try {
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
                logger.log("服务器已关闭");
            }
        } catch (IOException e) {
            logger.logException(e);
        }
    }
}