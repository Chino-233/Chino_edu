

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextArea;

public class Chino {
    // 定义一个Map，用于存储中文和英文的翻译
    private static final Map<String, String> translations = new HashMap<>();

    // 静态代码块，初始化Map
    static {
        // 将中文翻译存储到Map中
        translations.put("建校41年，深圳大学秉承“自立、自律、自强”的校训，紧随特区，锐意改革、快速发展，为特区发展和国家现代化建设做出了重要贡献。",
                "Sticking to the motto of “self-reliance, self-discipline, self-improvement”, the University is dedicated to serving the Shenzhen Special Economic Zone (SEZ), demonstrating China’s reform and opening up and pioneering change in higher education.");
        translations.put("Sticking to the motto of “self-reliance, self-discipline, self-improvement”, the University is dedicated to serving the Shenzhen Special Economic Zone (SEZ), demonstrating China’s reform and opening up and pioneering change in higher education.",
                "建校41年，深圳大学秉承“自立、自律、自强”的校训，紧随特区，锐意改革、快速发展，为特区发展和国家现代化建设做出了重要贡献。");
    }

    public static void main(String[] args) {
        // 创建一个JFrame对象，设置标题为"中英互译 Demo"
        // 创建一个名为“中英互译 Demo”的JFrame窗口
        JFrame frame = new JFrame("中英互译 Demo");
        // 设置窗口关闭时的默认操作
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // 设置窗口大小为700x400
        frame.setSize(700, 400);
        // 设置窗口布局为3x3的网格布局
        frame.setLayout(new GridLayout(3, 3));

        // 创建一个标签，显示中文句子
        JLabel label1 = new JLabel("中文句子:");
        // 创建一个文本区域，用于输入中文句子
        JTextArea textArea1 = new JTextArea();
        // 设置文本区域自动换行
        textArea1.setLineWrap(true);
        // 设置文本区域在换行时保持单词的完整性
        textArea1.setWrapStyleWord(true);
        // 设置文本区域自动换行
        textArea1.setLineWrap(true);
        // 设置文本区域在换行时保持单词的完整性
        textArea1.setWrapStyleWord(true);
        // 创建一个按钮，按钮上显示“中译英”
        JButton button1 = new JButton("中译英");

        // 创建一个标签，标签上显示“英文句子:”
        JLabel label2 = new JLabel("英文句子:");
        // 创建一个文本区域
        JTextArea textArea2 = new JTextArea();
        // 设置文本区域自动换行
        textArea2.setLineWrap(true);
        // 设置文本区域在换行时保持单词的完整性
        textArea2.setWrapStyleWord(true);
        // 创建一个按钮，按钮上显示“英译中”
        JButton button2 = new JButton("英译中");
        // 创建一个标签，显示文本“这里是分界线~”
        JLabel label3 = new JLabel("这里是分界线~");
        // 创建一个标签，显示文本“请在上面填入中文或在下面填入英文”
        JLabel label4 = new JLabel("请在上面填入中文或在下面填入英文");
        // 创建一个标签，显示文本“请点击按钮进行翻译~”
        JLabel label5 = new JLabel("请点击按钮进行翻译~");

        // 为按钮1添加监听器
        button1.addActionListener(new ActionListener() {
            // 重写actionPerformed方法
            @Override
            public void actionPerformed(ActionEvent e) {
                // 获取文本框1中的文本
                String chineseText = textArea1.getText();
                // 获取翻译
                String englishTranslation = translations.getOrDefault(chineseText, "翻译未找到");
                // 将翻译设置到文本框2中
                textArea2.setText(englishTranslation);
            }
        });

        // 为按钮2添加监听器
        button2.addActionListener(new ActionListener() {
            // 重写actionPerformed方法
            @Override
            public void actionPerformed(ActionEvent e) {
                // 获取文本框2中的英文文本
                String englishText = textArea2.getText();
                // 获取英文文本对应的中文翻译，如果未找到则返回"翻译未找到"
                String chineseTranslation = translations.getOrDefault(englishText, "翻译未找到");
                // 将中文翻译设置到文本框1中
                textArea1.setText(chineseTranslation);
            }
        });

        // 将label1添加到frame中
        frame.add(label1);
        // 将textArea1添加到frame中
        frame.add(textArea1);
        // 将button1添加到frame中
        frame.add(button1);
        // 将label3添加到frame中
        frame.add(label3);
        // 将label4添加到frame中
        frame.add(label4);
        // 将label5添加到frame中
        frame.add(label5);
        // 将label2添加到frame中
        frame.add(label2);  
        // 将textArea2添加到frame中
        frame.add(textArea2);
        // 将button2添加到frame中
        frame.add(button2);


// 设置窗口可见
        frame.setVisible(true);
    }
}
