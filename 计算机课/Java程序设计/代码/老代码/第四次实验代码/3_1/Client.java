

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class Client {
    // 定义一个BufferedReader对象，用于读取输入
    private BufferedReader in;
    // 定义一个PrintWriter对象，用于输出
    private PrintWriter out;
    // 定义一个String对象，用于存储用户名
    private String name;
    // 定义一个JFrame对象，用于创建聊天客户端窗口
    private JFrame frame = new JFrame("聊天客户端");
    // 定义一个JTextField对象，用于输入消息
    private JTextField textField = new JTextField(40);
    // 定义一个JTextArea对象，用于显示消息
    private JTextArea messageArea = new JTextArea(8, 40);

    public Client(String name) {
        // 构造函数，初始化客户端
        this.name = name;
        
        // 设置文本框可编辑
        textField.setEditable(true);
        // 设置消息区域不可编辑
        messageArea.setEditable(false);
        // 将文本框添加到窗口的南部
        frame.getContentPane().add(textField, BorderLayout.SOUTH);
        // 将消息区域添加到窗口的中央，并添加滚动条
        frame.getContentPane().add(new JScrollPane(messageArea), BorderLayout.CENTER);

        // 为文本框添加监听器，当按下回车键时，将文本框中的内容发送出去，并清空文本框
        textField.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                out.println(textField.getText());
                textField.setText("");
            }
        });
    }

    // 运行方法，抛出IOException异常
    private void run() throws IOException {
        try {
            // 创建Socket对象，连接到本地主机的12345端口
            Socket socket = new Socket("localhost", 12345);
            // 创建BufferedReader对象，读取Socket的输入流
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            // 创建PrintWriter对象，写入Socket的输出流
            out = new PrintWriter(socket.getOutputStream(), true);

            // 向服务器发送用户名
            out.println(name);

            // 无限循环，读取服务器发送的消息
            while (true) {
                // 读取一行消息
                String line = in.readLine();
                // 如果消息为空，则退出循环
                if (line == null) {
                    return;
                }
                // 将消息添加到消息区域
                messageArea.append(line + "\n");
            }
        } finally {
            // 关闭窗口
            frame.setVisible(false);
            frame.dispose();
        }
    }

   // 创建并显示GUI，传入客户端名称
    public static void createAndShowGUI(final String clientName) {
        // 创建客户端对象，传入客户端名称
        Client client = new Client(clientName);
        // 设置窗口关闭时的默认操作
        client.frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // 调整窗口大小以适应其子组件的首选大小
        client.frame.pack();
        // 设置窗口可见
        client.frame.setVisible(true);
        // 运行客户端
        try {
            client.run();
        } catch (IOException e) {
            // 打印异常堆栈信息
            e.printStackTrace();
        }
    }
}
