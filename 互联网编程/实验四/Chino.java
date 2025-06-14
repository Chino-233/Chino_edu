import java.io.*;
import java.net.*;
import java.util.concurrent.*;
import java.util.logging.*;

public class Chino {
    private static final int DEFAULT_PORT = 8080;
    private static final int THREAD_POOL_SIZE = 100;
    public static final String WEB_ROOT = "webroot";
    private static final Logger logger = Logger.getLogger(Chino.class.getName());

    private final ExecutorService threadPool;
    private volatile boolean running = true;
    private final SessionManager sessionManager;
    private final UserManager userManager;

    public Chino() {
        threadPool = Executors.newFixedThreadPool(THREAD_POOL_SIZE);
        sessionManager = new SessionManager();
        userManager = new UserManager(); // 创建UserManager实例

        // 创建网站根目录
        File webRootDir = new File(WEB_ROOT);
        if (!webRootDir.exists() && !webRootDir.mkdirs()) {
            logger.severe("无法创建网站根目录: " + WEB_ROOT);
        }
    }

    public void start() {
        start(DEFAULT_PORT);
    }

    public void start(int port) {
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            logger.info("HTTP服务器已启动，监听端口: " + port);

            while (running) {
                try {
                    Socket socket = serverSocket.accept();

                    // 创建并执行处理器
                    ClientHandler handler = new ClientHandler(socket, this, sessionManager, userManager);
                    threadPool.execute(handler);
                } catch (IOException e) {
                    if (running) {
                        logger.log(Level.SEVERE, "处理连接时出错", e);
                    }
                }
            }
        } catch (IOException e) {
            logger.log(Level.SEVERE, "服务器启动失败", e);
        }
    }

    public void connectionClosed() {
    }

    public void shutdown() {
        running = false;

        sessionManager.shutdown();
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

    public static void main(String[] args) {
        final Chino server = new Chino();

        // 添加关闭钩子
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("正在关闭服务器...");
            server.shutdown();
        }));

        server.start();
    }
}