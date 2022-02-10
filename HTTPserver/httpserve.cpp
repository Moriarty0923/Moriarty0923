#include<stdio.h>
#include<iostream>
#include<string>
#include<winsock2.h>
#include<io.h>
#include<Winerror.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
void error_die(string str)
{
	printf("[hint]%s failed:%d", str, WSAGetLastError());
	exit(-1);
}

void sendhtml(SOCKET clifd, char* filepath)
{
	FILE*pr = fopen(filepath, "r");
	if (pr == NULL)
	{
		error_die("fopen");
	}
	char data[1024] = "";
	do {
		fgets(data, 1024, pr);  //只读一次
		send(clifd, data, strlen(data), 0);
	} while (!feof(pr));
	fclose(pr);
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

SOCKET	listenClient()
{
	//创建socket
	//参数1：指定ip协议
	//参数2：数据传输格式
	//参数3：传输协议
	SOCKET serfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	return serfd;
}

void accept_request(SOCKET clifd)
{
	//从clifd接受数据
	char recvBuf[1024] = "";
	if (0 >= recv(clifd, recvBuf, sizeof(recvBuf), 0))
	{
		error_die("accept");
	}
	cout << "recvBuf" << recvBuf << endl;
	char filepath[128] = "inde.html";
	if (_access(filepath, 0) == 0)
	{
		sendhtml(clifd, filepath);
	}
	else
	{
		error_die("acc");
	}
}




int main()
{
	//初始化网络库wsa
	initSocket();

	SOCKET  serfd = listenClient();

	cout << "欢迎使用" << endl;
	//接受链接
	struct sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	while (1)
	{
		SOCKET clifd = accept(serfd, (struct sockaddr*)&cliAddr, &len);
		if (clifd == INVALID_SOCKET)
		{
			error_die("accept");
		}
		// 处理链接请求 
		cout << "有一个新的链接请求" << endl;
		//char sendData[1024] = "HELLO";
		//send(clifd, sendData, strlen(sendData),0);
		accept_request(clifd);
		closesocket(clifd);
	}





	//关闭连接

	closesocket(serfd);
	WSACleanup();
	return 0;
}
