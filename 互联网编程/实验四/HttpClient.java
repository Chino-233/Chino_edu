import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.List;
import javax.swing.*;

/**
 * 简易HTTP客户端实现
 * 支持发送GET、HEAD和POST请求
 * 包含Cookie管理功能
 */
public class HttpClient extends JFrame {
    private static final long serialVersionUID = 1L;
    
    private final JTextField urlField;
    private final JTextArea requestArea;
    private final JTextArea responseArea;
    private final JComboBox<String> methodCombo;
    private final Map<String, String> cookies = new HashMap<>();
    
    public HttpClient() {
        setTitle("HTTP 客户端");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        // URL面板
        JPanel urlPanel = new JPanel(new BorderLayout());
        urlPanel.add(new JLabel("URL: "), BorderLayout.WEST);
        urlField = new JTextField("http://localhost:8080/");
        urlPanel.add(urlField, BorderLayout.CENTER);
        
        // 请求方法面板
        JPanel methodPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        methodCombo = new JComboBox<>(new String[]{"GET", "HEAD", "POST"});
        methodPanel.add(new JLabel("方法: "));
        methodPanel.add(methodCombo);
        
        JButton sendButton = new JButton("发送请求");
        methodPanel.add(sendButton);
        
        JButton viewCookiesButton = new JButton("查看Cookies");
        methodPanel.add(viewCookiesButton);
        
        // 请求与响应面板
        JPanel contentPanel = new JPanel(new GridLayout(2, 1));
        
        JPanel requestPanel = new JPanel(new BorderLayout());
        requestPanel.add(new JLabel("请求正文 (用于POST请求):"), BorderLayout.NORTH);
        requestArea = new JTextArea();
        requestPanel.add(new JScrollPane(requestArea), BorderLayout.CENTER);
        
        JPanel responsePanel = new JPanel(new BorderLayout());
        responsePanel.add(new JLabel("响应:"), BorderLayout.NORTH);
        responseArea = new JTextArea();
        responseArea.setEditable(false);
        responsePanel.add(new JScrollPane(responseArea), BorderLayout.CENTER);
        
        contentPanel.add(requestPanel);
        contentPanel.add(responsePanel);
        
        // 主面板
        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(urlPanel, BorderLayout.NORTH);
        mainPanel.add(methodPanel, BorderLayout.CENTER);
        mainPanel.add(contentPanel, BorderLayout.SOUTH);
        
        add(mainPanel);
        
        // 添加发送请求事件处理
        sendButton.addActionListener(e -> sendRequest());
        
        // 添加查看Cookies事件处理
        viewCookiesButton.addActionListener(e -> showCookies());
    }
    
    private void sendRequest() {
        String urlString = urlField.getText();
        String method = (String) methodCombo.getSelectedItem();
        String requestBody = requestArea.getText();
        
        try {
            URL url = new URL(urlString);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            
            // 设置请求方法
            connection.setRequestMethod(method);
            
            // 设置通用请求头
            connection.setRequestProperty("User-Agent", "Chino-HttpClient/1.0");
            connection.setRequestProperty("Accept", "*/*");
            
            // 添加所有Cookies
            if (!cookies.isEmpty()) {
                StringBuilder cookieString = new StringBuilder();
                for (Map.Entry<String, String> entry : cookies.entrySet()) {
                    if (cookieString.length() > 0) {
                        cookieString.append("; ");
                    }
                    cookieString.append(entry.getKey()).append("=").append(entry.getValue());
                }
                connection.setRequestProperty("Cookie", cookieString.toString());
            }
            
            // 如果是POST请求，添加请求体
            if ("POST".equalsIgnoreCase(method) && !requestBody.isEmpty()) {
                connection.setDoOutput(true);
                connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
                try (DataOutputStream writer = new DataOutputStream(connection.getOutputStream())) {
                    writer.writeBytes(requestBody);
                    writer.flush();
                }
            }
            
            // 获取响应
            StringBuilder response = new StringBuilder();
            response.append("状态码: ").append(connection.getResponseCode())
                   .append(" ").append(connection.getResponseMessage()).append("\n\n");
            
            // 获取响应头
            response.append("响应头:\n");
            for (Map.Entry<String, List<String>> header : connection.getHeaderFields().entrySet()) {
                if (header.getKey() != null) {
                    response.append(header.getKey()).append(": ");
                    for (String value : header.getValue()) {
                        response.append(value);
                        
                        // 处理Cookie
                        if (header.getKey().equalsIgnoreCase("Set-Cookie")) {
                            processCookie(value);
                        }
                    }
                    response.append("\n");
                }
            }
            response.append("\n");
            
            // 获取响应内容 (HEAD请求没有响应体)
            if (!"HEAD".equalsIgnoreCase(method)) {
                response.append("响应体:\n");
                try (BufferedReader reader = new BufferedReader(
                        new InputStreamReader(connection.getInputStream()))) {
                    String line;
                    while ((line = reader.readLine()) != null) {
                        response.append(line).append("\n");
                    }
                } catch (IOException e) {
                    // 处理错误响应
                    try (BufferedReader reader = new BufferedReader(
                            new InputStreamReader(connection.getErrorStream()))) {
                        String line;
                        while ((line = reader.readLine()) != null) {
                            response.append(line).append("\n");
                        }
                    }
                }
            }
            
            responseArea.setText(response.toString());
            
        } catch (IOException e) {
            responseArea.setText("发送请求时出错: " + e.getMessage());
        }
    }
    
    private void processCookie(String cookieHeader) {
        String[] parts = cookieHeader.split(";");
        if (parts.length > 0) {
            String[] nameValue = parts[0].split("=", 2);
            if (nameValue.length == 2) {
                String name = nameValue[0].trim();
                String value = nameValue[1].trim();
                cookies.put(name, value);
            }
        }
    }
    
    private void showCookies() {
        StringBuilder cookieInfo = new StringBuilder("当前Cookies:\n\n");
        
        if (cookies.isEmpty()) {
            cookieInfo.append("没有存储的Cookie");
        } else {
            for (Map.Entry<String, String> entry : cookies.entrySet()) {
                cookieInfo.append(entry.getKey()).append(" = ").append(entry.getValue()).append("\n");
            }
        }
        
        JOptionPane.showMessageDialog(this, cookieInfo.toString(), "Cookie信息", JOptionPane.INFORMATION_MESSAGE);
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            HttpClient client = new HttpClient();
            client.setVisible(true);
        });
    }
}