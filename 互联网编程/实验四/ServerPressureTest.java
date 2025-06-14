import java.net.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * HTTP服务器压力测试工具
 * 用于测试服务器在高并发下的性能
 */
public class ServerPressureTest {
    private final String serverAddress;
    private final int port;
    private final int totalClients;
    private final int requestsPerClient;
    private final AtomicInteger successCount = new AtomicInteger(0);
    private final AtomicInteger failureCount = new AtomicInteger(0);

    public ServerPressureTest(String serverAddress, int port, int totalClients, int requestsPerClient) {
        this.serverAddress = serverAddress;
        this.port = port;
        this.totalClients = totalClients;
        this.requestsPerClient = requestsPerClient;
    }

    public void runTest() throws InterruptedException {
        System.out.println("开始压力测试 - " + totalClients + " 客户端, 每个发送 " +
                requestsPerClient + " 请求");

        ExecutorService executor = Executors.newFixedThreadPool(totalClients);
        CountDownLatch latch = new CountDownLatch(totalClients);

        long startTime = System.currentTimeMillis();

        // 创建并启动所有客户端线程
        for (int i = 0; i < totalClients; i++) {
            executor.submit(() -> {
                try {
                    for (int j = 0; j < requestsPerClient; j++) {
                        if (sendRequest()) {
                            successCount.incrementAndGet();
                        } else {
                            failureCount.incrementAndGet();
                        }
                    }
                } finally {
                    latch.countDown();
                }
            });
        }

        // 等待所有客户端完成
        latch.await();
        long endTime = System.currentTimeMillis();

        // 生成测试报告
        generateReport(startTime, endTime);

        executor.shutdown();
    }

    private boolean sendRequest() {
        try {
            URL url = new URL("http://" + serverAddress + ":" + port + "/");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.setConnectTimeout(5000);
            connection.setReadTimeout(5000);

            int responseCode = connection.getResponseCode();
            connection.disconnect();

            return (responseCode == HttpURLConnection.HTTP_OK);
        } catch (Exception e) {
            return false;
        }
    }

    private void generateReport(long startTime, long endTime) {
        long duration = endTime - startTime;
        int totalRequests = totalClients * requestsPerClient;
        int successful = successCount.get();
        int failed = failureCount.get();

        double requestsPerSecond = (double) totalRequests / (duration / 1000.0);

        System.out.println("\n===== 压力测试报告 =====");
        System.out.println("测试时间: " + (duration / 1000.0) + " 秒");
        System.out.println("并发客户端数: " + totalClients);
        System.out.println("每客户端请求数: " + requestsPerClient);
        System.out.println("总请求数: " + totalRequests);
        System.out.println("成功请求: " + successful);
        System.out.println("失败请求: " + failed);
        System.out.println("每秒请求数 (QPS): " + String.format("%.2f", requestsPerSecond));
        System.out.println("平均响应时间: " + String.format("%.2f", (duration / (double) totalRequests)) + " ms");
    }

    public static void main(String[] args) {
        try {
            // 测试参数
            String server = "localhost";
            int port = 8080;

            System.out.println("HTTP服务器压力测试工具");
            System.out.println("===========================");

            // 测试不同级别的并发
            System.out.println("\n测试场景1: 低并发");
            new ServerPressureTest(server, port, 10, 10).runTest();

            System.out.println("\n测试场景2: 中等并发");
            new ServerPressureTest(server, port, 50, 20).runTest();

            System.out.println("\n测试场景3: 高并发");
            new ServerPressureTest(server, port, 100, 50).runTest();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}