import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class Chino {
    private static final int DEFAULT_PORT = 8080;
    private static final int THREAD_POOL_SIZE = 100;
    public static final String WEB_ROOT = "webroot";

    private final ExecutorService threadPool;
    private volatile boolean running = true;
    private final SessionManager sessionManager;
    private final UserManager userManager;

    public Chino() {
        threadPool = Executors.newFixedThreadPool(THREAD_POOL_SIZE);
        sessionManager = new SessionManager();
        userManager = new UserManager();

        // 创建网站根目录
        File webRootDir = new File(WEB_ROOT);
        if (!webRootDir.exists()) {
            webRootDir.mkdirs();
        }

        System.out.println("Chino HTTP服务器初始化完成");
    }

    public void start() {
        start(DEFAULT_PORT);
    }

    public void start(int port) {
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("HTTP服务器已启动，监听端口: " + port);
            System.out.println("线程池大小: " + THREAD_POOL_SIZE);
            System.out.println("网站根目录: " + WEB_ROOT);

            while (running) {
                try {
                    Socket socket = serverSocket.accept();
                    System.out.println("接受新连接: " + socket.getInetAddress().getHostAddress());

                    // 创建并执行处理器
                    ClientHandler handler = new ClientHandler(socket, this, sessionManager, userManager);
                    threadPool.execute(handler);
                } catch (IOException e) {
                    if (running) {
                        System.err.println("处理连接时出错: " + e.getMessage());
                    }
                }
            }
        } catch (IOException e) {
            System.err.println("服务器启动失败: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public void connectionClosed() {
        // 连接关闭时的处理
    }

    public void shutdown() {
        System.out.println("开始关闭服务器...");
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
        System.out.println("HTTP服务器已关闭");
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