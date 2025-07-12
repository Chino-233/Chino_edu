import java.io.*;
import java.net.*;
import java.util.Scanner;
import java.text.DecimalFormat;

/**
 * 网络工具类 - 包含域名解析、文件下载和断点续传功能
 */
public class NetworkUtilities {
    
    private static final int BUFFER_SIZE = 8192; // 缓冲区大小
    private static final DecimalFormat df = new DecimalFormat("#.##");
    
    /**
     * 域名解析 - 将域名解析为IP地址
     * @param domain 要解析的域名
     * @return 域名对应的IP地址数组
     */
    public static String[] resolveDomain(String domain) throws UnknownHostException {
        System.out.println("正在解析域名: " + domain);
        InetAddress[] addresses = InetAddress.getAllByName(domain);
        String[] ipAddresses = new String[addresses.length];
        
        for (int i = 0; i < addresses.length; i++) {
            ipAddresses[i] = addresses[i].getHostAddress();
        }
        return ipAddresses;
    }
    //https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user
    //D:\Chino_edu\互联网编程\实验三\out\Chino1.exe
    /**
     * 基本文件下载功能
     * @param urlString 要下载的URL地址
     * @param savePath 保存文件的路径
     */
    public static void downloadFile(String urlString, String savePath) throws IOException {
        URL url = new URL(urlString);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        connection.connect();
        
        // 检查HTTP响应码
        int responseCode = connection.getResponseCode();
        if (responseCode != HttpURLConnection.HTTP_OK) {
            throw new IOException("服务器返回错误码: " + responseCode);
        }
        
        // 获取文件大小
        int fileSize = connection.getContentLength();
        if (fileSize < 0) {
            System.out.println("无法获取文件大小");
        } else {
            System.out.println("文件大小: " + formatFileSize(fileSize));
        }
        
        // 创建目录（如果不存在）
        File saveFile = new File(savePath);
        File parentDir = saveFile.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            parentDir.mkdirs();
        }
        
        // 获取文件名
        String fileName = saveFile.getName();
        
        try (InputStream in = connection.getInputStream();
             FileOutputStream out = new FileOutputStream(savePath)) {
            
            byte[] buffer = new byte[BUFFER_SIZE];
            int bytesRead;
            int totalBytesRead = 0;
            long startTime = System.currentTimeMillis();
            long lastStatTime = startTime;
            int lastStat = 0;
            
            System.out.println("开始下载文件: " + fileName);
            
            while ((bytesRead = in.read(buffer)) != -1) {
                out.write(buffer, 0, bytesRead);
                totalBytesRead += bytesRead;
                
                // 每半秒更新一次状态
                long currentTime = System.currentTimeMillis();
                if (currentTime - lastStatTime >= 500) {
                    // 计算下载速度（KB/s）
                    double speed = (totalBytesRead - lastStat) * 1000.0 / (currentTime - lastStatTime) / 1024;
                    lastStat = totalBytesRead;
                    lastStatTime = currentTime;
                    
                    // 显示下载进度
                    if (fileSize > 0) {
                        int percentCompleted = (int) (totalBytesRead * 100L / fileSize);
                        System.out.printf("\r下载进度: %d%% [%s] %.2f KB/s", 
                            percentCompleted, 
                            getProgressBar(percentCompleted), 
                            speed);
                    } else {
                        System.out.printf("\r已下载: %s, 速度: %.2f KB/s", 
                            formatFileSize(totalBytesRead), 
                            speed);
                    }
                }
            }
            
            long endTime = System.currentTimeMillis();
            double totalTime = (endTime - startTime) / 1000.0;
            System.out.printf("\n下载完成! 总大小: %s, 用时: %.1f 秒, 平均速度: %.2f KB/s\n", 
                formatFileSize(totalBytesRead),
                totalTime,
                (totalBytesRead / 1024.0 / totalTime));
        }
    }
    
    /**
     * 断点续传下载功能
     * @param urlString 要下载的URL地址
     * @param savePath 保存文件的路径
     */
    public static void resumableDownload(String urlString, String savePath) throws IOException {
        URL url = new URL(urlString);
        File file = new File(savePath);
        long existingFileSize = 0;
        
        // 检查文件是否已存在
        if (file.exists() && file.isFile()) {
            existingFileSize = file.length();
            System.out.println("发现现有文件，大小: " + formatFileSize(existingFileSize));
        } else {
            // 创建目录（如果不存在）
            File parentDir = file.getParentFile();
            if (parentDir != null && !parentDir.exists()) {
                parentDir.mkdirs();
            }
        }
        
        // 先获取文件总大小
        HttpURLConnection infoConnection = (HttpURLConnection) url.openConnection();
        infoConnection.setRequestMethod("HEAD");
        int fileSize = infoConnection.getContentLength();
        infoConnection.disconnect();
        
        if (fileSize > 0 && existingFileSize >= fileSize) {
            System.out.println("文件已完全下载，无需继续");
            return;
        }
        
        // 获取文件名
        String fileName = file.getName();
        
        // 准备进行断点续传
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        
        // 设置断点续传的 Range 头
        if (existingFileSize > 0) {
            connection.setRequestProperty("Range", "bytes=" + existingFileSize + "-");
            System.out.println("设置断点续传位置: " + formatFileSize(existingFileSize));
        }
        
        connection.connect();
        int responseCode = connection.getResponseCode();
        
        // 检查服务器响应
        if (responseCode == HttpURLConnection.HTTP_PARTIAL || (responseCode == HttpURLConnection.HTTP_OK && existingFileSize == 0)) {
            try (InputStream in = connection.getInputStream();
                 RandomAccessFile out = new RandomAccessFile(file, "rw")) {
                
                if (existingFileSize > 0) {
                    out.seek(existingFileSize);
                }
                
                byte[] buffer = new byte[BUFFER_SIZE];
                int bytesRead;
                long totalBytesRead = existingFileSize;
                long startTime = System.currentTimeMillis();
                long lastStatTime = startTime;
                long lastStat = existingFileSize;
                
                System.out.println("开始" + (existingFileSize > 0 ? "续传" : "下载") + "文件: " + fileName);
                
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                    totalBytesRead += bytesRead;
                    
                    // 每半秒更新一次状态
                    long currentTime = System.currentTimeMillis();
                    if (currentTime - lastStatTime >= 500) {
                        // 计算下载速度（KB/s）
                        double speed = (totalBytesRead - lastStat) * 1000.0 / (currentTime - lastStatTime) / 1024;
                        lastStat = totalBytesRead;
                        lastStatTime = currentTime;
                        
                        // 显示下载进度
                        if (fileSize > 0) {
                            int percentCompleted = (int) (totalBytesRead * 100L / fileSize);
                            System.out.printf("\r下载进度: %d%% [%s] %.2f KB/s", 
                                percentCompleted, 
                                getProgressBar(percentCompleted), 
                                speed);
                        } else {
                            System.out.printf("\r已下载: %s, 速度: %.2f KB/s", 
                                formatFileSize(totalBytesRead), 
                                speed);
                        }
                    }
                }
                
                long endTime = System.currentTimeMillis();
                double totalTime = (endTime - startTime) / 1000.0;
                System.out.printf("\n下载完成! 总大小: %s, 本次用时: %.1f 秒, 本次平均速度: %.2f KB/s\n", 
                    formatFileSize(totalBytesRead),
                    totalTime,
                    ((totalBytesRead - existingFileSize) / 1024.0 / totalTime));
            }
        } else if (responseCode == HttpURLConnection.HTTP_OK) {
            System.out.println("服务器不支持断点续传，将重新下载整个文件");
            downloadFile(urlString, savePath);
        } else {
            throw new IOException("服务器返回错误码: " + responseCode);
        }
    }
    
    //文件大小
    private static String formatFileSize(long size) {
        if (size < 1024) {
            return size + " B";
        } else if (size < 1024 * 1024) {
            return df.format(size / 1024.0) + " KB";
        } else if (size < 1024 * 1024 * 1024) {
            return df.format(size / (1024.0 * 1024)) + " MB";
        } else {
            return df.format(size / (1024.0 * 1024 * 1024)) + " GB";
        }
    }
    
    /**
     * 生成进度条字符串
     */
    private static String getProgressBar(int percent) {
        int width = 20; // 进度条宽度
        int completed = width * percent / 100;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < completed; i++) {
            sb.append('=');
        }
        for (int i = completed; i < width; i++) {
            sb.append(' ');
        }
        return sb.toString();
    }

    /**
     * 主测试方法
     */
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        while (true) {
            System.out.println("\n===== 网络工具菜单 =====");
            System.out.println("1. 域名解析");
            System.out.println("2. 文件下载");
            System.out.println("3. 断点续传下载");
            System.out.println("0. 退出");
            System.out.print("请选择功能: ");
            
            int choice;
            try {
                choice = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("输入无效，请重新选择");
                continue;
            }
            
            switch (choice) {
                case 1:
                    System.out.print("请输入要解析的域名: ");
                    String domain = scanner.nextLine();
                    try {
                        String[] ipAddresses = resolveDomain(domain);
                        System.out.println("域名 " + domain + " 解析结果:");
                        for (String ip : ipAddresses) {
                            System.out.println("  - " + ip);
                        }
                    } catch (UnknownHostException e) {
                        System.out.println("无法解析域名: " + e.getMessage());
                    }
                    break;
                
                case 2:
                    System.out.print("请输入URL: ");
                    String url = scanner.nextLine();
                    System.out.print("请输入保存路径: ");
                    String path = scanner.nextLine();
                    try {
                        downloadFile(url, path);
                    } catch (IOException e) {
                        System.out.println("下载失败: " + e.getMessage());
                        e.printStackTrace();
                    }
                    break;
                
                case 3:
                    System.out.print("请输入URL: ");
                    String resumableUrl = scanner.nextLine();
                    System.out.print("请输入保存路径: ");
                    String resumablePath = scanner.nextLine();
                    try {
                        resumableDownload(resumableUrl, resumablePath);
                    } catch (IOException e) {
                        System.out.println("下载失败: " + e.getMessage());
                        e.printStackTrace();
                    }
                    break;
                
                case 0:
                    System.out.println("程序已退出");
                    scanner.close();
                    return;
                
                default:
                    System.out.println("无效的选择，请重试");
            }
        }
    }
}