

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextArea;

public class Chino_1 {
    public static final Map<String, String> translations = new HashMap<>();

    static {
        translations.put("建校41年，深圳大学秉承“自立、自律、自强”的校训，紧随特区，锐意改革、快速发展，为特区发展和国家现代化建设做出了重要贡献。",
                "Sticking to the motto of “self-reliance, self-discipline, self-improvement”, the University is dedicated to serving the Shenzhen Special Economic Zone (SEZ), demonstrating China’s reform and opening up and pioneering change in higher education.");
        translations.put(
                "Sticking to the motto of “self-reliance, self-discipline, self-improvement”, the University is dedicated to serving the Shenzhen Special Economic Zone (SEZ), demonstrating China’s reform and opening up and pioneering change in higher education.",
                "建校41年，深圳大学秉承“自立、自律、自强”的校训，紧随特区，锐意改革、快速发展，为特区发展和国家现代化建设做出了重要贡献。");
    }

    public static String decodeUnicode(String unicodeStr) {
        StringBuilder sb = new StringBuilder();
        String[] hex = unicodeStr.split("\\\\u");
        for (int i = 1; i < hex.length; i++) {
            int data = Integer.parseInt(hex[i], 16);
            sb.append((char) data);
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("中英互译 Demo");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(700, 400);
        frame.setLayout(new GridLayout(3, 3));

        JLabel label1 = new JLabel("中文句子:");
        JTextArea textArea1 = new JTextArea();
        textArea1.setLineWrap(true);
        textArea1.setWrapStyleWord(true);
        JButton button1 = new JButton("中译英");

        JLabel label2 = new JLabel("英文句子:");
        JTextArea textArea2 = new JTextArea();
        textArea2.setLineWrap(true);
        textArea2.setWrapStyleWord(true);
        JButton button2 = new JButton("英译中");
        JLabel label3 = new JLabel("这里是分界线~");
        JLabel label4 = new JLabel("请在上面填入中文或在下面填入英文");
        JLabel label5 = new JLabel("请点击按钮进行翻译~");
        button1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String chineseText = textArea1.getText();
                String englishTranslation = translations.getOrDefault(chineseText, "翻译未找到");
                if ("翻译未找到".equals(englishTranslation)) {
                    englishTranslation = translateZhToEn(chineseText);
                }
                textArea2.setText(englishTranslation);
            }
        });

        button2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String englishText = textArea2.getText();
                String chineseTranslation = translations.getOrDefault(englishText, "翻译未找到");
                if ("翻译未找到".equals(chineseTranslation)) {
                    chineseTranslation = translateEnToZh(englishText);
                }
                textArea1.setText(chineseTranslation);
            }
        });

        frame.add(label1);
        frame.add(textArea1);
        frame.add(button1);
        frame.add(label3);
        frame.add(label4);
        frame.add(label5);
        frame.add(label2);  
        frame.add(textArea2);
        frame.add(button2);

        frame.setVisible(true);
    }

    private static String translateZhToEn(String query) {
        String appId = "**"; 
        String secretKey = "**";
        String salt = String.valueOf(new Random().nextInt(10000));
        String sign = md5(appId + query + salt + secretKey);
        String url = "http://api.fanyi.baidu.com/api/trans/vip/translate?q=" + encode(query) + "&from=zh&to=en&appid="
                + appId + "&salt=" + salt + "&sign=" + sign;

        try {
            HttpURLConnection conn = (HttpURLConnection) new URL(url).openConnection();
            conn.setRequestMethod("GET");
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            StringBuilder result = new StringBuilder();
            String line;
            while ((line = in.readLine()) != null) {
                result.append(line);
            }
            in.close();
            // 解析返回的JSON数据
            String response = result.toString();
            System.err.println(response);
            // 获取dst字段的起始位置
            int start = response.indexOf("\"dst\":\"") + 7;
            // 获取dst字段的结束位置
            int end = response.indexOf("\"", start);
            // 获取dst字段的值并进行URL解码
            String translatedText = response.substring(start, end);
            System.err.println("translatedText:" + translatedText);
            if((translatedText.charAt(0)=='\\')&&(translatedText.charAt(1)=='u')){
                translatedText = decodeUnicode(translatedText);
            }
            else if(translatedText.equals("r_code")){
                String temp="";
                return temp;
            }
            System.err.println("translatedText:" + translatedText);
            return translatedText;
        } catch (Exception e) {
            e.printStackTrace();
            return "翻译失败";
        }
    }

    private static String translateEnToZh(String query) {
        String appId = "**"; // 替换为你的百度翻译API的App ID
        String secretKey = "**"; // 替换为你的百度翻译API的Secret Key
        String salt = String.valueOf(new Random().nextInt(10000));
        String sign = md5(appId + query + salt + secretKey);
        String url = "http://api.fanyi.baidu.com/api/trans/vip/translate?q=" + encode(query) + "&from=en&to=zh&appid="
                + appId + "&salt=" + salt + "&sign=" + sign;

        try {
            HttpURLConnection conn = (HttpURLConnection) new URL(url).openConnection();
            conn.setRequestMethod("GET");
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            StringBuilder result = new StringBuilder();
            String line;
            while ((line = in.readLine()) != null) {
                result.append(line);
            }
            in.close();
            // 解析返回的JSON数据
            String response = result.toString();
            System.err.println(response);
            // 获取dst字段的起始位置
            int start = response.indexOf("\"dst\":\"") + 7;
            // 获取dst字段的结束位置
            int end = response.indexOf("\"", start);
            // 获取dst字段的值并进行URL解码
            String translatedText = response.substring(start, end);
            //translatedText = decodeUnicode(translatedText);
            System.err.println("translatedText: " + translatedText);
            if(translatedText.equals("r_code")){
                String temp="";
                return temp;
            }
        // 遍历翻译后的文本
            for(int i=0;i<translatedText.length();i++){
        // 定义临时字符串
                String temp="";
        // 如果翻译后的文本中存在反斜杠
                if(translatedText.charAt(i)=='\\'){
        // 获取反斜杠后的三个字符
                    temp=translatedText.substring(i,i+6);
        // 将这三个字符解码
                    String temp2=decodeUnicode(temp);
        // 将解码后的字符串替换原来的字符串
                    translatedText=translatedText.replace(temp,temp2);
                }
            }
            System.err.println("translatedText: " + translatedText);
            return translatedText;
        } catch (Exception e) {
            e.printStackTrace();
            return "翻译失败";
        }
    }

    private static String md5(String input) {
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] array = md.digest(input.getBytes());
            StringBuilder sb = new StringBuilder();
            for (byte b : array) {
                sb.append(String.format("%02x", b));
            }
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static String encode(String input) {
        try {
            return URLEncoder.encode(input, "UTF-8");
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}

