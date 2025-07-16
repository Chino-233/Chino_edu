

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.HashSet;
import java.util.Set;

public class ServerS {
    private static final int PORT = 12345;
    private static Set<ClientInfo> clients = new HashSet<>();
    
    static class ClientInfo {
        InetAddress address;
        int port;
        String name;
        
        public ClientInfo(InetAddress address, int port, String name) {
            this.address = address;
            this.port = port;
            this.name = name;
        }
        
        @Override
        public boolean equals(Object obj) {
            if (obj instanceof ClientInfo) {
                ClientInfo other = (ClientInfo) obj;
                return address.equals(other.address) && port == other.port;
            }
            return false;
        }
        
        @Override
        public int hashCode() {
            return address.hashCode() + port;
        }
    }
    
    public static void main(String[] args) throws IOException {
        DatagramSocket socket = new DatagramSocket(PORT);
        System.out.println("UDP服务器已启动...");
        
        byte[] receiveData = new byte[1024];
        
        while (true) {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            socket.receive(receivePacket);
            
            String message = new String(receivePacket.getData(), 0, receivePacket.getLength());
            InetAddress clientAddress = receivePacket.getAddress();
            int clientPort = receivePacket.getPort();
            
            // 处理新客户端连接
            if (message.startsWith("CONNECT:")) {
                String clientName = message.substring(8);
                clients.add(new ClientInfo(clientAddress, clientPort, clientName));
                System.out.println(clientName + " 已连接");
                continue;
            }
            
            // 转发消息给其他客户端
            for (ClientInfo client : clients) {
                if (client.port != clientPort || !client.address.equals(clientAddress)) {
                    String senderName = "";
                    for (ClientInfo c : clients) {
                        if (c.port == clientPort && c.address.equals(clientAddress)) {
                            senderName = c.name;
                            break;
                        }
                    }
                    
                    String forwardMessage = senderName + ": " + message;
                    byte[] sendData = forwardMessage.getBytes();
                    DatagramPacket sendPacket = new DatagramPacket(
                        sendData,
                        sendData.length,
                        client.address,
                        client.port
                    );
                    socket.send(sendPacket);
                }
            }
        }
    }
}
