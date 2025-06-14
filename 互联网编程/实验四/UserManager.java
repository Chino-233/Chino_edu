import java.io.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;

/**
 * 用户管理类
 * 负责用户信息的存储、验证和管理
 */
public class UserManager {
    private static final String USER_FILE = "users.dat";
    private Map<String, UserInfo> users = new HashMap<>();

    public UserManager() {
        loadUsers();
    }

    /**
     * 用户信息类
     */
    public static class UserInfo {
        private String username;
        private String passwordHash; // 存储密码的哈希值而非明文
        private String email;

        public UserInfo(String username, String password, String email) {
            this.username = username;
            this.passwordHash = hashPassword(password);
            this.email = email;
        }

        public String getUsername() {
            return username;
        }

        public String getPasswordHash() {
            return passwordHash;
        }

        public String getEmail() {
            return email;
        }
    }

    /**
     * 注册新用户
     */
    public boolean registerUser(String username, String password, String email) {
        if (users.containsKey(username)) {
            return false; // 用户已存在
        }

        UserInfo newUser = new UserInfo(username, password, email);
        users.put(username, newUser);
        saveUsers();
        return true;
    }

    /**
     * 验证用户登录
     */
    public boolean validateUser(String username, String password) {
        UserInfo user = users.get(username);
        if (user == null) {
            return false;
        }

        String inputHash = hashPassword(password);
        return inputHash.equals(user.getPasswordHash());
    }

    /**
     * 获取用户信息
     */
    public UserInfo getUserInfo(String username) {
        return users.get(username);
    }

    /**
     * 密码哈希处理
     */
    private static String hashPassword(String password) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] hash = md.digest(password.getBytes());
            StringBuilder sb = new StringBuilder();
            for (byte b : hash) {
                sb.append(String.format("%02x", b));
            }
            return sb.toString();
        } catch (NoSuchAlgorithmException e) {
            return ""; // 处理异常情况
        }
    }

    /**
     * 加载用户信息
     */
    private void loadUsers() {
        File file = new File(USER_FILE);
        if (!file.exists()) {
            return;
        }

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file))) {
            @SuppressWarnings("unchecked")
            Map<String, UserInfo> loadedUsers = (Map<String, UserInfo>) ois.readObject();
            users = loadedUsers;
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("加载用户数据失败: " + e.getMessage());
        }
    }

    /**
     * 保存用户信息
     */
    private void saveUsers() {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(USER_FILE))) {
            oos.writeObject(users);
        } catch (IOException e) {
            System.err.println("保存用户数据失败: " + e.getMessage());
        }
    }
}