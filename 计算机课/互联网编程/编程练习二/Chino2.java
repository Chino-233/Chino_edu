import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Chino2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean running = true;
        
        System.out.println("域名和IP地址互查程序");
        
        while (running) {
            System.out.println("\n请选择查询模式：");
            System.out.println("1. 通过域名查询IP地址");
            System.out.println("2. 通过IP地址查询域名");
            System.out.println("0. 退出程序");
            System.out.print("请输入选项: ");
            
            int choice;
            try {
                choice = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("无效输入，请输入数字选项。");
                continue;
            }
            
            switch (choice) {
                case 1:
                    lookupIPByHostname(scanner);
                    break;
                case 2:
                    lookupHostnameByIP(scanner);
                    break;
                case 0:
                    running = false;
                    System.out.println("程序已退出。");
                    break;
                default:
                    System.out.println("无效选项，请重新选择。");
            }
        }
        
        scanner.close();
    }
    private static void lookupIPByHostname(Scanner scanner) {
        System.out.print("请输入域名(例如 www.szu.edu.cn): ");
        String hostname = scanner.nextLine();
        try {
            InetAddress[] addresses = InetAddress.getAllByName(hostname);
            System.out.println("域名 " + hostname + " 对应的IP地址：");
            for (InetAddress address : addresses) {
                System.out.println("- " + address.getHostAddress());
            }
        } catch (UnknownHostException e) {
            System.err.println("无法解析域名: " + e.getMessage());
        }
    }
    private static void lookupHostnameByIP(Scanner scanner) {
        System.out.print("请输入IP地址(例如 192.168.1.1): ");
        String ipStr = scanner.nextLine();
        try {
            // 将IP地址字符串转换为字节数组
            String[] parts = ipStr.split("\\.");
            if (parts.length != 4) {
                System.out.println("无效的IPv4地址格式，请使用点分十进制表示法（例如：192.168.1.1）");
                return;
            }
            byte[] ipBytes = new byte[4];
            for (int i = 0; i < 4; i++) {
                try {
                    int value = Integer.parseInt(parts[i]);
                    if (value < 0 || value > 255) {
                        System.out.println("IP地址中的每个部分必须在0-255之间");
                        return;
                    }
                    ipBytes[i] = (byte) value;
                } catch (NumberFormatException e) {
                    System.out.println("IP地址格式错误：每部分必须是数字");
                    return;
                }
            }
            InetAddress address = InetAddress.getByAddress(ipBytes);
            System.out.println("IP地址 " + ipStr + " 对应的域名是：" + address.getHostName());
            if (address.getHostName().equals(ipStr)) {
                System.out.println("注意：没有找到对应的域名记录，或者此IP没有关联的域名");
            }
        } catch (UnknownHostException e) {
            System.err.println("无法解析IP地址: " + e.getMessage());
        }
    }
}