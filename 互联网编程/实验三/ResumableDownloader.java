import java.io.*;
import java.net.*;
import java.text.DecimalFormat;
import java.util.Scanner;


public class ResumableDownloader {
    
    private static final int BUFFER_SIZE = 8192; // 缓冲区大小
    private static final DecimalFormat df = new DecimalFormat("#.##");
    

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
        if (responseCode == HttpURLConnection.HTTP_PARTIAL || 
            (responseCode == HttpURLConnection.HTTP_OK && existingFileSize == 0)) {
            
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
            downloadEntireFile(connection, savePath, file.getName());
        } else {
            throw new IOException("服务器返回错误码: " + responseCode);
        }
    }
    
    //备用下载
    private static void downloadEntireFile(HttpURLConnection connection, String savePath, String fileName) 
            throws IOException {
        
        int fileSize = connection.getContentLength();
        if (fileSize > 0) {
            System.out.println("文件大小: " + formatFileSize(fileSize));
        }
        
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
    
    //大小
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
    
    //文件名
    private static String extractFileNameFromUrl(String urlString) {
        try {
            URL url = new URL(urlString);
            String path = url.getPath();
            String fileName = path.substring(path.lastIndexOf('/') + 1);
            
            // 如果文件名为空或只包含查询参数，使用默认名称
            if (fileName == null || fileName.trim().isEmpty() || !fileName.contains(".")) {
                fileName = "Chino_file";
            }
            
            return fileName;
        } catch (Exception e) {
            return "downloaded_file_" + System.currentTimeMillis();
        }
    }


    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("===== 断点续传下载工具 =====");
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
                resumableDownload(url, path);
            } catch (IOException e) {
                System.out.println("下载失败: " + e.getMessage());
                e.printStackTrace();
            }
        }
        
        scanner.close();
    }
}