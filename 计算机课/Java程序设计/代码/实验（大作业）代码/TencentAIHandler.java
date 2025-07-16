package com.chino;

import com.tencentcloudapi.common.AbstractModel;
import com.tencentcloudapi.common.Credential;
import com.tencentcloudapi.common.SSEResponseModel;
import com.tencentcloudapi.common.exception.TencentCloudSDKException;
import com.tencentcloudapi.common.profile.ClientProfile;
import com.tencentcloudapi.common.profile.HttpProfile;
import com.tencentcloudapi.hunyuan.v20230901.HunyuanClient;
import com.tencentcloudapi.hunyuan.v20230901.models.ChatCompletionsRequest;
import com.tencentcloudapi.hunyuan.v20230901.models.ChatCompletionsResponse;
import com.tencentcloudapi.hunyuan.v20230901.models.Message;

public class TencentAIHandler {
    public static String callTencentCloudAPI(String content) {
        try {
            // 创建Credential对象，包含腾讯云的API密钥和密钥ID
            Credential cred = new Credential("**", "**");
            HttpProfile httpProfile = new HttpProfile(); // 创建HttpProfile对象
            httpProfile.setEndpoint("hunyuan.tencentcloudapi.com"); // 设置API的终端地址
            ClientProfile clientProfile = new ClientProfile(); 
            clientProfile.setHttpProfile(httpProfile);
            HunyuanClient client = new HunyuanClient(cred, "ap-guangzhou", clientProfile); 
            ChatCompletionsRequest req = new ChatCompletionsRequest(); 
            req.setModel("hunyuan-lite"); // 设置模型为"hunyuan-lite"
            Message message = new Message(); 
            message.setRole("user"); 
            message.setContent(content); 
            req.setMessages(new Message[]{message}); 
            ChatCompletionsResponse resp = client.ChatCompletions(req);
            String responseContent; 
            if (resp.isStream()) { 
                StringBuilder result = new StringBuilder(); // 创建StringBuilder对象用于拼接响应数据
                for (SSEResponseModel.SSE e : resp) { 
                    result.append(e.Data); // 将每个数据片段拼接到result中
                }
                responseContent = result.toString(); // 将拼接后的结果转换为字符串
            } else {
                responseContent = AbstractModel.toJsonString(resp); // 将非流式响应转换为JSON字符串
            }
            int start = responseContent.indexOf("\"Content\":\"") + 11; // 查找响应内容中"Content"字段的起始位置
            int end = responseContent.indexOf("\"", start); // 查找响应内容中"Content"字段的结束位置
            String extractedContent = responseContent.substring(start, end); // 提取"Content"字段的内容
            extractedContent = extractedContent.replace("\\n", "\n").replaceAll("\\*\\*", ""); // 去除Markdown语法
            return extractedContent; // 返回提取后的内容
        } catch (TencentCloudSDKException e) { 
            System.err.println(e.toString()); 
            return e.toString(); 
        }
    }
}