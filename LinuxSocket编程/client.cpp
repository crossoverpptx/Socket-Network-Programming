#include <stdio.h>
#include <stdlib.h> // 定义了最常用的系统函数：malloc、free等
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // 网络地址类型定义：uint16_t、uint32_t、sockaddr_in、sockaddr_in6等
#include <arpa/inet.h>

#define SERVER_PORT 9991
#define SERVER_IP  "127.0.0.1"

int main(int argc, char* argv[]) {

    int sockfd;
    char *message;
    struct sockaddr_in servaddr;
    int len; // 从服务器读（接收）数据的长度
    char buf[64];

    if(argc != 2) {
        fputs("Usage: ./client message \n", stderr); // 向指定的文件写入一个字符串
        exit(1);
    }

    message = argv[1];
    printf("send message: %s\n", message);

    // 1.创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, '\0', sizeof(struct sockaddr_in)); // 初始化函数。作用是将某一块内存中的内容全部设置为指定的值
                                                         // 对结构体清零
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT);

    // 2.与服务器建立连接
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // 3.向服务器写（发送）数据
    write(sockfd, message, strlen(message));

    // 4.从服务器读（接收）数据
    len = read(sockfd, buf, sizeof(buf)-1);
    if (len > 0) {
        buf[len] = '\0';
        printf("receive from server: %s\n", buf);
    } else {
        perror("error!!!");
    }

    printf("finished.\n");
    close(sockfd);
    return 0;
}
