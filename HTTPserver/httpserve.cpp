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
		fgets(data, 1024, pr);  //ֻ��һ��
		send(clifd, data, strlen(data), 0);
	} while (!feof(pr));
	fclose(pr);
}

void initSocket()
{
	//��ʼ�������wsa			windows socket ansyc windows�첽�׽���
	//����1��ָ��socket�汾	����2.2�汾
	//����2�������汾
	WORD version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (0 != WSAStartup(version, &wsadata))
	{
		error_die("WSAStartup");
	}
}

SOCKET	listenClient()
{
	//����socket
	//����1��ָ��ipЭ��
	//����2�����ݴ����ʽ
	//����3������Э��
	SOCKET serfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serfd == INVALID_SOCKET)
	{
		error_die("socket");
	}

	//��IP��ַ�Ͷ˿ں�
	//����1��ָ����socket		��һ���е�serfd
	//����2��ip��ַ�Ͷ˿ں�
	//����3������
	struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(80); //�����ֽ���תΪ�����ֽ���
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY; //�󶨱�������ip��ַ 
	if (SOCKET_ERROR == bind(serfd, (struct sockaddr*)&serAddr, sizeof(serAddr)))
	{
		error_die("bind");
	}

	//���� �ȵ绰
	listen(serfd, 10);
	return serfd;
}

void accept_request(SOCKET clifd)
{
	//��clifd��������
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
	//��ʼ�������wsa
	initSocket();

	SOCKET  serfd = listenClient();

	cout << "��ӭʹ��" << endl;
	//��������
	struct sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	while (1)
	{
		SOCKET clifd = accept(serfd, (struct sockaddr*)&cliAddr, &len);
		if (clifd == INVALID_SOCKET)
		{
			error_die("accept");
		}
		// ������������ 
		cout << "��һ���µ���������" << endl;
		//char sendData[1024] = "HELLO";
		//send(clifd, sendData, strlen(sendData),0);
		accept_request(clifd);
		closesocket(clifd);
	}





	//�ر�����

	closesocket(serfd);
	WSACleanup();
	return 0;
}
