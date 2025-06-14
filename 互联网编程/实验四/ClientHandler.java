import java.io.*;
import java.net.*;
import java.nio.file.Files;
import java.util.*;
import java.util.logging.*;

/**
 * 处理客户端HTTP请求
 */
public class ClientHandler implements Runnable {
    private static final Logger logger = Logger.getLogger(ClientHandler.class.getName());
    private static final String WEB_ROOT = "webroot";
    private static final String DEFAULT_FILE = "index.html";
    private static final Map<String, String> MIME_TYPES = new HashMap<>();

    static {
        // 初始化MIME类型映射
        MIME_TYPES.put("html", "text/html");
        MIME_TYPES.put("htm", "text/html");
        MIME_TYPES.put("txt", "text/plain");
        MIME_TYPES.put("css", "text/css");
        MIME_TYPES.put("js", "application/javascript");
        MIME_TYPES.put("jpg", "image/jpeg");
        MIME_TYPES.put("jpeg", "image/jpeg");
        MIME_TYPES.put("png", "image/png");
        MIME_TYPES.put("gif", "image/gif");
        MIME_TYPES.put("ico", "image/x-icon");
    }

    private final Socket socket;
    private final Chino server;
    private final SessionManager sessionManager;
    private final UserManager userManager;

    public ClientHandler(Socket socket, Chino server, SessionManager sessionManager, UserManager userManager) {
        this.socket = socket;
        this.server = server;
        this.sessionManager = sessionManager;
        this.userManager = userManager;
    }

    @Override
    public void run() {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                BufferedOutputStream outputStream = new BufferedOutputStream(socket.getOutputStream())) {

            // 读取HTTP请求头信息
            String requestLine = reader.readLine();
            if (requestLine == null) {
                return;
            }

            logger.info("收到请求: " + requestLine);

            // 解析请求行
            StringTokenizer tokenizer = new StringTokenizer(requestLine);
            String method = tokenizer.hasMoreTokens() ? tokenizer.nextToken().toUpperCase() : "";
            String path = tokenizer.hasMoreTokens() ? tokenizer.nextToken() : "";

            // 读取所有请求头
            Map<String, String> headers = new HashMap<>();
            String headerLine;
            while ((headerLine = reader.readLine()) != null && !headerLine.isEmpty()) {
                int colonPos = headerLine.indexOf(':');
                if (colonPos > 0) {
                    String headerName = headerLine.substring(0, colonPos).trim();
                    String headerValue = headerLine.substring(colonPos + 1).trim();
                    headers.put(headerName.toLowerCase(), headerValue);
                }
            }

            // 处理Cookie
            Map<String, String> cookies = parseCookies(headers.get("cookie"));

            // 读取请求体(如果是POST请求)
            StringBuilder requestBody = new StringBuilder();
            if ("POST".equals(method) && headers.containsKey("content-length")) {
                int contentLength = Integer.parseInt(headers.get("content-length"));
                char[] buffer = new char[contentLength];
                int bytesRead = reader.read(buffer, 0, contentLength);
                if (bytesRead > 0) {
                    requestBody.append(buffer, 0, bytesRead);
                }
            }

            // 根据请求方法处理请求
            switch (method) {
                case "GET":
                    handleGetRequest(path, headers, cookies, outputStream);
                    break;
                case "HEAD":
                    handleHeadRequest(path, headers, cookies, outputStream);
                    break;
                case "POST":
                    handlePostRequest(path, headers, cookies, requestBody.toString(), outputStream);
                    break;
                default:
                    // 不支持的方法
                    sendErrorResponse(outputStream, 501, "Not Implemented");
                    break;
            }

        } catch (IOException e) {
            logger.log(Level.WARNING, "处理客户端请求时出错", e);
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                logger.log(Level.WARNING, "关闭客户端连接时出错", e);
            }
            server.connectionClosed();
        }
    }

    private Map<String, String> parseCookies(String cookieHeader) {
        Map<String, String> cookies = new HashMap<>();
        if (cookieHeader != null) {
            String[] cookiePairs = cookieHeader.split(";");
            for (String cookiePair : cookiePairs) {
                int equalsPos = cookiePair.indexOf('=');
                if (equalsPos > 0) {
                    String name = cookiePair.substring(0, equalsPos).trim();
                    String value = cookiePair.substring(equalsPos + 1).trim();
                    cookies.put(name, value);
                }
            }
        }
        return cookies;
    }

    private void handleGetRequest(String path, Map<String, String> headers,
            Map<String, String> cookies, BufferedOutputStream out) throws IOException {
        // 处理路径，处理默认页面
        if (path.equals("/")) {
            path = "/" + DEFAULT_FILE;
        }

        File file = new File(WEB_ROOT, path.substring(1));

        // 检查文件是否存在
        if (!file.exists()) {
            sendErrorResponse(out, 404, "Not Found");
            return;
        }

        // 如果是目录，尝试查找默认文件
        if (file.isDirectory()) {
            file = new File(file, DEFAULT_FILE);
            if (!file.exists()) {
                sendErrorResponse(out, 404, "Not Found");
                return;
            }
        }

        // 获取文件MIME类型
        String contentType = getMimeType(file.getName());

        // 发送响应头和文件内容
        byte[] fileData = Files.readAllBytes(file.toPath());

        // 构建响应头
        StringBuilder responseHeader = new StringBuilder();
        responseHeader.append("HTTP/1.1 200 OK\r\n");
        responseHeader.append("Content-Type: ").append(contentType).append("\r\n");
        responseHeader.append("Content-Length: ").append(fileData.length).append("\r\n");

        // 设置会话Cookie (如果不存在)
        if (!cookies.containsKey("session_id")) {
            String sessionId = generateSessionId();
            responseHeader.append("Set-Cookie: session_id=").append(sessionId)
                    .append("; Path=/; Max-Age=3600\r\n");
        }

        // 结束响应头
        responseHeader.append("\r\n");

        // 发送响应
        out.write(responseHeader.toString().getBytes());
        out.write(fileData);
        out.flush();
    }

    private void handleHeadRequest(String path, Map<String, String> headers,
            Map<String, String> cookies, BufferedOutputStream out) throws IOException {
        // HEAD请求与GET相同，但不返回响应体
        if (path.equals("/")) {
            path = "/" + DEFAULT_FILE;
        }

        File file = new File(WEB_ROOT, path.substring(1));

        if (!file.exists()) {
            sendErrorResponse(out, 404, "Not Found");
            return;
        }

        if (file.isDirectory()) {
            file = new File(file, DEFAULT_FILE);
            if (!file.exists()) {
                sendErrorResponse(out, 404, "Not Found");
                return;
            }
        }

        // 获取文件MIME类型
        String contentType = getMimeType(file.getName());

        // 只发送响应头，不发送文件内容
        StringBuilder responseHeader = new StringBuilder();
        responseHeader.append("HTTP/1.1 200 OK\r\n");
        responseHeader.append("Content-Type: ").append(contentType).append("\r\n");
        responseHeader.append("Content-Length: ").append(file.length()).append("\r\n");

        // 设置会话Cookie (如果不存在)
        if (!cookies.containsKey("session_id")) {
            String sessionId = generateSessionId();
            responseHeader.append("Set-Cookie: session_id=").append(sessionId)
                    .append("; Path=/; Max-Age=3600\r\n");
        }

        responseHeader.append("\r\n");

        out.write(responseHeader.toString().getBytes());
        out.flush();
    }

    private void handlePostRequest(String path, Map<String, String> headers, Map<String, String> cookies,
            String requestBody, BufferedOutputStream out) throws IOException {
        // 登录请求处理
        if (path.equals("/login")) {
            String username = parseFormField(requestBody, "username");
            String password = parseFormField(requestBody, "password");

            StringBuilder response = new StringBuilder();
            response.append("HTTP/1.1 200 OK\r\n");
            response.append("Content-Type: text/html; charset=UTF-8\r\n");

            // 设置会话Cookie
            if (!cookies.containsKey("session_id")) {
                String sessionId = generateSessionId();
                response.append("Set-Cookie: session_id=").append(sessionId)
                        .append("; Path=/; Max-Age=3600\r\n");
            }

            // 验证用户登录
            boolean loginSuccess = false;
            if (username != null && password != null) {
                // 使用UserManager验证用户
                loginSuccess = userManager.validateUser(username, password);
                if (loginSuccess) {
                    // 设置用户Cookie
                    response.append("Set-Cookie: username=").append(username)
                            .append("; Path=/; Max-Age=3600\r\n");
                }
            }

            response.append("\r\n");

            // 构建响应内容
            String responseBody;
            if (loginSuccess) {
                responseBody = "<html><body>" +
                        "<h1>登录成功</h1>" +
                        "<p>欢迎回来, " + username + "!</p>" +
                        "<p><a href='index.html'>返回首页</a></p>" +
                        "</body></html>";
            } else {
                responseBody = "<html><body>" +
                        "<h1>登录失败</h1>" +
                        "<p>用户名或密码不正确</p>" +
                        "<p><a href='login.html'>重新登录</a></p>" +
                        "</body></html>";
            }

            response.append(responseBody);
            out.write(response.toString().getBytes("UTF-8"));
            out.flush();
            return;
        }

        // 注册请求处理
        else if (path.equals("/register")) {
            String username = parseFormField(requestBody, "username");
            String password = parseFormField(requestBody, "password");
            String email = parseFormField(requestBody, "email");

            StringBuilder response = new StringBuilder();
            response.append("HTTP/1.1 200 OK\r\n");
            response.append("Content-Type: text/html; charset=UTF-8\r\n");

            // 注册用户
            boolean registerSuccess = false;
            if (username != null && !username.isEmpty() && password != null && !password.isEmpty()) {
                registerSuccess = userManager.registerUser(username, password, email == null ? "" : email);
            }

            response.append("\r\n");

            // 构建响应内容
            String responseBody;
            if (registerSuccess) {
                responseBody = "<html><body>" +
                        "<h1>注册成功</h1>" +
                        "<p>您的账号 " + username + " 已创建!</p>" +
                        "<p><a href='login.html'>返回登录</a></p>" +
                        "</body></html>";
            } else {
                responseBody = "<html><body>" +
                        "<h1>注册失败</h1>" +
                        "<p>用户名已存在或输入信息不完整</p>" +
                        "<p><a href='register.html'>重新注册</a></p>" +
                        "</body></html>";
            }

            response.append(responseBody);
            out.write(response.toString().getBytes("UTF-8"));
            out.flush();
            return;
        }

        // 其他POST请求处理...
    }

    private String parseFormField(String formData, String fieldName) {
        String[] pairs = formData.split("&");
        for (String pair : pairs) {
            String[] keyValue = pair.split("=");
            if (keyValue.length == 2 && keyValue[0].equals(fieldName)) {
                try {
                    return URLDecoder.decode(keyValue[1], "UTF-8");
                } catch (UnsupportedEncodingException e) {
                    return keyValue[1];
                }
            }
        }
        return null;
    }

    private void sendErrorResponse(BufferedOutputStream out, int statusCode, String statusMessage) throws IOException {
        String responseBody = "<html><body><h1>" + statusCode + " " + statusMessage + "</h1></body></html>";

        StringBuilder response = new StringBuilder();
        response.append("HTTP/1.1 ").append(statusCode).append(" ").append(statusMessage).append("\r\n");
        response.append("Content-Type: text/html\r\n");
        response.append("Content-Length: ").append(responseBody.getBytes().length).append("\r\n");
        response.append("\r\n");
        response.append(responseBody);

        out.write(response.toString().getBytes());
        out.flush();
    }

    private String getMimeType(String fileName) {
        int dotPos = fileName.lastIndexOf('.');
        if (dotPos > 0) {
            String extension = fileName.substring(dotPos + 1).toLowerCase();
            if (MIME_TYPES.containsKey(extension)) {
                return MIME_TYPES.get(extension);
            }
        }
        return "application/octet-stream"; // 默认二进制类型
    }

    private String generateSessionId() {
        return UUID.randomUUID().toString().replace("-", "");
    }

    private boolean isValidPath(String path) {
        try {
            // 标准化路径
            String normalizedPath = new File(WEB_ROOT, path).getCanonicalPath();
            String rootPath = new File(WEB_ROOT).getCanonicalPath();

            // 确保请求的文件在Web根目录下
            return normalizedPath.startsWith(rootPath);
        } catch (IOException e) {
            logger.log(Level.WARNING, "路径验证失败", e);
            return false; // 如果出现IO异常，认为路径不安全
        }
    }
}