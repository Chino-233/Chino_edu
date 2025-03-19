import java.io.*;
import java.net.*;

public class client {
    private static final String SERVER_IP = "127.0.0.1";
    private static final int SERVER_PORT = 8888;
    private static final String SAVE_DIRECTORY = "D:\\chino_edu\\互联网编程\\实验一\\received\\";
    
    public static void main(String[] args) {
        // 确保保存目录存在
        File saveDir = new File(SAVE_DIRECTORY);
        if (!saveDir.exists()) {
            saveDir.mkdirs();
        }
        
        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT)) {
            System.out.println("已连接到服务器: " + SERVER_IP + ":" + SERVER_PORT);
            
            // 获取输入流
            DataInputStream dataIn = new DataInputStream(socket.getInputStream());
            
            // 接收要接收的文件数量
            int fileCount = dataIn.readInt();
            System.out.println("将接收 " + fileCount + " 个文件");
            
            // 循环接收每个文件
            for (int i = 0; i < fileCount; i++) {
                // 接收文件名
                String fileName = dataIn.readUTF();
                
                // 空文件名表示服务器跳过了这个文件
                if (fileName.isEmpty()) {
                    System.out.println("服务器跳过了一个文件");
                    continue;
                }
                
                // 接收文件大小
                long fileSize = dataIn.readLong();
                
                System.out.println("开始接收文件: " + fileName + " (" + fileSize + " 字节)");
                
                // 创建文件输出流
                String savePath = SAVE_DIRECTORY + fileName;
                try (FileOutputStream fileOut = new FileOutputStream(savePath);
                     BufferedOutputStream bufferedOut = new BufferedOutputStream(fileOut)) {
                    
                    byte[] buffer = new byte[4096];
                    int bytesRead;
                    long totalReceived = 0;
                    
                    // 接收并写入文件内容
                    while (totalReceived < fileSize) {
                        bytesRead = dataIn.read(buffer, 0, (int) Math.min(buffer.length, fileSize - totalReceived));
                        if (bytesRead == -1) {
                            break;
                        }
                        
                        bufferedOut.write(buffer, 0, bytesRead);
                        totalReceived += bytesRead;
                        
                        // 打印进度
                        if (fileSize > 0) {
                            int progress = (int) ((totalReceived * 100) / fileSize);
                            System.out.print("\r接收进度: " + progress + "%");
                        }
                    }
                    
                    System.out.println("\n文件 " + fileName + " 接收完成，保存到: " + savePath);
                }
            }
            
            System.out.println("所有文件接收完成");
            
        } catch (IOException e) {
            System.err.println("客户端错误: " + e.getMessage());
            e.printStackTrace();
        }
    }
}