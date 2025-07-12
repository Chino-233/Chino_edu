package 互联网编程.实验二;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ThreadPoolTcpServer {
    private final int port;
    private boolean running;
    private ServerSocket serverSocket;
    private final Logger logger;
    private final ExecutorService threadPool;
    private final int poolSize;
    
    public ThreadPoolTcpServer(int port) {
        this(port, Runtime.getRuntime().availableProcessors() * 2);
    }
    
    public ThreadPoolTcpServer(int port, int poolSize) {
        this.port = port;
        this.poolSize = poolSize;
        this.logger = Logger.getInstance("server_thread_pool.log");
        this.threadPool = Executors.newFixedThreadPool(poolSize);
    }
    
    public void start() {
        try {
            serverSocket = new ServerSocket(port);
            running = true;
            logger.log("线程池TCP服务器启动，监听端口: " + port + "，线程池大小: " + poolSize);
            
            // 主循环等待客户端连接
            while (running) {
                try {
                    Socket clientSocket = serverSocket.accept();
                    // 将客户端连接提交到线程池处理
                    threadPool.execute(new ClientHandler(clientSocket, logger));
                    logger.log("线程池分配了一个线程处理客户端连接");
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
        
        if (threadPool != null && !threadPool.isShutdown()) {
            threadPool.shutdown();
            logger.log("线程池已关闭");
        }
    }
}