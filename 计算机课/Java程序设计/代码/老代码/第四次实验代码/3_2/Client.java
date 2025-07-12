

import java.awt.BorderLayout;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class Client {
    private static final String SERVER_IP = "localhost";
    private static final int SERVER_PORT = 12345;
    private DatagramSocket socket;
    private String name;
    private JFrame frame;
    private JTextField textField;
    private JTextArea messageArea;
    
    public Client(String name) {
        this.name = name;
        
        // 创建GUI
        frame = new JFrame("UDP聊天客户端 - " + name);
        textField = new JTextField(40);
        messageArea = new JTextArea(8, 40);
        
        textField.setEditable(true);
        messageArea.setEditable(false);
        frame.getContentPane().add(textField, BorderLayout.SOUTH);
        frame.getContentPane().add(new JScrollPane(messageArea), BorderLayout.CENTER);
        
        textField.addActionListener(e -> sendMessage(textField.getText()));
        
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }
    
    private void sendMessage(String message) {
        try {
            byte[] sendData = message.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(
                sendData,
                sendData.length,
                InetAddress.getByName(SERVER_IP),
                SERVER_PORT
            );
            socket.send(sendPacket);
            textField.setText("");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    private void run() throws IOException {
        socket = new DatagramSocket();
        
        // 发送连接消息
        String connectMessage = "CONNECT:" + name;
        byte[] connectData = connectMessage.getBytes();
        DatagramPacket connectPacket = new DatagramPacket(
            connectData,
            connectData.length,
            InetAddress.getByName(SERVER_IP),
            SERVER_PORT
        );
        socket.send(connectPacket);
        
        // 启动接收线程
        new Thread(() -> {
            try {
                byte[] receiveData = new byte[1024];
                while (true) {
                    DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    socket.receive(receivePacket);
                    String message = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    messageArea.append(message + "\n");
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }
    
    public static void main(String[] args) {
        String clientName = args[0];
        Client client = new Client(clientName);
        try {
            client.run();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
