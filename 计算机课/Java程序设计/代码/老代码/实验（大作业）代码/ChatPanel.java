package com.chino;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ChatPanel extends JPanel {
    private String model; // 模型名称
    private JPanel chatArea; // 聊天显示区域
    private JTextField inputField; // 输入框
    private PrintWriter out; // 输出流，用于发送数据到服务器
    private BufferedReader in; // 输入流，用于接收服务器数据
    private ExecutorService executorService = Executors.newFixedThreadPool(10); // 线程池，用于处理并发任务
    private Timer typingTimer; // 定时器，用于显示逐渐变化的 "..."

    public ChatPanel(String model) {
        this.model = model; // 初始化模型名称
        setLayout(new BorderLayout()); // 设置布局管理器为边界布局

        chatArea = new JPanel(); // 创建聊天显示区域
        chatArea.setLayout(new BoxLayout(chatArea, BoxLayout.Y_AXIS)); // 设置聊天显示区域布局为垂直BoxLayout
        JScrollPane scrollPane = new JScrollPane(chatArea); // 创建滚动面板并包含聊天显示区域
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED); // 设置垂直滚动条策略为需要时显示
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER); // 禁用水平滚动条
        scrollPane.setPreferredSize(new Dimension(800, 600)); // 设置滚动面板的首选大小
        add(scrollPane, BorderLayout.CENTER); // 将滚动面板添加到面板中央

        inputField = new JTextField(); // 创建输入框
        inputField.setPreferredSize(new Dimension(0, 50)); // 调整文本输入框高度
        inputField.addActionListener(new ActionListener() { // 为输入框添加动作监听器
            @Override
            public void actionPerformed(ActionEvent e) {
                String input = inputField.getText(); // 获取输入框中的文本
                inputField.setText(""); // 清空输入框
                addMessage("你: " + input, true); // 将用户输入显示在聊天区域
                executorService.submit(() -> handleUserInput(input)); // 提交任务到线程池处理用户输入
            }
        });
        add(inputField, BorderLayout.SOUTH); // 将输入框添加到面板底部

        initializeConnection(); // 初始化连接
    }

    private void initializeConnection() {
        try {
            Socket socket = new Socket("localhost", 12345); // 创建与服务器的套接字连接
            in = new BufferedReader(new InputStreamReader(socket.getInputStream())); // 初始化输入流
            out = new PrintWriter(socket.getOutputStream(), true); // 初始化输出流
            out.println(model); // 发送模型名称到服务器
        } catch (IOException e) {
            e.printStackTrace(); // 捕获并打印异常
        }
    }

    private void handleUserInput(String input) {
        out.println(input); // 将用户输入发送到服务器
        startTypingIndicator(); // 开始显示逐渐变化的 "..."
        try {
            StringBuilder response = new StringBuilder();
            String responseLine = "";
            while ((responseLine = in.readLine()) != null) { // 读取服务器响应
                if ("END".equals(responseLine)) { // 检查是否为结束标志
                    break;
                }
                if (!responseLine.equals("")) {
                    response.append(responseLine + "\n"); // 将服务器响应显示在聊天区域
                }
            }
            stopTypingIndicator(); // 停止显示逐渐变化的 "..."
            addMessage("AI: " + response.toString(), false); // 将服务器响应显示在聊天区域
            logConversation("你: " + input + "\nAI: " + response + "\n"); // 记录对话日志
        } catch (IOException e) {
            e.printStackTrace(); // 捕获并打印异常
        }
    }

    // 添加消息方法
    private void addMessage(String message, boolean isUser) {
        // 创建一个文本区域，用于显示消息
        JTextArea messageArea = new JTextArea(message);
        // 设置文本区域自动换行
        messageArea.setLineWrap(true);
        // 设置文本区域按单词换行
        messageArea.setWrapStyleWord(true);
        // 设置文本区域不可编辑
        messageArea.setEditable(false);
        // 设置文本区域背景颜色
        messageArea.setBackground(Color.decode("#8ab7eb"));
        // 设置文本区域前景颜色
        messageArea.setForeground(Color.WHITE);
        // 设置文本区域边框
        messageArea.setBorder(BorderFactory.createCompoundBorder(
                // 设置文本区域边框为黑色实线
                BorderFactory.createLineBorder(Color.BLACK),
                // 设置文本区域边框内边距
                BorderFactory.createEmptyBorder(10, 10, 10, 10)
        ));
        // 设置文本区域最大宽度
        messageArea.setMaximumSize(new Dimension(chatArea.getWidth(), Integer.MAX_VALUE)); // 设置最大宽度
        // 创建一个面板，用于放置文本区域
        JPanel messagePanel = new JPanel(new BorderLayout());
        // 根据是否为用户消息，将文本区域放置在面板的不同位置
        messagePanel.add(messageArea, isUser ? BorderLayout.EAST : BorderLayout.WEST);
        // 设置面板边框内边距
        messagePanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        // 将面板添加到聊天区域
        chatArea.add(messagePanel);
        // 重新验证和绘制聊天区域
        chatArea.revalidate();
        chatArea.repaint();
    }

    private void startTypingIndicator() {
        // 创建一个JTextArea，用于显示等待AI回应的文本
        JTextArea typingIndicator = new JTextArea("等待AI回应.");
        // 设置自动换行
        typingIndicator.setLineWrap(true);
        // 设置换行方式为单词换行
        typingIndicator.setWrapStyleWord(true);
        // 设置不可编辑
        typingIndicator.setEditable(false);
        // 设置背景颜色
        typingIndicator.setBackground(Color.decode("#8ab7eb"));
        // 设置前景颜色
        typingIndicator.setForeground(Color.WHITE);
        // 设置边框
        typingIndicator.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.BLACK),
                BorderFactory.createEmptyBorder(10, 10, 10, 10)
        ));
        // 设置typingIndicator的边框，包括黑色线条和10像素的内边距
        typingIndicator.setMaximumSize(new Dimension(chatArea.getWidth() * 2 / 3, Integer.MAX_VALUE)); // 设置最大宽度

        // 创建一个JPanel，用于放置typingIndicator
        JPanel typingPanel = new JPanel(new BorderLayout());
        typingPanel.add(typingIndicator, BorderLayout.WEST);
        typingPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        // 将typingPanel添加到chatArea中，并重新验证和重绘chatArea
        chatArea.add(typingPanel);
        chatArea.revalidate();
        chatArea.repaint();

        // 创建一个定时器，每500毫秒执行一次ActionListener中的方法
        typingTimer = new Timer(500, new ActionListener() {
            private int dotCount = 1;

            @Override
            public void actionPerformed(ActionEvent e) {
                // 根据dotCount的值，在typingIndicator中显示不同数量的点
                String dots = ".".repeat(dotCount);
                typingIndicator.setText("等待AI回应" + dots);
                dotCount = (dotCount % 3) + 1;
            }
        });
        // 启动定时器
        typingTimer.start();
    }

    // 停止输入指示器
    private void stopTypingIndicator() {
        // 如果输入计时器不为空
        if (typingTimer != null) {
            // 停止输入计时器
            typingTimer.stop();
            // 将输入计时器置为空
            typingTimer = null;
        }
        chatArea.remove(chatArea.getComponentCount() - 1); // 移除最后一个等待消息
        chatArea.revalidate();
        chatArea.repaint();
    }

    // 记录对话日志
    private void logConversation(String log) {
        // 获取当前时间戳
        String timeStamp = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
        // 分隔符
        String separator = "-----------------------------";
        try (FileWriter fw = new FileWriter("conversation_log.txt", true); // 创建一个FileWriter对象，用于写入文件
             BufferedWriter bw = new BufferedWriter(fw); // 创建一个BufferedWriter对象，用于缓冲写入
             PrintWriter out = new PrintWriter(bw)) { // 创建一个PrintWriter对象，用于写入字符串
            out.println(separator); // 写入分隔符
            out.println("对话模型: " + model); // 写入对话模型
            out.println("对话时间: " + timeStamp); // 写入对话时间
            out.println(log); // 将对话日志写入文件
            out.println(separator); // 写入分隔符
        } catch (IOException e) { // 捕获IOException异常
            e.printStackTrace(); // 捕获并打印异常
        }
    }

    // 设置新的模型名称
    public void setModel(String model) {
        this.model = model; // 设置新的模型名称
        initializeConnection(); // 重新初始化连接以使用新的模型
    }

    // 获取当前模型名称
    public String getModel() {
        return model; // 获取当前模型名称
    }
}
