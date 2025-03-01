package com.chino;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Chino {
    public static void main(String[] args) {
        String message = "上述消息提到，4月27日晚举行的深圳大学40周年校庆捐赠仪式暨“海岸之声”音乐晚会上，多家企业向深圳大学40周年校庆进行捐赠。明礼德教育科技集团有限公司向深圳大学捐赠1000万元；心里程控股集团向深圳大学捐赠1亿元；工勘岩土集团捐赠4000万元；正中集团捐赠5000万元；海岸集团捐赠6000万元；腾讯公益慈善基金会捐赠2亿元。此前，正中集团已向深大捐赠4700万元，海岸集团已向深大捐赠2200万元，腾讯创始人校友团队和腾讯公益慈善基金会已向深大捐赠3.9亿元。除此之外，平安集团捐赠5000万元，点维文化传播捐赠1000万元，叶晓彬校友捐赠1000万元，已于日前完成相关签约。";

        // 使用正则表达式匹配所有的数字
        Pattern pattern = Pattern.compile("\\d+(\\.\\d+)?");
        Matcher matcher = pattern.matcher(message);

        double totalDonation = 0;

        while (matcher.find()) {
            String numberStr = matcher.group();
            double number = Double.parseDouble(numberStr);
            totalDonation += number;
        }

        System.out.println("总金额: " + totalDonation + "万元");
    }
}
