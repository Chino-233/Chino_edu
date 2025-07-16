###############################################################
# Driverlib.pm - Perl驱动程序的辅助函数包
# 
# 版权所有 (c) 2005 David R. O'Hallaron, 保留所有权利。
###############################################################

package Driverlib;

use Socket;

# 带有实验特定常量的自动生成头文件
use lib ".";
use Driverhdrs;

require Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(
             driver_post
             );

use strict;

#####
# 公共函数
#

#
# driver_post - 这是驱动程序在需要将自动结果字符串传输到结果服务器时调用的例程。
#
sub driver_post ($$) {
    my $userid = shift;       # 此提交的用户ID
    my $result = shift;       # 自动结果字符串
    my $autograded = shift;   # 如果由自动评分器调用则设置

    # 如果驱动程序是由自动评分器调用的，则将自动结果字符串回显到stdout
    if ($autograded) {
        print "\n";
        print "AUTORESULT_STRING=$result\n";
        return;
    }    

    # 如果驱动程序使用特定的userid调用，则通过互联网向结果服务器提交
    # 自动结果字符串。
    if ($userid) {
        my $status = submitr($Driverhdrs::SERVER_NAME, 
                             $Driverhdrs::SERVER_PORT, 
                             $Driverhdrs::COURSE_NAME, 
                             $userid, 
                             $Driverhdrs::LAB, 
                             $result);
        
        # 打印传输的状态
        if (!($status =~ /OK/)) {
            print "$status\n";
            print "未向结果服务器发送自动结果字符串。\n";
            exit(1);
        }
        print "成功: 向结果服务器发送了 $userid 的自动结果字符串。\n";
    }    
}


#####
# 私有函数
#

#
# submitr - 向结果服务器发送自动结果字符串
#
sub submitr ($$$$$$) {
    my $hostname = shift;
    my $port = shift;
    my $course = shift;
    my $userid = shift;
    my $lab = shift;
    my $result = shift;

    my $internet_addr;
    my $enc_result;
    my $paddr;
    my $line;
    my $http_version;
    my $errcode;
    my $errmsg;

    # 建立与服务器的连接
    socket(SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp'));
    $internet_addr = inet_aton($hostname)
        or die "无法将 $hostname 转换为互联网地址: $!\n";
    $paddr = sockaddr_in($port, $internet_addr);
    connect(SERVER, $paddr)
        or die "无法连接到 $hostname:$port:$!\n";

    select((select(SERVER), $| = 1)[0]); # 启用命令缓冲

    # 向服务器发送HTTP请求
    $enc_result = url_encode($result);
    print SERVER  "GET /$course/submitr.pl/?userid=$userid&lab=$lab&result=$enc_result&submit=submit HTTP/1.0\r\n\r\n";

    # 获取第一个HTTP响应行
    $line = <SERVER>;
    chomp($line);
    ($http_version, $errcode, $errmsg) = split(/\s+/, $line);
    if ($errcode != 200) {
        return "错误: HTTP请求失败，错误 $errcode: $errmsg";
    }

    # 读取剩余的HTTP响应头行
    while ($line = <SERVER>) {
        if ($line =~ /^\r\n/) {
            last;
        }
    }

    # 读取并返回结果服务器的响应
    $line = <SERVER>;
    chomp($line);

    close SERVER;
    return $line;
    
}

#
# url_encode - 编码文本字符串，使其可以包含在GET请求的URI中
#
sub url_encode ($) {
    my $value = shift;

    $value =~s/([^a-zA-Z0-9_\-.])/uc sprintf("%%%02x",ord($1))/eg;
    return $value;
}

# 总是以1结束模块，使其返回TRUE
1;