package com.chino;

import java.util.Arrays;

import com.alibaba.dashscope.aigc.generation.Generation;
import com.alibaba.dashscope.aigc.generation.GenerationParam;
import com.alibaba.dashscope.aigc.generation.GenerationResult;
import com.alibaba.dashscope.common.Message;
import com.alibaba.dashscope.common.Role;
import com.alibaba.dashscope.exception.ApiException;
import com.alibaba.dashscope.exception.InputRequiredException;
import com.alibaba.dashscope.exception.NoApiKeyException;

public class AlibabaAIHandler {
    // 定义一个静态方法callWithMessage，接受三个字符串参数，并抛出三种异常
    public static String callWithMessage(String content, String from, String to) throws ApiException, NoApiKeyException, InputRequiredException {
        Generation gen = new Generation(); 
        Message systemMsg = new Message(); 
        systemMsg.setRole(Role.SYSTEM.getValue());
        systemMsg.setContent("You are a helpful assistant."); 

        Message userMsg = new Message();
        userMsg.setRole(Role.USER.getValue()); 
        userMsg.setContent(content);
        GenerationParam param = GenerationParam.builder()
                .apiKey(System.getenv("DASHSCOPE_API_KEY")) // 从环境变量中获取API密钥
                .model("qwen-plus") // 设置模型名称
                .messages(Arrays.asList(systemMsg, userMsg)) // 设置消息列表
                .resultFormat(GenerationParam.ResultFormat.MESSAGE) // 设置结果格式
                .build();
        GenerationResult result = gen.call(param); // 调用生成方法并获取结果
        // 获取生成结果的内容，并去除Markdown格式
        String output = result.getOutput().getChoices().get(0).getMessage().getContent()
                .replaceAll("[*`_~#]+", "");
        System.out.println(output); // 输出结果
        return output; // 返回结果
    }
}
