import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Logger;

/**
 * HTTP会话管理器
 * 用于管理用户会话和Cookie
 */
public class SessionManager {
    private static final Logger logger = Logger.getLogger(SessionManager.class.getName());
    private static final long SESSION_TIMEOUT = 30 * 60 * 1000; // 30分钟会话超时

    private final Map<String, Session> sessions = new ConcurrentHashMap<>();
    private final ScheduledExecutorService cleanupService;

    public SessionManager() {
        // 启动定期清理过期会话的任务
        cleanupService = Executors.newSingleThreadScheduledExecutor();
        cleanupService.scheduleAtFixedRate(this::cleanupSessions, 5, 5, TimeUnit.MINUTES);
        logger.info("会话管理器已初始化");
    }

    /**
     * 获取或创建会话
     */
    public Session getSession(String sessionId, boolean create) {
        Session session = null;

        if (sessionId != null) {
            session = sessions.get(sessionId);
            if (session != null) {
                session.access();
                return session;
            }
        }

        if (create) {
            sessionId = generateSessionId();
            session = new Session(sessionId);
            sessions.put(sessionId, session);
            logger.info("创建新会话: " + sessionId);
            return session;
        }

        return null;
    }

    /**
     * 注销会话
     */
    public void invalidateSession(String sessionId) {
        if (sessionId != null && sessions.remove(sessionId) != null) {
            logger.info("注销会话: " + sessionId);
        }
    }

    /**
     * 清理过期会话
     */
    private void cleanupSessions() {
        long now = System.currentTimeMillis();
        int beforeCount = sessions.size();

        sessions.entrySet().removeIf(entry -> {
            boolean expired = (now - entry.getValue().getLastAccessTime()) > SESSION_TIMEOUT;
            if (expired) {
                logger.fine("会话过期: " + entry.getKey());
            }
            return expired;
        });

        int afterCount = sessions.size();
        if (beforeCount > afterCount) {
            logger.info("清理了 " + (beforeCount - afterCount) + " 个过期会话");
        }
    }

    /**
     * 生成唯一会话ID
     */
    private String generateSessionId() {
        return UUID.randomUUID().toString().replace("-", "");
    }

    /**
     * 关闭会话管理器
     */
    public void shutdown() {
        cleanupService.shutdown();
        logger.info("会话管理器已关闭");
    }

    /**
     * 获取活跃会话数
     */
    public int getActiveSessionCount() {
        return sessions.size();
    }

    /**
     * 会话类
     */
    public static class Session {
        private final String id;
        private final Map<String, Object> attributes = new ConcurrentHashMap<>();
        private long lastAccessTime;

        public Session(String id) {
            this.id = id;
            this.lastAccessTime = System.currentTimeMillis();
        }

        public String getId() {
            return id;
        }

        public void setAttribute(String name, Object value) {
            attributes.put(name, value);
        }

        public Object getAttribute(String name) {
            return attributes.get(name);
        }

        public void removeAttribute(String name) {
            attributes.remove(name);
        }

        public long getLastAccessTime() {
            return lastAccessTime;
        }

        public void access() {
            lastAccessTime = System.currentTimeMillis();
        }
    }
}