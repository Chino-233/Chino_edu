import java.net.*;
import java.util.Scanner;


public class DomainResolver {
    

    public static String[] resolveDomain(String domain) throws UnknownHostException {
        System.out.println("正在解析域名: " + domain);
        InetAddress[] addresses = InetAddress.getAllByName(domain);
        String[] ipAddresses = new String[addresses.length];
        
        for (int i = 0; i < addresses.length; i++) {
            ipAddresses[i] = addresses[i].getHostAddress();
        }
        return ipAddresses;
    }
    

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("===== 域名解析工具 =====");
        System.out.println("输入'exit'退出程序");
        
        while (true) {
            System.out.print("\n请输入要解析的域名: ");
            String domain = scanner.nextLine().trim();
            
            if (domain.equalsIgnoreCase("exit")) {
                System.out.println("程序已退出");
                break;
            }
            
            if (domain.isEmpty()) {
                System.out.println("域名不能为空，请重新输入");
                continue;
            }
            
            try {
                String[] ipAddresses = resolveDomain(domain);
                System.out.println("域名 " + domain + " 解析结果:");
                for (String ip : ipAddresses) {
                    System.out.println("  - " + ip);
                }
            } catch (UnknownHostException e) {
                System.out.println("无法解析域名: " + e.getMessage());
            }
        }
        
        scanner.close();
    }
}