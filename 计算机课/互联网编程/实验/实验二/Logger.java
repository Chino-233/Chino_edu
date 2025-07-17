package 互联网编程.实验二;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Logger {
    private final String logFilePath;
    private static Logger instance;
    private final SimpleDateFormat dateFormat;
    
    // 单例模式，确保只有一个Logger实例
    private Logger(String logFilePath) {
        this.logFilePath = logFilePath;
        this.dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
    }
    
    public static synchronized Logger getInstance(String logFilePath) {
        if (instance == null) {
            instance = new Logger(logFilePath);
        }
        return instance;
    }
    
    /**
     * 记录信息到日志文件
     * 使用synchronized确保线程安全
     * @param message 要记录的信息
     */
    public synchronized void log(String message) {
        try (FileWriter fw = new FileWriter(logFilePath, true);
             PrintWriter writer = new PrintWriter(fw)) {
            
            String timestamp = dateFormat.format(new Date());
            String threadInfo = Thread.currentThread().getName();
            String logEntry = String.format("[%s] [%s] %s", timestamp, threadInfo, message);
            
            writer.println(logEntry);
            System.out.println(logEntry);
            
        } catch (IOException e) {
            System.err.println("记录日志失败: " + e.getMessage());
        }
    }
    
    /**
     * 记录客户端连接信息
     * @param clientIp 客户端IP
     * @param port 客户端端口
     */
    public void logClientConnected(String clientIp, int port) {
        log("新客户端连接: " + clientIp + ":" + port);
    }
    
    /**
     * 记录客户端断开连接信息
     * @param clientIp 客户端IP
     * @param port 客户端端口
     */
    public void logClientDisconnected(String clientIp, int port) {
        log("客户端断开连接: " + clientIp + ":" + port);
    }
    
    /**
     * 记录异常信息
     * @param e 异常对象
     */
    public void logException(Exception e) {
        log("异常: " + e.getClass().getName() + " - " + e.getMessage());
    }
}