#include <stdio.h> // 标准输入输出
#include <unistd.h> // 对 POSIX(可移植操作系统接口) 操作系统 API 的访问
#include <sys/types.h> // 基本系统数据类型
#include <sys/socket.h> // 用于操作网络套接字
#include <string.h> // 字符串处理
#include <ctype.h> // 测试字符是否属于特定的字符类别
#include <arpa/inet.h> // Linux下网络编程提供信息转换

#define SERVER_PORT 9991

int main() {

    int sock;   // 代表信箱：指向新创建的socket的文件描述符
    struct sockaddr_in server_addr;


    // 1.创建信箱
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2.清空标签，写上地址和端口号
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;   // 选择协议族IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听本地所有IP地址
    server_addr.sin_port = htons(SERVER_PORT);  // 绑定端口号

    // 3.实现标签贴到收信的信箱上
    bind(sock, (struct sockaddr *)&server_addr,  sizeof(server_addr));

    // 4.把信箱挂置到传达室，这样，就可以接收信件了
    listen(sock, 128);

    // 万事俱备，只等来信
    printf("等待客户端的连接\n");


    int done =1;
    while (done) {
        struct sockaddr_in client;
        int client_sock, len, i; // 与客户端通信的套接字、客户端发送的数据的长度、转换成大写的临时变量
        char client_ip[64]; // 存储客户端IP地址
        char buf[256]; // 存储客户端发送的数据

        // 返回一个新的socket文件描述符，用于和客户端通信
        socklen_t  client_addr_len;
        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);

        // 打印客户端IP地址和端口号
        printf("client ip: %s\t port: %d\n",
                 inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)),
                 ntohs(client.sin_port));

        // 读取客户端发送的数据
        len = read(client_sock, buf, sizeof(buf)-1);
        buf[len] = '\0';
        printf("receive[%d]: %s\n", len, buf);

        // 转换成大写
        for(i = 0; i < len; ++i) {
            /*if(buf[i]>='a' && buf[i]<='z'){
                buf[i] = buf[i] - 32;
            }*/
            buf[i] = toupper(buf[i]);
        }

        // 向客户端写（发送）数据
        len = write(client_sock, buf, len);
        printf("finished len: %d\n", len);
        
        close(client_sock);
    }

    close(sock);    
    return 0;
}
