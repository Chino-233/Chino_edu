

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextArea;

import com.alibaba.dashscope.aigc.generation.Generation;
import com.alibaba.dashscope.aigc.generation.GenerationParam;
import com.alibaba.dashscope.aigc.generation.GenerationResult;
import com.alibaba.dashscope.common.Message;
import com.alibaba.dashscope.common.Role;
import com.alibaba.dashscope.exception.ApiException;
import com.alibaba.dashscope.exception.InputRequiredException;
import com.alibaba.dashscope.exception.NoApiKeyException;

public class Chino_3 {
    public static void main(String[] args) {
        // 创建一个 JFrame 对象，设置标题为 "通义千问对话 Demo"
        JFrame frame = new JFrame("通义千问对话 Demo");
        // 设置关闭窗口时退出程序
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // 设置窗口大小为 700x400
        frame.setSize(700, 400);
        // 设置窗口布局为 3x3 的网格布局
        frame.setLayout(new GridLayout(3, 3));

        // 创建一个标签，显示文本“对话输入框~”
        JLabel label1 = new JLabel("对话输入框~");
        // 创建一个文本区域，用于输入对话内容
        JTextArea textArea1 = new JTextArea();
        // 设置文本区域自动换行
        textArea1.setLineWrap(true);
        // 设置文本区域在换行时保持单词的完整性
        textArea1.setWrapStyleWord(true);
        JButton button1 = new JButton("对话");

        // 创建一个标签，显示文本"AI回答:"
        JLabel label2 = new JLabel("AI回答:");
        // 创建一个文本区域，用于显示AI的回答
        JTextArea textArea2 = new JTextArea();
        // 设置文本区域自动换行
        textArea2.setLineWrap(true);
        // 设置文本区域在换行时保持单词的完整性
        textArea2.setWrapStyleWord(true);
        // 创建一个按钮，用于进行对话
        JButton button2 = new JButton("");
        // 创建一个标签，显示文本"这里是分界线~"
        JLabel label3 = new JLabel("这里是分界线~");
        // 创建一个标签，显示文本"这里是分界线~"
        JLabel label4 = new JLabel("这里是分界线~");
        // 创建一个标签，显示文本"请点击按钮进行对话~"
        JLabel label5 = new JLabel("请点击按钮进行对话~");
        // 为按钮1添加监听器
        button1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String chineseText = textArea1.getText();
                String englishTranslation = "";
                    try {
                        englishTranslation = callWithMessage(chineseText, "zh", "en");
                    } catch (Exception ex) {
                        ex.printStackTrace();
                        englishTranslation = "对话失败,请更换对话内容";
                    }
                textArea2.setText(englishTranslation);
            }
        });

        frame.add(label1);
        // 添加标签1到框架中
        frame.add(textArea1);
        // 添加文本区域1到框架中
        frame.add(button1);
        // 添加按钮1到框架中
        frame.add(label3);
        // 添加标签3到框架中
        frame.add(label4);
        // 添加标签4到框架中
        frame.add(label5);
        // 添加标签5到框架中
        frame.add(label2);
        // 添加标签2到框架中
        frame.add(textArea2);
        // 添加文本区域2到框架中
        //frame.add(button2);

        frame.setVisible(true);
    }
    //调用通义千问API进行对话
    //以下大部分为通义千问提供的模板
    private static String callWithMessage(String content, String from, String to) throws ApiException, NoApiKeyException, InputRequiredException {
        Generation gen = new Generation();
        Message systemMsg = Message.builder()
                .role(Role.SYSTEM.getValue())
                .content("You are a helpful assistant.")
                .build();
        Message userMsg = Message.builder()
                .role(Role.USER.getValue())
                .content(content)//传入对话内容
                .build();
        GenerationParam param = GenerationParam.builder()
                .apiKey(System.getenv("DASHSCOPE_API_KEY"))//传入API_KEY
                .model("qwen-plus")
                .messages(Arrays.asList(systemMsg, userMsg))
                .resultFormat(GenerationParam.ResultFormat.MESSAGE)
                .build();
        GenerationResult result = gen.call(param);
        return result.getOutput().getChoices().get(0).getMessage().getContent();//返回回答内容
    }
}

