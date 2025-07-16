#!/usr/bin/perl
#######################################################################
# driver.pl - CS:APP 数据实验驱动程序
#
# 版权所有 (c) 2004-2011, R. Bryant 和 D. O'Hallaron, 保留所有权利。
# 未经许可不得使用、修改或复制。
#
# 注意：驱动程序可以使用 btest 或 BDD 检查器来检查
# 谜题的正确性。此版本的实验使用 btest，它
# 已被扩展以更好地测试整数和浮点谜题。
#
#######################################################################

use strict 'vars';
use Getopt::Std;

use lib ".";
use Driverlib;

# 设置为1使用btest，设置为0使用BDD检查器。
my $USE_BTEST = 1;

# 通用设置
$| = 1;      # 每次刷新stdout
umask(0077); # 用户在tmp中创建的文件只对该用户可读
$ENV{PATH} = "/usr/local/bin:/usr/bin:/bin";

#
# usage - 打印帮助信息并终止
#
sub usage {
    printf STDERR "$_[0]\n";
    printf STDERR "用法: $0 [-h] [-u \"昵称\"]\n";
    printf STDERR "选项:\n";
    printf STDERR "  -h              打印此消息。\n";
    printf STDERR "  -u \"昵称\"     将自动结果发送到服务器，在记分板上使用昵称\n";
    die "\n";
}

##############
# 主程序
##############
my $login = getlogin() || (getpwuid($<))[0] || "unknown";
my $tmpdir = "/var/tmp/datalab.$login.$$";
my $diemsg = "文件位于 $tmpdir。";

my $driverfiles;
my $infile;
my $autograded;

my $status;
my $inpuzzles;
my $puzzlecnt;
my $line;
my $blank;
my $name;
my $c_points;
my $c_rating;
my $c_errors;
my $p_points;
my $p_rating;
my $p_errors;
my $total_c_points;
my $total_c_rating;
my $total_p_points;
my $total_p_rating;
my $tops;
my $tpoints;
my $trating;
my $foo;
my $name;
my $msg;
my $nickname;
my $autoresult;

my %puzzle_c_points;
my %puzzle_c_rating;
my %puzzle_c_errors;
my %puzzle_p_points;
my %puzzle_p_ops;
my %puzzle_p_maxops;
my %puzzle_number;


# 解析命令行参数
no strict;
getopts('hu:f:A');
if ($opt_h) {
    usage();
}

# 默认输入文件是bits.c（用-f更改）
$infile = "bits.c";
$nickname = "";

#####
# 每个驱动程序必须支持的命令行参数
#

# 使驱动程序代表用户向服务器发送自动结果
if ($opt_u) {
    $nickname = $opt_u;
    check_nickname($nickname);
}

# 隐藏标志，表示驱动程序是由自动评分器调用的
if ($opt_A) {
    $autograded = $opt_A;
}

#####
# 驱动程序还可以定义任意数量的其他命令行参数
#
# 自动评分器使用的可选隐藏标志
if ($opt_f) {  
    $infile = $opt_f;
}

use strict 'vars';

################################################
# 计算正确性和性能得分
################################################

# 确保存在可执行的dlc（数据实验编译器）
(-e "./dlc" and -x "./dlc")
    or  die "$0: 错误: 没有可执行的dlc二进制文件。\n";


# 如果使用bdd检查器，确保它存在
if (!$USE_BTEST) {
    (-e "./bddcheck/cbit/cbit" and -x "./bddcheck/cbit/cbit")
    or  die "$0: 错误: 没有可执行的cbit二进制文件。\n";
}

#
# 设置临时目录的内容
#
system("mkdir $tmpdir") == 0
    or die "$0: 无法创建临时目录 $tmpdir。\n";

# 将学生的工作复制到临时目录
unless (system("cp $infile $tmpdir/bits.c") == 0) { 
    clean($tmpdir);
    die "$0: 无法将文件 $infile 复制到临时目录 $tmpdir。\n";
}

# 将各种自动评分文件复制到临时目录
if ($USE_BTEST) {
    $driverfiles = "Makefile dlc btest.c decl.c tests.c btest.h bits.h";
    unless (system("cp -r $driverfiles $tmpdir") == 0) {
    clean($tmpdir);
    die "$0: 无法将自动评分文件复制到 $tmpdir。\n";
    }
} 
else {
    $driverfiles = "dlc tests.c bddcheck";
    unless (system("cp -r $driverfiles $tmpdir") == 0) {
    clean($tmpdir);
    die "$0: 无法将支持文件复制到 $tmpdir。\n";
    }
}

# 将当前工作目录更改为临时目录
unless (chdir($tmpdir)) {
    clean($tmpdir);
    die "$0: 无法将目录更改为 $tmpdir。\n";
}

#
# 生成bits.c的zapped版本（用于编码规则）。在此
# zapped版本的bits.c中，任何具有非法运算符的函数都会
# 转换为具有空函数体。
#
print "1. 运行 './dlc -z' 来识别编码规则违规。\n";
system("cp bits.c save-bits.c") == 0
    or die "$0: 错误: 无法创建bits.c的备份副本。$diemsg\n";
system("./dlc -z -o zap-bits.c bits.c") == 0
    or die "$0: 错误: zapped bits.c未编译。$diemsg\n";

#
# 运行btest或BDD检查器以确定正确性得分
#
if ($USE_BTEST) {
    print "\n2. 编译并运行 './btest -g' 以确定正确性得分。\n";
    system("cp zap-bits.c bits.c");

    # 编译btest
    system("make btestexplicit") == 0
    or die "$0: 无法在 $tmpdir 中制作btest。$diemsg\n";

    # 运行btest
    $status = system("./btest -g > btest-zapped.out 2>&1");
    if ($status != 0) {
    die "$0: 错误: btest检查失败。$diemsg\n";
    }
}
else {
    print "\n2. 运行 './bddcheck/check.pl -g' 以确定正确性得分。\n";
    system("cp zap-bits.c bits.c");
    $status = system("./bddcheck/check.pl -g > btest-zapped.out 2>&1");
    if ($status != 0) {
    die "$0: 错误: BDD检查失败。$diemsg\n";
    }
}

#
# 运行dlc以识别运算符计数违规。
# 
print "\n3. 运行 './dlc -Z' 以识别运算符计数违规。\n";
system("./dlc -Z -o Zap-bits.c save-bits.c") == 0
    or die "$0: 错误: dlc无法生成Zapped bits.c文件。\n";

#
# 运行btest或bdd检查器以计算性能得分
#
if ($USE_BTEST) {
    print "\n4. 编译并运行 './btest -g -r 2' 以确定性能得分。\n";
    system("cp Zap-bits.c bits.c");

    # 编译btest
    system("make btestexplicit") == 0
    or die "$0: 无法在 $tmpdir 中制作btest。$diemsg\n";
    print "\n";

    # 运行btest
    $status = system("./btest -g -r 2 > btest-Zapped.out 2>&1");
    if ($status != 0) {
    die "$0: 错误: Zapped btest失败。$diemsg\n";
    }
}
else {
    print "\n4. 运行 './bddcheck/check.pl -g -r 2' 以确定性能得分。\n";
    system("cp Zap-bits.c bits.c");
    $status = system("./bddcheck/check.pl -g -r 2 > btest-Zapped.out 2>&1");
    if ($status != 0) {
    die "$0: 错误: Zapped bdd检查器失败。$diemsg\n";
    }
}

#
# 运行dlc以获取zapped输入文件上的运算符计数
#
print "\n5. 运行 './dlc -e' 以获取每个函数的运算符计数。\n";
$status = system("./dlc -W1 -e zap-bits.c > dlc-opcount.out 2>&1");
if ($status != 0) {
    die "$0: 错误: bits.c未编译。$diemsg\n";
}
 
#################################################################
# 收集每个谜题的正确性和性能结果
#################################################################

#
# 收集正确性结果
#
%puzzle_c_points = (); # 由btest计算的正确性得分
%puzzle_c_errors = (); # 由btest发现的正确性错误
%puzzle_c_rating = (); # 正确性谜题评级（最高分）
  
$inpuzzles = 0;      # 当我们开始读取谜题结果时变为true
$puzzlecnt = 0;      # 每个谜题获取一个唯一的编号
$total_c_points = 0;
$total_c_rating = 0; 

open(INFILE, "$tmpdir/btest-zapped.out") 
    or die "$0: 错误: 无法打开输入文件 $tmpdir/btest-zapped.out\n";

while ($line = <INFILE>) {
    chomp($line);

    # 注意我们已准备好读取谜题得分
    if ($line =~ /^Score/) {
    $inpuzzles = 1;
    next;
    }

    # 注意我们已完成读取谜题得分
    if ($line =~ /^Total/) {
    $inpuzzles = 0;
    next;
    }

    # 读取并记录谜题的名称和得分
    if ($inpuzzles) {
    ($blank, $c_points, $c_rating, $c_errors, $name) = split(/\s+/, $line);
    $puzzle_c_points{$name} = $c_points;
    $puzzle_c_errors{$name} = $c_errors;
    $puzzle_c_rating{$name} = $c_rating;
    $puzzle_number{$name} = $puzzlecnt++;
    $total_c_points += $c_points;
    $total_c_rating += $c_rating;
    }

}
close(INFILE);

#
# 收集性能结果
#
%puzzle_p_points = (); # 性能得分

$inpuzzles = 0;       # 当我们开始读取谜题结果时变为true
$total_p_points = 0;  
$total_p_rating = 0;

open(INFILE, "$tmpdir/btest-Zapped.out") 
    or die "$0: 错误: 无法打开输入文件 $tmpdir/btest-Zapped.out\n";

while ($line = <INFILE>) {
    chomp($line);

    # 注意我们已准备好读取谜题得分
    if ($line =~ /^Score/) {
    $inpuzzles = 1;
    next;
    }

    # 注意我们已完成读取谜题得分
    if ($line =~ /^Total/) {
    $inpuzzles = 0;
    next;
    }

    # 读取并记录谜题的名称和得分
    if ($inpuzzles) {
    ($blank, $p_points, $p_rating, $p_errors, $name) = split(/\s+/, $line);
    $puzzle_p_points{$name} = $p_points;
    $total_p_points += $p_points;
    $total_p_rating += $p_rating;
    }
}
close(INFILE);

#
# 收集由dlc生成的运算符计数
#
open(INFILE, "$tmpdir/dlc-opcount.out") 
    or die "$0: 错误: 无法打开输入文件 $tmpdir/dlc-opcount.out\n";

$tops = 0;
while ($line = <INFILE>) {
    chomp($line);

    if ($line =~ /(\d+) operators/) {
    ($foo, $foo, $foo, $name, $msg) = split(/:/, $line);
    $puzzle_p_ops{$name} = $1;
    $tops += $1;
    }
}
close(INFILE);

# 
# 打印按谜题编号排序的结果表
#
print "\n";
printf("%s\t%s\n", "正确性结果", "性能结果");
printf("%s\t%s\t%s\t%s\t%s\t%s\n", "分数", "评级", "错误", 
       "分数", "操作数", "谜题");
foreach $name (sort {$puzzle_number{$a} <=> $puzzle_number{$b}} 
           keys %puzzle_number) {
    printf("%d\t%d\t%d\t%d\t%d\t\%s\n", 
       $puzzle_c_points{$name},
       $puzzle_c_rating{$name},
       $puzzle_c_errors{$name},
       $puzzle_p_points{$name},
       $puzzle_p_ops{$name},
       $name);
}

$tpoints = $total_c_points + $total_p_points;
$trating = $total_c_rating + $total_p_rating;

print "\n得分 = $tpoints/$trating [$total_c_points/$total_c_rating 正确性 + $total_p_points/$total_p_rating 性能] ($tops 总操作数)\n";

#
# 如果驱动程序使用-u命令行标志调用，则可选地将自动结果发送到比赛服务器。
#
if ($nickname) {
    # 生成自动结果
    $autoresult = "$tpoints|$total_c_points|$total_p_points|$tops";
    foreach $name (sort {$puzzle_number{$a} <=> $puzzle_number{$b}} 
           keys %puzzle_number) {
    $autoresult .= " |$name:$puzzle_c_points{$name}:$puzzle_c_rating{$name}:$puzzle_p_points{$name}:$puzzle_p_ops{$name}";
    }

    # 将自动结果发布到服务器。Linux登录ID与用户提供的昵称连接，
    # 用于对提交进行一些（非常）松散的身份验证。
    &Driverlib::driver_post("$login:$nickname", $autoresult, $autograded);
}

# 清理并退出
clean ($tmpdir);
exit;

##################
# 辅助函数
#

#
# check_nickname - 检查昵称是否合法
#
sub check_nickname {
    my $nickname = shift;

    # 昵称不能为空
    if (length($nickname) < 1) {
        die "$0: 错误: 空昵称。\n";
    }

    # 昵称不能太长
    if (length($nickname) > 35) {
        die "$0: 错误: 昵称超过35个字符。\n";
    }

    # 昵称只能有受限制的元字符集（例如，没有#HTML标签）
    if (!($nickname =~ /^[_-\w.,'@ ]+$/)) {
        die "$0: 错误: 昵称中有非法字符。只允许字母数字、撇号、逗号、句点、破折号、下划线和&符号。\n";
    }

    # 昵称不能全是空格
    if ($nickname =~ /^\s*$/) {
        die "$0: 错误: 昵称全是空格。\n";
    }

}
    
#
# clean - 删除临时目录
#
sub clean {
    my $tmpdir = shift;
    system("rm -rf $tmpdir");
}