import java.net.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * HTTP服务器压力测试类
 * 用于测试服务器在高并发下的性能
 */
public class HttpServerStressTest {

    private final String serverUrl;
    private final int concurrentClients;
    private final int requestsPerClient;
    private final AtomicInteger successCount = new AtomicInteger(0);
    private final AtomicInteger failureCount = new AtomicInteger(0);

    public HttpServerStressTest(String serverUrl, int concurrentClients, int requestsPerClient) {
        this.serverUrl = serverUrl;
        this.concurrentClients = concurrentClients;
        this.requestsPerClient = requestsPerClient;
    }

    public void runTest() throws InterruptedException {
        System.out.println("开始压力测试 - 并发客户端数: " + concurrentClients +
                ", 每客户端请求数: " + requestsPerClient);

        ExecutorService executor = Executors.newFixedThreadPool(concurrentClients);
        CountDownLatch startSignal = new CountDownLatch(1);
        CountDownLatch doneSignal = new CountDownLatch(concurrentClients);

        // 创建并启动所有客户端线程
        for (int i = 0; i < concurrentClients; i++) {
            executor.submit(() -> {
                try {
                    startSignal.await(); // 等待统一开始

                    for (int j = 0; j < requestsPerClient; j++) {
                        try {
                            URL url = new URL(serverUrl);
                            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                            connection.setRequestMethod("GET");
                            connection.setConnectTimeout(5000);

                            int responseCode = connection.getResponseCode();
                            if (responseCode == HttpURLConnection.HTTP_OK) {
                                successCount.incrementAndGet();
                            } else {
                                failureCount.incrementAndGet();
                            }

                            connection.disconnect();
                        } catch (Exception e) {
                            failureCount.incrementAndGet();
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    doneSignal.countDown();
                }
            });
        }

        // 记录开始时间并启动测试
        long startTime = System.currentTimeMillis();
        startSignal.countDown(); // 所有线程同时开始

        // 等待所有客户端完成
        doneSignal.await();
        long endTime = System.currentTimeMillis();

        // 生成报告
        generateReport(startTime, endTime);

        executor.shutdown();
    }

    private void generateReport(long startTime, long endTime) {
        long duration = endTime - startTime;
        int totalRequests = concurrentClients * requestsPerClient;
        int successful = successCount.get();
        int failed = failureCount.get();

        double requestsPerSecond = (double) totalRequests / (duration / 1000.0);

        System.out.println("\n===== 压力测试报告 =====");
        System.out.println("测试时长: " + (duration / 1000.0) + " 秒");
        System.out.println("并发数: " + concurrentClients);
        System.out.println("总请求数: " + totalRequests);
        System.out.println("成功请求: " + successful + " (" + (successful * 100.0 / totalRequests) + "%)");
        System.out.println("失败请求: " + failed + " (" + (failed * 100.0 / totalRequests) + "%)");
        System.out.println("每秒请求数 (QPS): " + String.format("%.2f", requestsPerSecond));
        System.out.println("平均响应时间: " + String.format("%.2f", (duration * 1.0 / totalRequests)) + " ms");
    }

    public static void main(String[] args) {
        try {
            String serverUrl = "http://localhost:8080/";

            // 测试不同并发级别
            System.out.println("\n=== 场景1: 低并发测试 ===");
            new HttpServerStressTest(serverUrl, 10, 10).runTest();

            System.out.println("\n=== 场景2: 中等并发测试 ===");
            new HttpServerStressTest(serverUrl, 50, 20).runTest();

            System.out.println("\n=== 场景3: 高并发测试 ===");
            new HttpServerStressTest(serverUrl, 100, 50).runTest();

            // 测试不同资源加载性能
            System.out.println("\n=== 场景4: 静态HTML页面加载测试 ===");
            new HttpServerStressTest(serverUrl + "index.html", 20, 50).runTest();

            System.out.println("\n=== 场景5: 图片加载测试 ===");
            new HttpServerStressTest(serverUrl + "test.jpg", 20, 50).runTest();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}