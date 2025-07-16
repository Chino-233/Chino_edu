package com.chino;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

public class Chino {
    private static int chatCount = 0; // 定义静态变量chatCount，用于记录对话次数
    private static Map<String, ChatPanel> chatPanels = new HashMap<>(); // 定义静态变量chatPanels，用于存储对话面板
    private static Map<String, String> chatModels = new HashMap<>(); // 定义静态变量chatModels，用于存储对话模型

    public static void main(String[] args) { // 主方法
        JFrame frame = new JFrame("Java知识问答机器人"); // 创建一个JFrame窗口，标题为"Java知识问答机器人"
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 设置窗口关闭操作为退出程序
        frame.setSize(800, 600); // 设置窗口大小为800x600
        frame.setLayout(new BorderLayout()); // 设置窗口布局为BorderLayout

        JPanel leftPanel = new JPanel(); // 创建左侧面板
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS)); // 设置左侧面板布局为垂直BoxLayout
        JButton newChatButton = new JButton("添加新对话"); // 创建一个按钮，文本为"添加新对话"
        setButtonSize(newChatButton); // 设置按钮大小
        leftPanel.add(newChatButton); // 将按钮添加到左侧面板

        JPanel rightPanel = new JPanel(); // 创建右侧面板
        rightPanel.setLayout(new CardLayout()); // 设置右侧面板布局为CardLayout

        newChatButton.addActionListener(new ActionListener() { // 为"添加新对话"按钮添加事件监听器
            @Override
            public void actionPerformed(ActionEvent e) { // 当按钮被点击时执行的操作
                String[] models = {"腾讯混元", "阿里通义千问"}; // 定义模型数组
                String selectedModel = (String) JOptionPane.showInputDialog(frame, "选择大模型", "选择大模型", JOptionPane.QUESTION_MESSAGE, null, models, models[0]); // 弹出对话框让用户选择模型
                if (selectedModel != null) { // 如果用户选择了模型
                    chatCount++; // 对话次数加1
                    String chatKey = "第" + chatCount + "次对话 " + selectedModel; // 生成对话键
                    ChatPanel chatPanel = new ChatPanel(selectedModel); // 创建新的对话面板
                    chatPanels.put(chatKey, chatPanel); // 将对话面板存储到chatPanels中
                    chatModels.put(chatKey, selectedModel); // 将对话模型存储到chatModels中
                    rightPanel.add(chatPanel, chatKey); // 将对话面板添加到右侧面板
                    JButton chatButton = new JButton(chatKey); // 创建新的按钮，文本为对话键
                    setButtonSize(chatButton); // 设置按钮大小
                    leftPanel.add(chatButton); // 将按钮添加到左侧面板
                    leftPanel.revalidate(); // 重新验证左侧面板
                    leftPanel.repaint(); // 重新绘制左侧面板
                    chatButton.addActionListener(new ActionListener() { // 为新的按钮添加事件监听器
                        @Override
                        public void actionPerformed(ActionEvent e) { // 当按钮被点击时执行的操作
                            CardLayout cl = (CardLayout) (rightPanel.getLayout()); // 获取右侧面板的CardLayout
                            cl.show(rightPanel, chatKey); // 显示对应的对话面板
                            chatPanel.setModel(chatModels.get(chatKey)); // 设置对话面板的模型
                        }
                    });
                    CardLayout cl = (CardLayout) (rightPanel.getLayout()); // 获取右侧面板的CardLayout
                    cl.show(rightPanel, chatKey); // 显示对应的对话面板
                }
            }
        });

        frame.add(leftPanel, BorderLayout.WEST); // 将左侧面板添加到窗口的左侧
        frame.add(rightPanel, BorderLayout.CENTER); // 将右侧面板添加到窗口的中间
        frame.setVisible(true); // 设置窗口可见
    }

    private static void setButtonSize(JButton button) { // 设置按钮大小的方法
        Dimension size = new Dimension(200, 50); // 定义按钮大小为200x50
        button.setPreferredSize(size); // 设置按钮的首选大小
        button.setMaximumSize(size); // 设置按钮的最大大小
        button.setMinimumSize(size); // 设置按钮的最小大小
    }
}
