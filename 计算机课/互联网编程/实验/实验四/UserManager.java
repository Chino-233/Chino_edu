import java.io.*;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

/**
 * 用户管理器
 * 处理用户注册、登录验证和用户数据持久化
 */
public class UserManager {
    private static final String USERS_FILE = "users.txt";
    private final Map<String, UserInfo> users = new ConcurrentHashMap<>();

    /**
     * 用户信息内部类
     */
    public static class UserInfo {
        private final String username;
        private final String password;
        private final String email;
        private final long registrationTime;

        public UserInfo(String username, String password, String email) {
            this.username = username;
            this.password = password;
            this.email = email;
            this.registrationTime = System.currentTimeMillis();
        }

        public UserInfo(String username, String password, String email, long registrationTime) {
            this.username = username;
            this.password = password;
            this.email = email;
            this.registrationTime = registrationTime;
        }

        // Getter方法
        public String getUsername() {
            return username;
        }

        public String getPassword() {
            return password;
        }

        public String getEmail() {
            return email;
        }

        public long getRegistrationTime() {
            return registrationTime;
        }

        // 转换为文本格式存储
        public String toFileString() {
            return username + "|" + password + "|" + email + "|" + registrationTime;
        }

        // 从文本格式创建UserInfo
        public static UserInfo fromFileString(String line) {
            String[] parts = line.split("\\|", 4);
            if (parts.length >= 3) {
                String username = parts[0];
                String password = parts[1];
                String email = parts[2];
                long registrationTime = parts.length > 3 ? parseLong(parts[3], System.currentTimeMillis())
                        : System.currentTimeMillis();
                return new UserInfo(username, password, email, registrationTime);
            }
            return null;
        }

        private static long parseLong(String str, long defaultValue) {
            try {
                return Long.parseLong(str);
            } catch (NumberFormatException e) {
                return defaultValue;
            }
        }

        @Override
        public String toString() {
            return "UserInfo{username='" + username + "', email='" + email +
                    "', registrationTime=" + new Date(registrationTime) + "}";
        }
    }

    public UserManager() {
        loadUsers();
    }

    public synchronized boolean registerUser(String username, String password, String email) {
        if (username == null || username.trim().isEmpty() ||
                password == null || password.trim().isEmpty()) {
            System.err.println("注册失败：用户名或密码为空");
            return false;
        }

        username = username.trim();
        password = password.trim();
        email = (email != null) ? email.trim() : "";

        if (users.containsKey(username)) {
            System.err.println("注册失败：用户名 " + username + " 已存在");
            return false;
        }

        try {
            UserInfo userInfo = new UserInfo(username, password, email);
            users.put(username, userInfo);

            if (saveUsers()) {
                System.out.println("用户注册成功：" + username);
                return true;
            } else {
                // 保存失败，回滚注册
                users.remove(username);
                System.err.println("用户数据保存失败，注册回滚");
                return false;
            }
        } catch (Exception e) {
            System.err.println("注册用户时发生异常：" + e.getMessage());
            users.remove(username); // 确保回滚
            return false;
        }
    }

    public boolean validateUser(String username, String password) {
        if (username == null || password == null) {
            return false;
        }

        username = username.trim();
        password = password.trim();

        UserInfo userInfo = users.get(username);
        if (userInfo != null) {
            boolean isValid = userInfo.getPassword().equals(password);
            if (isValid) {
                System.out.println("用户 " + username + " 登录成功");
            } else {
                System.out.println("用户 " + username + " 登录失败：密码错误");
            }
            return isValid;
        }

        System.out.println("用户 " + username + " 登录失败：用户不存在");
        return false;
    }

    public UserInfo getUserInfo(String username) {
        return users.get(username);
    }

    public Set<String> getAllUsers() {
        return new HashSet<>(users.keySet());
    }

    /**
     * 从文本文件加载用户数据
     */
    private void loadUsers() {
        File usersFile = new File(USERS_FILE);
        if (!usersFile.exists()) {
            System.out.println("用户数据文件不存在，创建默认用户");
            createDefaultUsers();
            return;
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(usersFile))) {
            String line;
            int loadedCount = 0;

            while ((line = reader.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")) {
                    continue; // 跳过空行和注释行
                }

                UserInfo userInfo = UserInfo.fromFileString(line);
                if (userInfo != null) {
                    users.put(userInfo.getUsername(), userInfo);
                    loadedCount++;
                } else {
                    System.err.println("无效的用户数据行：" + line);
                }
            }

            System.out.println("成功加载 " + loadedCount + " 个用户数据");

        } catch (IOException e) {
            System.err.println("加载用户数据失败: " + e.getMessage());
            System.out.println("创建默认用户");
            createDefaultUsers();
        }
    }

    /**
     * 保存用户数据到文本文件
     */
    private boolean saveUsers() {
        try (PrintWriter writer = new PrintWriter(new FileWriter(USERS_FILE))) {
            // 写入文件头注释
            writer.println("# Chino HTTP服务器用户数据文件");
            writer.println("# 格式：用户名|密码|邮箱|注册时间戳");
            writer.println("# 创建时间：" + new Date());
            writer.println();

            // 写入用户数据
            for (UserInfo userInfo : users.values()) {
                writer.println(userInfo.toFileString());
            }

            writer.flush();
            System.out.println("用户数据保存成功，共 " + users.size() + " 个用户");
            return true;

        } catch (IOException e) {
            System.err.println("保存用户数据失败: " + e.getMessage());
            return false;
        }
    }

    /**
     * 创建默认用户
     */
    private void createDefaultUsers() {
        System.out.println("创建默认用户");

        // 创建一些默认用户用于测试
        users.put("admin", new UserInfo("admin", "admin123", "admin@example.com"));
        users.put("test", new UserInfo("test", "test123", "test@example.com"));
        users.put("user", new UserInfo("user", "user123", "user@example.com"));

        // 保存默认用户
        saveUsers();

        System.out.println("默认用户创建完成");
    }

    /**
     * 获取用户统计信息
     */
    public String getUserStats() {
        StringBuilder stats = new StringBuilder();
        stats.append("用户统计信息：\n");
        stats.append("总用户数：").append(users.size()).append("\n");
        stats.append("用户列表：\n");

        for (UserInfo user : users.values()) {
            stats.append("- ").append(user.toString()).append("\n");
        }

        return stats.toString();
    }

    /**
     * 删除用户（管理功能）
     */
    public synchronized boolean deleteUser(String username) {
        if (username == null || username.trim().isEmpty()) {
            return false;
        }

        UserInfo removed = users.remove(username.trim());
        if (removed != null) {
            saveUsers();
            System.out.println("用户 " + username + " 已删除");
            return true;
        }

        return false;
    }

    /**
     * 修改用户密码
     */
    public synchronized boolean changePassword(String username, String oldPassword, String newPassword) {
        if (username == null || oldPassword == null || newPassword == null) {
            return false;
        }

        UserInfo userInfo = users.get(username.trim());
        if (userInfo != null && userInfo.getPassword().equals(oldPassword.trim())) {
            // 创建新的UserInfo对象（因为字段是final的）
            UserInfo newUserInfo = new UserInfo(
                    userInfo.getUsername(),
                    newPassword.trim(),
                    userInfo.getEmail(),
                    userInfo.getRegistrationTime());

            users.put(username.trim(), newUserInfo);
            saveUsers();
            System.out.println("用户 " + username + " 密码修改成功");
            return true;
        }

        return false;
    }
}