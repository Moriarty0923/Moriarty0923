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

int main()
{
	//��ʼ�������wsa
	initSocket();

	//����socket
	//����1��ָ��ipЭ��
	//����2�����ݴ����ʽ
	//����3������Э��
	SOCKET serfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
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


	//��������
	struct sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	SOCKET clifd = accept(serfd, (struct sockaddr*)&cliAddr, &len);
	if (clifd == INVALID_SOCKET)
	{
		error_die("accept");
	}

	// ������������ 
	send;



	WSACleanup();

}
