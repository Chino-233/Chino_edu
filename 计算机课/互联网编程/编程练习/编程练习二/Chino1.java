import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class Chino1 {
    public static void main(String[] args) {
        try {
            // 1) 获取本机地址
            InetAddress localHost = InetAddress.getLocalHost();
            System.out.println("本机主机名: " + localHost.getHostName());
            System.out.println("本机IP地址: " + localHost.getHostAddress());
            
            // 2) 查看www.szu.edu.cn的地址并测试可达性
            InetAddress szuAddress = InetAddress.getByName("www.szu.edu.cn");
            System.out.println("\nwww.szu.edu.cn的主机名: " + szuAddress.getHostName());
            System.out.println("www.szu.edu.cn的IP地址: " + szuAddress.getHostAddress());
            // 测试www.szu.edu.cn的可达性
            boolean reachable = szuAddress.isReachable(3000);
            System.out.println("www.szu.edu.cn是否可达: " + (reachable ? "可达" : "不可达"));
            
        } catch (UnknownHostException e) {
            System.err.println("找不到主机: " + e.getMessage());
        } catch (IOException e) {
            System.err.println("IO异常: " + e.getMessage());
        }
    }
}