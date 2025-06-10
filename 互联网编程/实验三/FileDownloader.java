import java.io.*;
import java.net.*;
import java.util.Scanner;
import java.text.DecimalFormat;


public class FileDownloader {
    
    private static final int BUFFER_SIZE = 8192; // 缓冲区大小
    private static final DecimalFormat df = new DecimalFormat("#.##");
    

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
    
    //进度条
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
    

    private static String extractFileNameFromUrl(String urlString) {
        try {
            URL url = new URL(urlString);
            String path = url.getPath();
            String fileName = path.substring(path.lastIndexOf('/') + 1);
            

            if (fileName == null || fileName.trim().isEmpty() || !fileName.contains(".")) {
                fileName = "Chino_file";
            }
            
            return fileName;
        } catch (Exception e) {
            return "Chino_file_" + System.currentTimeMillis();
        }
    }


    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("===== 文件下载工具 =====");
        System.out.println("输入'exit'退出程序");
        
        while (true) {
            System.out.print("\n请输入URL(或输入exit退出): ");
            String url = scanner.nextLine().trim();
            
            if (url.equalsIgnoreCase("exit")) {
                System.out.println("程序已退出");
                break;
            }
            
            if (url.isEmpty()) {
                System.out.println("URL不能为空，请重新输入");
                continue;
            }
            
            System.out.print("请输入保存路径(留空则保存到默认位置): ");
            String path = scanner.nextLine().trim();
            
            // 如果路径为空，使用默认路径
            if (path.isEmpty()) {
                String fileName = extractFileNameFromUrl(url);
                path = System.getProperty("user.home") + File.separator + "Chino" +
                        File.separator + fileName;
                System.out.println("使用默认保存路径: " + path);
            }
            
            try {
                downloadFile(url, path);
            } catch (IOException e) {
                System.out.println("下载失败: " + e.getMessage());
            }
        }
        
        scanner.close();
    }
}