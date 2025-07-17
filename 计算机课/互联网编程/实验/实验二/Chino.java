package 互联网编程.实验二;
import java.util.Scanner;

/**
 * Java多线程TCP服务器实验主类
 * 实现两种不同线程管理技术的TCP服务器并分析其性能
 */
public class Chino {
    // 默认服务器端口
    private static final int DEFAULT_PORT = 1204;
    
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("======= TCP服务器线程编程实验 =======");
        System.out.println("请选择要启动的服务器类型:");
        System.out.println("1. 传统多线程服务器");
        System.out.println("2. 线程池服务器");
        System.out.println("3. 运行性能对比测试");
        System.out.print("请输入选择 (1-3): ");
        
        int choice = scanner.nextInt();
        
        switch (choice) {
            case 1:
                startMultithreadedServer();
                break;
            case 2:
                startThreadPoolServer();
                break;
            case 3:
                runPerformanceTest();
                break;
            default:
                System.out.println("无效选择，程序退出");
        }
        
        scanner.close();
    }
    
    private static void startMultithreadedServer() {
        System.out.println("启动传统多线程TCP服务器...");
        System.out.println("服务器正在运行，按Enter键停止服务器...");
        final MultithreadedTcpServer server = new MultithreadedTcpServer(DEFAULT_PORT);
        
        // 在独立线程中运行服务器
        Thread serverThread = new Thread(() -> {
            server.start();
        });
        serverThread.start();
        
        // 等待用户输入以停止服务器
        new Scanner(System.in).nextLine();
        System.out.println("正在停止服务器...");
        server.stop();
    }
    
    private static void startThreadPoolServer() {
        System.out.println("启动线程池TCP服务器...");
        System.out.println("服务器正在运行，按Enter键停止服务器...");
        final ThreadPoolTcpServer server = new ThreadPoolTcpServer(DEFAULT_PORT);
        
        // 在独立线程中运行服务器
        Thread serverThread = new Thread(() -> {
            server.start();
        });
        serverThread.start();
        
        // 等待用户输入以停止服务器
        new Scanner(System.in).nextLine();
        System.out.println("正在停止服务器...");
        server.stop();
    }
    
    private static void runPerformanceTest() {
        System.out.println("开始性能对比测试...");
        ServerPerformanceTest test = new ServerPerformanceTest(DEFAULT_PORT);
        test.runTests();
    }
}