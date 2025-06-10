
import java.io.*;
import java.net.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.*; // 添加这行导入原子类
import java.util.logging.*;

/**
 * 简易HTTP服务器实现
 * 支持GET、HEAD和POST请求
 * 采用线程池处理并发请求
 */
public class Chino {
    private static final Logger logger = Logger.getLogger(Chino.class.getName());
    private static final int PORT = 8080;
    private static final String WEB_ROOT = "webroot"; // 网站根目录
    private static final int THREAD_POOL_SIZE = 50; // 线程池大小
    
    private final ExecutorService threadPool;
    private volatile boolean running = true;
    
    // 性能统计
    private final AtomicInteger activeConnections = new AtomicInteger(0);
    private final AtomicInteger totalRequests = new AtomicInteger(0);
    private final AtomicLong startTime = new AtomicLong(0);
    
    public Chino() {
        threadPool = Executors.newFixedThreadPool(THREAD_POOL_SIZE);
        
        // 创建网站根目录
        File webRootDir = new File(WEB_ROOT);
        if (!webRootDir.exists() && !webRootDir.mkdirs()) {
            logger.severe("无法创建网站根目录: " + WEB_ROOT);
        }
    }
    
    public void start() {
        startTime.set(System.currentTimeMillis());
        
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            serverSocket.setReuseAddress(true);
            logger.info("HTTP服务器已启动，监听端口: " + PORT);
            
            // 启动性能监控线程
            startPerformanceMonitor();
            
            while (running) {
                try {
                    Socket clientSocket = serverSocket.accept();
                    activeConnections.incrementAndGet();
                    totalRequests.incrementAndGet();
                    
                    // 提交到线程池处理
                    threadPool.execute(new ClientHandler(clientSocket, this));
                } catch (IOException e) {
                    if (running) {
                        logger.log(Level.SEVERE, "接受客户端连接时出错", e);
                    }
                }
            }
        } catch (IOException e) {
            logger.log(Level.SEVERE, "服务器启动失败", e);
        } finally {
            shutdown();
        }
    }
    
    public void shutdown() {
        running = false;
        threadPool.shutdown();
        try {
            if (!threadPool.awaitTermination(5, TimeUnit.SECONDS)) {
                threadPool.shutdownNow();
            }
        } catch (InterruptedException e) {
            threadPool.shutdownNow();
            Thread.currentThread().interrupt();
        }
        logger.info("HTTP服务器已关闭");
    }
    
    private void startPerformanceMonitor() {
        Thread monitorThread = new Thread(() -> {
            while (running) {
                try {
                    Thread.sleep(10000); // 10秒输出一次性能信息
                    
                    long uptime = System.currentTimeMillis() - startTime.get();
                    int active = activeConnections.get();
                    int total = totalRequests.get();
                    double requestsPerSecond = total / (uptime / 1000.0);
                    
                    logger.info(String.format(
                        "性能统计 - 活跃连接: %d, 总请求数: %d, 请求/秒: %.2f, 运行时间: %.2f分钟",
                        active, total, requestsPerSecond, uptime / 60000.0));
                        
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        });
        monitorThread.setDaemon(true);
        monitorThread.start();
    }
    
    void connectionClosed() {
        activeConnections.decrementAndGet();
    }
    
    public static void main(String[] args) {
        Chino server = new Chino();
        server.start();
    }
}