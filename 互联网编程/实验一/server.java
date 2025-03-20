import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;

public class server {
    private static final int PORT = 8888;
    
    public static void main(String[] args) {
        List<String> filesToSend = new ArrayList<>();
        // 发送文件位置列表
        filesToSend.add("D:\\chino_edu\\互联网编程\\实验一\\testfiles\\chino.txt");
        filesToSend.add("D:\\chino_edu\\互联网编程\\实验一\\testfiles\\dream.txt");
        filesToSend.add("D:\\chino_edu\\互联网编程\\实验一\\testfiles\\love.txt");
        filesToSend.add("D:\\chino_edu\\互联网编程\\实验一\\testfiles\\1718881856589.jpg");
        
        
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("服务器启动，等待连接...");
            
            // 接受客户端连接
            try (Socket clientSocket = serverSocket.accept()) {
                System.out.println("客户端已连接: " + clientSocket.getInetAddress());
                
                DataOutputStream dataOut = new DataOutputStream(clientSocket.getOutputStream());
                
                // 获取发送要发送的文件数量
                dataOut.writeInt(filesToSend.size());
                
                // 发送文件
                for (String filePath : filesToSend) {
                    File file = new File(filePath);
                    if (!file.exists()) {
                        System.out.println("文件不存在: " + filePath);
                        // 发送空文件名表示跳过此文件
                        dataOut.writeUTF("");
                        continue;
                    }
                    
                    // 发送文件名
                    String fileName = file.getName();
                    dataOut.writeUTF(fileName);
                    
                    // 发送文件大小
                    long fileSize = file.length();
                    dataOut.writeLong(fileSize);
                    
                    System.out.println("开始发送文件: " + fileName + " (" + fileSize + " 字节)");
                    
                    // 发送文件内容
                    try (FileInputStream fileIn = new FileInputStream(file);
                         BufferedInputStream bufferedIn = new BufferedInputStream(fileIn)) {
                        
                        byte[] buffer = new byte[4096];
                        int bytesRead;
                        long totalSent = 0;
                        
                        while ((bytesRead = bufferedIn.read(buffer)) != -1) {
                            dataOut.write(buffer, 0, bytesRead);
                            totalSent += bytesRead;
                            
                            // 打印进度
                            if (fileSize > 0) {
                                int progress = (int) ((totalSent * 100) / fileSize);
                                System.out.print("\r发送进度: " + progress + "%");
                            }
                        }
                        
                        System.out.println("\n文件 " + fileName + " 发送完成");
                    }
                }
                
                System.out.println("所有文件发送完成");
            }
        } catch (IOException e) {
            System.err.println("服务器错误: " + e.getMessage());
            e.printStackTrace();
        }
    }
}