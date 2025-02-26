package com.chino;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

// 定义一个CSRankings类，用于存储研究方向和对应的会议信息
class CSRankings {
    // 研究方向
    private String researchDirection;
    // 会议信息
    private Map<String, String[]> conferenceMap;

    // 构造函数，传入研究方向，初始化会议信息
    public CSRankings(String researchDirection) {
        this.researchDirection = researchDirection;
        conferenceMap = new HashMap<>();
        initializeConferenceMap();
    }

    // 初始化会议信息
    private void initializeConferenceMap() {
        conferenceMap.put("Artificial Intelligence", new String[]{
            "AAAI dblp.org/db/conf/aaai/index.html",
            "IJCAI dblp.org/db/conf/ijcai/index.html"
        });
        conferenceMap.put("Computer Vision", new String[]{
            "CVPR dblp.org/db/conf/cvpr/index.html",
            "ECCV dblp.org/db/conf/eccv/index.html",
            "ICCV dblp.org/db/conf/iccv/index.html"
        });
        conferenceMap.put("Machine Learning & Data Mining", new String[]{
            "ICML dblp.org/db/conf/icml/index.html",
            "KDD dblp.org/db/conf/kdd/index.html",
            "NeurIPS dblp.org/db/conf/nips/index.html"
        });
        conferenceMap.put("Natural Language Processing", new String[]{
            "ACL dblp.org/db/conf/acl/index.html",
            "EMNLP dblp.org/db/conf/emnlp/index.html",
            "NAACL dblp.org/db/conf/naacl/index.html"
        });
        conferenceMap.put("The Web & Information Retrieval", new String[]{
            "SIGIR dblp.org/db/conf/sigir/index.html",
            "WWW dblp.org/db/conf/www/index.html"
        });
        conferenceMap.put("Computer Architecture", new String[]{
            "ASPLOS dblp.org/db/conf/asplos/index.html",
            "ISCA dblp.org/db/conf/isca/index.html",
            "MICRO dblp.org/db/conf/micro/index.html",
            "HPCA dblp.org/db/conf/hpca/index.html"
        });
        conferenceMap.put("Computer Networks", new String[]{
            "NSDI dblp.org/db/conf/nsdi/index.html",
            "SIGCOMM dblp.org/db/conf/sigcomm/index.html"
        });
        conferenceMap.put("Computer Security", new String[]{
            "CCS dblp.org/db/conf/ccs/index.html",
            "S&P dblp.org/db/conf/sp/index.html",
            "USENIXSEC dblp.org/db/conf/uss/index.html",
            "NDSS dblp.org/db/conf/ndss/index.html",
            "PETS dblp.org/db/conf/pet/index.html"
        });
        conferenceMap.put("Databases", new String[]{
            "VLDB dblp.org/db/conf/vldb/index.html",
            "SIGMOD dblp.org/db/conf/sigmod/index.html",
            "ICDE dblp.org/db/conf/icde/index.html",
            "PODS dblp.org/db/conf/pods/index.html"
        });
        conferenceMap.put("Design Automation", new String[]{
            "DAC dblp.org/db/conf/dac/index.html",
            "ICCAD dblp.org/db/conf/iccad/index.html"
        });
        conferenceMap.put("Embedded & Real-Time Systems", new String[]{
            "EMSOFT dblp.org/db/conf/emsoft/index.html",
            "RTAS dblp.org/db/conf/rtas/index.html",
            "RTSS dblp.org/db/conf/rtss/index.html"
        });
        conferenceMap.put("High-Performance Computing", new String[]{
            "SC dblp.org/db/conf/sc/index.html",
            "HPDC dblp.org/db/conf/hpdc/index.html",
            "ICS dblp.org/db/conf/ics/index.html"
        });
        conferenceMap.put("Mobile Computing", new String[]{
            "MOBICOM dblp.org/db/conf/mobicom/index.html",
            "MOBISYS dblp.org/db/conf/mobisys/index.html",
            "SENSYS dblp.org/db/conf/sensys/index.html"
        });
        conferenceMap.put("Measurement & Perf. Analysis", new String[]{
            "IMC dblp.org/db/conf/imc/index.html",
            "SIGMETRICS dblp.org/db/conf/sigmetrics/index.html"
        });
        conferenceMap.put("Operating Systems", new String[]{
            "OSDI dblp.org/db/conf/osdi/index.html",
            "SOSP dblp.org/db/conf/sosp/index.html",
            "EUROSYS dblp.org/db/conf/eurosys/index.html",
            "FAST dblp.org/db/conf/fast/index.html",
            "USENIXATC dblp.org/db/conf/usenix/index.html"
        });
        conferenceMap.put("Programming Languages", new String[]{
            "POPL dblp.org/db/conf/popl/index.html",
            "PLDI dblp.org/db/conf/pldi/index.html",
            "OOPSLA dblp.org/db/conf/oopsla/index.html",
            "ICFP dblp.org/db/conf/icfp/index.html"
        });
        conferenceMap.put("Software Engineering", new String[]{
            "FSE dblp.org/db/conf/sigsoft/index.html",
            "ICSE dblp.org/db/conf/icse/index.html",
            "ASE dblp.org/db/conf/kbse/index.html",
            "ISSTA dblp.org/db/conf/issta/index.html"
        });
        conferenceMap.put("Algorithms & Complexity", new String[]{
            "FOCS dblp.org/db/conf/focs/index.html",
            "SODA dblp.org/db/conf/soda/index.html",
            "STOC dblp.org/db/conf/stoc/index.html"
        });
        conferenceMap.put("Cryptography", new String[]{
            "CRYPTO dblp.org/db/conf/crypto/index.html",
            "EUROCRYPT dblp.org/db/conf/eurocrypt/index.html"
        });
        conferenceMap.put("Logic & Verification", new String[]{
            "CAV dblp.org/db/conf/cav/index.html",
            "LICS dblp.org/db/conf/lics/index.html"
        });
        conferenceMap.put("Comp. Bio & Bioinformatics", new String[]{
            "ISMB dblp.org/db/conf/ismb/index.html",
            "RECOMB dblp.org/db/conf/recomb/index.html"
        });
        conferenceMap.put("Computer Graphics", new String[]{
            "SIGGRAPH dblp.org/db/conf/siggraph/index.html",
            "SIGGRAPH ASIA dblp.org/db/conf/siggrapha/index.html"
        });
        conferenceMap.put("Economics & Computation", new String[]{
            "EC dblp.org/db/conf/sigecom/index.html",
            "WINE dblp.org/db/conf/wine/index.html"
        });
        conferenceMap.put("Human-Computer Interaction", new String[]{
            "CHI dblp.org/db/conf/chi/index.html",
            "UBICOMP dblp.org/db/conf/huc/index.html",
            "UIST dblp.org/db/conf/uist/index.html"
        });
        conferenceMap.put("Robotics", new String[]{
            "ICRA dblp.org/db/conf/icra/index.html",
            "IROS dblp.org/db/conf/iros/index.html",
            "RSS dblp.org/db/conf/rss/index.html"
        });
        conferenceMap.put("Visualization", new String[]{
            "VIS dblp.org/db/conf/visualization/index.html",
            "VR dblp.org/db/conf/vr/index.html"
        });
    }

    public void printConferenceInfo() {
        // 打印研究方向
        System.out.println("Research Direction: " + researchDirection);
        // 如果研究方向在conferenceMap中存在
        if (conferenceMap.containsKey(researchDirection)) {
            // 遍历conferenceMap中对应研究方向的所有会议
            for (String conference : conferenceMap.get(researchDirection)) {
                // 将会议字符串按空格分割
                String[] parts = conference.split(" ");
                // 打印会议名称和网址
                System.out.println("会议名称: " + parts[0] + " 网址: " + parts[1]);
            }
        } else {
            // 如果研究方向不存在，打印提示信息
            System.out.println("研究方向不存在");
        }
    }
}

public class Chino {
    public static void main(String[] args) {
        // 创建Scanner对象，用于接收用户输入
        Scanner input = new Scanner(System.in);
        // 提示用户输入研究方向
        System.out.println("请输入研究方向：");
        // 接收用户输入的研究方向
        String researchDirection = input.nextLine();
        // 创建CSRankings对象，传入研究方向
        CSRankings csRankings = new CSRankings(researchDirection);
        // 调用printConferenceInfo方法，打印会议信息
        csRankings.printConferenceInfo();
    }
}
