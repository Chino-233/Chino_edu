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
        setTitle("Chino的HTTP客户端");
        setSize(900, 700); // 增大窗口尺寸
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // URL面板
        JPanel urlPanel = new JPanel(new BorderLayout());
        urlPanel.add(new JLabel("URL: "), BorderLayout.WEST);
        urlField = new JTextField("http://localhost:8080/");
        urlPanel.add(urlField, BorderLayout.CENTER);

        // 请求方法面板
        JPanel methodPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        methodCombo = new JComboBox<>(new String[] { "GET", "HEAD", "POST" });
        methodPanel.add(new JLabel("方法: "));
        methodPanel.add(methodCombo);

        JButton sendButton = new JButton("发送请求");
        methodPanel.add(sendButton);

        JButton viewCookiesButton = new JButton("查看Cookies");
        methodPanel.add(viewCookiesButton);

        JButton clearButton = new JButton("清空响应");
        methodPanel.add(clearButton);

        // 请求与响应面板 - 使用分割面板
        JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        splitPane.setResizeWeight(0.3); // 上部分占30%，下部分占70%

        // 请求面板
        JPanel requestPanel = new JPanel(new BorderLayout());
        requestPanel.add(new JLabel("请求正文 (用于POST请求):"), BorderLayout.NORTH);
        requestArea = new JTextArea(8, 50);
        requestArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        requestArea.setLineWrap(true);
        requestArea.setWrapStyleWord(true);

        JScrollPane requestScrollPane = new JScrollPane(requestArea);
        requestScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        requestScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        requestPanel.add(requestScrollPane, BorderLayout.CENTER);

        // 响应面板
        JPanel responsePanel = new JPanel(new BorderLayout());
        responsePanel.add(new JLabel("响应:"), BorderLayout.NORTH);
        responseArea = new JTextArea(20, 50);
        responseArea.setEditable(false);
        responseArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        responseArea.setLineWrap(true);
        responseArea.setWrapStyleWord(true);

        // 为响应区域添加滑动条
        JScrollPane responseScrollPane = new JScrollPane(responseArea);
        responseScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        responseScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        // 设置滑动条的速度
        responseScrollPane.getVerticalScrollBar().setUnitIncrement(16);
        responseScrollPane.getHorizontalScrollBar().setUnitIncrement(16);

        responsePanel.add(responseScrollPane, BorderLayout.CENTER);

        // 将面板添加到分割面板
        splitPane.setTopComponent(requestPanel);
        splitPane.setBottomComponent(responsePanel);

        // 主面板
        JPanel mainPanel = new JPanel(new BorderLayout());
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.add(urlPanel, BorderLayout.NORTH);
        topPanel.add(methodPanel, BorderLayout.CENTER);

        mainPanel.add(topPanel, BorderLayout.NORTH);
        mainPanel.add(splitPane, BorderLayout.CENTER);

        add(mainPanel);

        // 添加事件处理
        sendButton.addActionListener(e -> sendRequest());
        viewCookiesButton.addActionListener(e -> showCookies());
        clearButton.addActionListener(e -> responseArea.setText(""));

        // 添加键盘快捷键
        urlField.addActionListener(e -> sendRequest()); // 在URL框按Enter发送请求
    }

    private void sendRequest() {
        String urlString = urlField.getText();
        String method = (String) methodCombo.getSelectedItem();
        String requestBody = requestArea.getText();

        // 在发送请求前显示加载状态
        responseArea.setText("正在发送请求...");
        responseArea.setCaretPosition(0); // 滚动到顶部

        // 使用SwingWorker在后台执行网络请求，避免UI冻结
        SwingWorker<String, Void> worker = new SwingWorker<String, Void>() {
            @Override
            protected String doInBackground() throws Exception {
                return performHttpRequest(urlString, method, requestBody);
            }

            @Override
            protected void done() {
                try {
                    String result = get();
                    responseArea.setText(result);
                    responseArea.setCaretPosition(0); // 滚动到顶部
                } catch (Exception e) {
                    responseArea.setText("发送请求时出错: " + e.getMessage());
                }
            }
        };

        worker.execute();
    }

    private String performHttpRequest(String urlString, String method, String requestBody) throws IOException {
        URL url = new URL(urlString);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();

        try {
            // 设置请求方法
            connection.setRequestMethod(method);
            connection.setRequestProperty("User-Agent", "Chino-HttpClient/1.0");
            connection.setRequestProperty("Accept", "*/*");

            // 如果是POST请求，添加请求体
            if ("POST".equalsIgnoreCase(method) && !requestBody.isEmpty()) {
                connection.setDoOutput(true);
                connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
                connection.setRequestProperty("Content-Length", String.valueOf(requestBody.getBytes("UTF-8").length));

                // 确保连接超时设置
                connection.setConnectTimeout(10000);
                connection.setReadTimeout(10000);

                try (DataOutputStream writer = new DataOutputStream(connection.getOutputStream())) {
                    writer.write(requestBody.getBytes("UTF-8"));
                    writer.flush();
                }
            }

            // 获取响应码，这会触发实际的HTTP请求
            int responseCode = connection.getResponseCode();

            // 设置响应
            StringBuilder response = new StringBuilder();
            response.append("=== HTTP 响应 ===\n");
            response.append("状态码: ").append(connection.getResponseCode())
                    .append(" ").append(connection.getResponseMessage()).append("\n\n");

            // 获取响应头
            response.append("=== 响应头 ===\n");
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
                response.append("=== 响应体 ===\n");
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

            return response.toString();
        } catch (IOException e) {
            throw e;
        } finally {
            connection.disconnect();
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
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                // 使用默认外观
            }

            HttpClient client = new HttpClient();
            client.setVisible(true);
        });
    }
}