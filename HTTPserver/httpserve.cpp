#include<stdio.h>
#include<iostream>
#include<string>
#include<winsock2.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
void error_die(string str)
{
	printf("[hint]%s failed:%d", str, WSAGetLastError());
	exit(-1);
}

void initSocket()
{
	//初始化网络库wsa			windows socket ansyc windows异步套接字
	//参数1：指定socket版本	最新2.2版本
	//参数2：传出版本
	WORD version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (0 != WSAStartup(version, &wsadata))
	{
		error_die("WSAStartup");
	}
}

int main()
{
	//初始化网络库wsa
	initSocket();

	//创建socket
	//参数1：指定ip协议
	//参数2：数据传输格式
	//参数3：传输协议
	SOCKET serfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (serfd == INVALID_SOCKET)
	{
		error_die("socket");
	}

	//绑定IP地址和端口号
	//参数1：指定的socket		上一步中的serfd
	//参数2：ip地址和端口号
	//参数3：长度
	struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(80); //本地字节序转为网络字节序
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY; //绑定本机任意ip地址 
	if (SOCKET_ERROR == bind(serfd, (struct sockaddr*)&serAddr, sizeof(serAddr)))
	{
		error_die("bind");
	}

	//监听 等电话
	listen(serfd, 10);


	//接受链接
	struct sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	SOCKET clifd = accept(serfd, (struct sockaddr*)&cliAddr, &len);
	if (clifd == INVALID_SOCKET)
	{
		error_die("accept");
	}

	// 处理链接请求 
	send;



	WSACleanup();

}
