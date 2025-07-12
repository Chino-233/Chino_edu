package com.chino;

import java.util.*;

public class Chino {
    public static void main(String[] args) {
        // 1. 读取网页内容到字符串
        // 实际输入不完全展示
        String content = "e Computer Science and Technology discipline at Shenzhen University was founded with support from Tsinghua University in 1983 when the Shenzhen University was just established. College of Computer Science and Software Engineering was formally established in 2008, which was headed by Guoliang Chen, the Academician of the Chinese Academy of Sciences, as its founding Dean. Over the years' development, CSSE has now led by Professor Hui Huang and become a prominent college in education and research nationally and globally.\r\n" + //
                        "\r\n" + //
                        " \r\n" + //
                        "\r\n" + //
                        "The platforms include two national-level research platforms (National Engineering Laboratory for the Big Data System Computing Technology, MOE-GD Collaborative Innovation Center for GD-HK Modern Information Service), three national-level teaching platforms (Computer Experimental Teaching Center, Virtual Simulation Experimental Center for Network Engineering, and Tencent Cloud AI College), the Guangdong Laboratory of Artificial Intelligence and Digital Economy (Shenzhen), ten other provincial-level and ten municipal-level platforms.\r\n" + //
                        "\r\n" + //
                        "National Research Platform\r\n" + //
                        "National Engineering Laboratory for Big Data System Computing Technology\r\n" + //
                        "\r\n" + //
                        "MOE-GD Collaborative Innovation Center for GD-HK Modern Information Service\r\n" + //
                        "\r\n" + //
                        "National Teaching Platforms\r\n" + //
                        "Computer Experimental Teaching Center\r\n" + //
                        "(National Computer Experimental Demonstration Center)\r\n" + //
                        "\r\n" + //
                        "Virtual Simulation Experimental Center for Network Engineering\r\n" + //
                        "(National Virtual Simulation Experimental Center)\r\n" + //
                        "\r\n" + //
                        "Tencent Cloud AI College\r\n" + //
                        "\r\n" + //
                        "Municipal Research Platforms\r\n" + //
                        "Shenzhen Institute of Computing Sciences\r\n" + //
                        "\r\n" + //
                        "Shenzhen City Key Laboratory of Embedded System Design\r\n" + //
                        "\r\n" + //
                        "Shenzhen Key Laboratory of Service Computing and Application\r\n" + //
                        "\r\n" + //
                        "Shenzhen Key Laboratory of Intelligent Medical Diagnosis\r\n" + //
                        "\r\n" + //
                        "Shenzhen Science and Technology Innovation Resource Sharing Technology Service Platform\r\n" + //
                        "\r\n" + //
                        "Shenzhen Engineering Laboratory of Middleware Technology for Mobile Internet Application\r\n" + //
                        "\r\n" + //
                        "Shenzhen Laboratory of IC Design for Internet of Things\r\n" + //
                        "\r\n" + //
                        "Shenzhen Engineering Laboratory of Big Data Intelligence Information Processing\r\n" + //
                        "\r\n" + //
                        "Shenzhen Public Technology Service Platform for Multimedia and Virtual Reality\r\n" + //
                        "\r\n" + //
                        "Shenzhen Key Laboratory of Digitalization Technology and Systems\r\n" + //
                        "\r\n" + //
                        "Provincial Research Platforms\r\n" + //
                        "Guangdong Laboratory of Artificial Intelligence and Digital Economy (SZ)\r\n" + //
                        "\r\n" + //
                        "Guangdong Provincial Key Laboratory of Popular High Performance Computers\r\n" + //
                        "\r\n" + //
                        "Laboratory of AI Supported Brain Media\r\n" + //
                        "\r\n" + //
                        "Big Data Analysis Engineering Technology Research Center of Universities in Guangdong\r\n" + //
                        "\r\n" + //
                        "China-UK Joint Research Lab on Visual Information Processing\r\n" + //
                        "\r\n" + //
                        "Guangdong Provincial Mobile Internet Application Middleware Engineering Technology Research Center\r\n" + //
                        "\r\n" + //
                        "Maternal Child Health Monitoring and Early Warning Engineering Technology Research Center of Guangdong Province\r\n" + //
                        "\r\n" + //
                        "The Guangdong Wireless Big Data and Future Network Research Center\r\n" + //
                        "\r\n" + //
                        "Guangdong Provincial Engineering Center of China-Made High Performance Data Computing System\r\n" + //
                        "\r\n" + //
                        "3D Content Creation Engineering Technology Research Center of Guangdong Pr";
        
        // 2. 将字符串分割成单词
        String[] words = content.toLowerCase().split("\\W+");
        
        // 3. 统计每个单词的出现次数
        Map<String, Integer> wordCount = new HashMap<>();
        for (String word : words) {
            wordCount.put(word, wordCount.getOrDefault(word, 0) + 1);
        }
        
        // 4. 按出现次数排序单词
        List<Map.Entry<String, Integer>> sortedWords = new ArrayList<>(wordCount.entrySet());
        sortedWords.sort((a, b) -> {
            if (a.getValue().equals(b.getValue())) {
                return a.getKey().compareTo(b.getKey());
            } else {
                return b.getValue().compareTo(a.getValue());
            }
        });
        
        // 5. 输出出现次数最多的10个单词
        for (int i = 0; i < Math.min(10, sortedWords.size()); i++) {
            System.out.println(sortedWords.get(i).getKey() + ": " + sortedWords.get(i).getValue());
        }
    }
}
