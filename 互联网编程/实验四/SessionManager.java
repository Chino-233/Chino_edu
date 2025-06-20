import java.util.*;
import java.util.concurrent.*;

/**
 * HTTP会话管理器
 */
public class SessionManager {
    private static final long SESSION_TIMEOUT = 30 * 60 * 1000; // 30分钟
    private final Map<String, Session> sessions = new ConcurrentHashMap<>();
    private final ScheduledExecutorService cleanupService;

    public SessionManager() {
        cleanupService = Executors.newSingleThreadScheduledExecutor();
        cleanupService.scheduleAtFixedRate(this::cleanupSessions, 5, 5, TimeUnit.MINUTES);
    }

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
            return session;
        }

        return null;
    }

    public void invalidateSession(String sessionId) {
        if (sessionId != null) {
            sessions.remove(sessionId);
        }
    }

    private void cleanupSessions() {
        long now = System.currentTimeMillis();
        sessions.entrySet().removeIf(entry -> (now - entry.getValue().getLastAccessTime()) > SESSION_TIMEOUT);
    }

    private String generateSessionId() {
        return UUID.randomUUID().toString().replace("-", "");
    }

    public void shutdown() {
        cleanupService.shutdown();
    }

    public int getActiveSessionCount() {
        return sessions.size();
    }

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