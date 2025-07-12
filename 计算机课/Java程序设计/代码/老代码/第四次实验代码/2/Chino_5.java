

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

import com.tencentcloudapi.common.SSEResponseModel;
import com.tencentcloudapi.common.AbstractModel;
import com.tencentcloudapi.common.Credential;
import com.tencentcloudapi.common.profile.ClientProfile;
import com.tencentcloudapi.common.profile.HttpProfile;
import com.tencentcloudapi.common.exception.TencentCloudSDKException;
import com.tencentcloudapi.hunyuan.v20230901.HunyuanClient;
import com.tencentcloudapi.hunyuan.v20230901.models.*;

public class Chino_5 {
    public static void main(String[] args) {
        // 创建一个JFrame窗口，设置窗口标题为"腾讯混元大模型对话 Demo"
        JFrame frame = new JFrame("腾讯混元大模型对话 Demo");
        // 设置窗口关闭时的默认操作为退出程序
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // 设置窗口大小为700x400
        frame.setSize(700, 400);
        // 设置窗口布局为3行3列的网格布局
        frame.setLayout(new GridLayout(3, 3));

        // 创建一个JLabel标签，显示文本"对话输入框~"
        JLabel label1 = new JLabel("对话输入框~");
        // 创建一个JTextArea文本域，用于显示对话内容
        JTextArea textArea1 = new JTextArea();
        // 设置文本域自动换行
        textArea1.setLineWrap(true);
        // 设置文本域在换行时保持单词的完整性
        textArea1.setWrapStyleWord(true);
        // 创建一个JButton按钮，显示文本"对话"
        JButton button1 = new JButton("对话");

        // 创建一个标签，显示文本"AI回答:"
        JLabel label2 = new JLabel("AI回答:");
        // 创建一个文本区域，用于显示AI的回答
        JTextArea textArea2 = new JTextArea();
        // 设置文本区域自动换行
        textArea2.setLineWrap(true);
        // 设置文本区域在换行时保持单词的完整性
        textArea2.setWrapStyleWord(true);
        // 创建一个按钮，用于触发对话
        JButton button2 = new JButton("");
        // 创建一个标签，显示文本"这里是分界线~"
        JLabel label3 = new JLabel("这里是分界线~");
        // 创建一个标签，显示文本"这里是分界线~"
        JLabel label4 = new JLabel("这里是分界线~");
        // 创建一个标签，显示文本"请点击按钮进行对话~"
        JLabel label5 = new JLabel("请点击按钮进行对话~");
        // 为按钮添加一个监听器，当按钮被点击时触发事件
        // 为按钮1添加监听器
        button1.addActionListener(new ActionListener() {
            // 重写actionPerformed方法
            @Override
            public void actionPerformed(ActionEvent e) {
                // 获取文本框1中的文本
                String inputText = textArea1.getText();
                // 调用腾讯云API，获得输出文本
                String responseText = callTencentCloudAPI(inputText);
                // 将输出文本设置到文本框2中
                textArea2.setText(responseText);
            }
        });
        // 将标签、文本域和按钮添加到窗口中
        frame.add(label1);
        frame.add(textArea1);
        frame.add(button1);
        frame.add(label3);
        frame.add(label4);
        frame.add(label5);
        frame.add(label2);  
        frame.add(textArea2);
        //frame.add(button2);

// 设置窗口可见
        frame.setVisible(true);
    }

    // 调用腾讯混元大模型API进行对话
    //以下大部分为腾讯云提供的模板
    private static String callTencentCloudAPI(String content) {
        //传入所问内容
        try {
            Credential cred = new Credential("**", "**");
            HttpProfile httpProfile = new HttpProfile();
            httpProfile.setEndpoint("hunyuan.tencentcloudapi.com");
            ClientProfile clientProfile = new ClientProfile();
            clientProfile.setHttpProfile(httpProfile);
            HunyuanClient client = new HunyuanClient(cred, "ap-guangzhou", clientProfile);
            ChatCompletionsRequest req = new ChatCompletionsRequest();
            req.setModel("hunyuan-lite");
            Message message = new Message();
            message.setRole("user");
            message.setContent(content);
            req.setMessages(new Message[]{message});
            ChatCompletionsResponse resp = client.ChatCompletions(req);
            String responseContent;
            if (resp.isStream()) {
                StringBuilder result = new StringBuilder();
                for (SSEResponseModel.SSE e : resp) {
                    result.append(e.Data);
                }
                responseContent = result.toString();
            } else {
                responseContent = AbstractModel.toJsonString(resp);
            }
            // 提取"Content"字段后的文字
            int start = responseContent.indexOf("\"Content\":\"") + 11;
            int end = responseContent.indexOf("\"", start);
            String extractedContent = responseContent.substring(start, end);
            // 替换\n为正常的换行符，并去除其他符号
            extractedContent = extractedContent.replace("\\n", "\n").replaceAll("\\*\\*", "");
            return extractedContent;
        } catch (TencentCloudSDKException e) {
            //System.err.println(e.toString());
            return e.toString();
            //返回回答内容
        }
    }
}