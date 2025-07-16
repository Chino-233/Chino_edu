#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INT_SIZE sizeof(unsigned long)
#define REQUEST_SIZE 35
#define PORT 7788
#define BUFFER_SIZE 1024
#define STOP_BYTE 0xFF
#define VIDEO_PATH "D:\\exp-code\\video\\"
#define VIDEO_NAME "video.mp4"
#define END_REQUEST "END_REQUEST"
#define VIDEO_LEN 60 // 视频总时长为60s

// 定义一个获取文件大小的函数，参数是一个文件指针
long get_file_size(FILE *fp)
{
    long file_size = -1;
    fpos_t cur_pos;
    fgetpos(fp, &cur_pos);
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fsetpos(fp, &cur_pos);
    return file_size;
}

int main()
{
    /***初始化阶段***/
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    int server_fd, new_socket;
    struct sockaddr_in server_address;
    int opt = 1;
    int addrlen = sizeof(server_address);
    char buffer[BUFFER_SIZE] = {0}; // 创建发送缓冲区

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Create Server Socket Success.\n");

    // 设置套接字选项
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server Bind Port Success. \n");

    // 监听套接字
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server Listening......\n");

    // 接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, &addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server Accept Success. \n");

    while (1)
    {
        // 接收客户端下载请求
        int bytes_recv = 0;
        char req[REQUEST_SIZE] = "";
        bytes_recv = recv(new_socket, req, REQUEST_SIZE, 0);
        if (bytes_recv <= 0) {
            printf("ERROR in recv, 错误码: %d\n", WSAGetLastError());
            break;
        }
        printf("req: %s\n", req); //接收请求的文件名
        
        // 接收请求终止字节
        unsigned char r_stop_byte;
        if (recv(new_socket, &r_stop_byte, 1, 0) != 1 || r_stop_byte != STOP_BYTE) {
            printf("ERROR in receiving stop byte 0x%02X\n", r_stop_byte);
            break;
        }
        r_stop_byte = 'e'; // 重置

        /****************************************************************************/
        /********** 扩展任务：如何根据client信号，终止传输并退出循环？**********/
        /****************************************************************************/
        if (strcmp(req, END_REQUEST) == 0) {
            printf("收到客户端终止请求，服务器将停止传输\n");
            break;  // 退出主循环
        }
        
        // 找到文件
        char *file_name = req;
        char file_path[100] = VIDEO_PATH;
        strcat(file_path, file_name);
        
        // 打开视频文件
        FILE *fp = fopen(file_path, "rb"); // 以二进制读模式打开文件
        if (fp == NULL)
        {
            perror("File open failed");
            continue;
        }

        // 获取文件大小
        unsigned long file_size = (unsigned long)get_file_size(fp);
        printf("文件路径: %s, 文件大小: %lu 字节\n", file_path, file_size);

        // 发送文件的大小到客户端
        unsigned long file_size_buf = htonl(file_size); // 转换为网络字节序
        int bytes_sent = 0;
        bytes_sent = send(new_socket, (char *)&file_size_buf, INT_SIZE, 0);
        if (bytes_sent < 0) {
            printf("ERROR in send file size, 错误码: %d\n", WSAGetLastError());
            fclose(fp);
            continue;
        }

        // 发送视频片段
        int send_count = 0;
        while (send_count < file_size)
        {
            /***************************************************************************************************/
            /**************************  任务2： 发送指定视频文件（按照固定大小buffer方式） ********************/
            /***************************************************************************************************/
            
            // 确定本次要发送的数据大小
            int bytes_to_send = BUFFER_SIZE;
            if (file_size - send_count < BUFFER_SIZE) {
                bytes_to_send = file_size - send_count;
            }
            
            // 从文件读取数据到缓冲区
            int bytes_read = fread(buffer, 1, bytes_to_send, fp);
            if (bytes_read <= 0) {
                printf("文件读取错误或已到达文件尾\n");
                break;
            }
            
            // 发送缓冲区数据
            bytes_sent = send(new_socket, buffer, bytes_read, 0);
            if (bytes_sent < 0) {
                printf("ERROR in send data, 错误码: %d\n", WSAGetLastError());
                break;
            }
            send_count += bytes_sent;
            // 显示发送进度
            printf("\r发送进度: %.2f%%", (float)send_count / file_size * 100);
            fflush(stdout);
        }
        printf("\n发送完成，共发送 %d 字节\n", send_count);
        
        // 发送文件结束符
        unsigned char s_stop_byte = STOP_BYTE;
        bytes_sent = send(new_socket, &s_stop_byte, sizeof(s_stop_byte), 0);
        if (bytes_sent < 0)
            printf("ERROR in send stop byte, 错误码: %d\n", WSAGetLastError());

        // 关闭文件
        fclose(fp);
        printf("文件已关闭\n");
    }

    /***结束阶段***/
    closesocket(server_fd);
    closesocket(new_socket);
    WSACleanup();

    return 0;
}