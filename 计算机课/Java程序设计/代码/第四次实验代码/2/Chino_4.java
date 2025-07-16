

import com.tencentcloudapi.common.SSEResponseModel;
import com.tencentcloudapi.common.AbstractModel;
import com.tencentcloudapi.common.Credential;
import com.tencentcloudapi.common.profile.ClientProfile;
import com.tencentcloudapi.common.profile.HttpProfile;
import com.tencentcloudapi.common.exception.TencentCloudSDKException;
import com.tencentcloudapi.hunyuan.v20230901.HunyuanClient;
import com.tencentcloudapi.hunyuan.v20230901.models.*;

public class Chino_4
{
    public static void main(String [] args) {
        try{
            Credential cred = new Credential("**", "**");
            HttpProfile httpProfile = new HttpProfile();
            httpProfile.setEndpoint("hunyuan.tencentcloudapi.com");
            ClientProfile clientProfile = new ClientProfile();
            clientProfile.setHttpProfile(httpProfile);
            HunyuanClient client = new HunyuanClient(cred, "ap-guangzhou", clientProfile);
            ChatCompletionsRequest req = new ChatCompletionsRequest();
            
            // 设置模型参数
            req.setModel("hunyuan-lite"); // 请将"模型名称"替换为实际的模型名称
            
            // 设置对话内容
            Message message = new Message();
            message.setRole("user");
            message.setContent("香风智乃是谁");
            req.setMessages(new Message[]{message});
            
            // 返回的resp是一个ChatCompletionsResponse的实例，与请求对象对应
            ChatCompletionsResponse resp = client.ChatCompletions(req);
            // 输出json格式的字符串回包
            if (resp.isStream()) { // 流式响应
                for (SSEResponseModel.SSE e : resp) {
                    System.out.println(e.Data);
                }
            } else { // 非流式响应
                System.out.println(AbstractModel.toJsonString(resp));
            }
        } catch (TencentCloudSDKException e) {
            System.out.println(e.toString());
        }
    }
}