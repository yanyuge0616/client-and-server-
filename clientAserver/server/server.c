#include<stdio.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

#define SERV_PORT 1344      //�궨��������Ķ˿ں�Ϊ����

void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int lfd = 0;  //creat sockfd,���ڽ������׽��ֵ�fd������Ҫ����bind
	int cfd = 0; //�������ڽ���accept()���ص�file descriptor,����ͨ��

	int i;
	int ret;         //���ڷ���read()ʵ�ʶ������ֽ���
	char buf[BUFSIZ],client_IP[BUFSIZ];//BUFSIZ��ͷ�ļ��еĺ궨��

	struct sockaddr_in serv_addr;//������������ַ�ṹ��IP+port��,������Ϊsockaddr_in
	struct sockaddr_in clit_addr;//�����ͻ��˵�ַ�ṹ
	socklen_t clit_addr_len;     //�����ͻ��˵�ַ���ȱ���Ϊaccept()����

	/*serv_addr�ĳ�ʼ��*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	lfd=socket(AF_INET,SOCK_STREAM,0);
	if (lfd == -1)
	{
		sys_err("socket error");
	}                          //����socket�����ɹ����õ�sockfd

	/*��IP��ַ�Ͷ˿ں�*/
	bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));//�˴�Ӧ�õõ�һ������ֵ������ַ�Ƿ�󶨳ɹ�������ûд

	/*���ü�������*/
	listen(lfd,128);//ͬ������鷵��ֵ����ûд

	/*���������ͻ��˽�������*/
	clit_addr_len = sizeof(clit_addr);//������ͻ��˵�ַ���Ȳ��ܽ�ֵ����accept()
	cfd=accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
	if (cfd == -1)
	{
		sys_err("accept error");
	}
	printf("client IP:%s port:%d connect successfully\n",inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr,client_IP,sizeof(client_IP)),ntohs(clit_addr.sin_port));

	while (1) 
	{
		ret = read(cfd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, ret);
		printf("\n");

		/*for (i = 0; i < ret; i++)
			buf[i] = toupper(buf[i]);

		write(cfd, buf, ret);*/
	}

	close(lfd);
	close(cfd);

	return 0;
}