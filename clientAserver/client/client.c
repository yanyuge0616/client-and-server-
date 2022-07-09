#include<stdio.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

#define SERV_PORT 1344      //宏定义服务器的端口号为常量

int main(int argc, char *argv[])
{
	int cfd;        //创建文件描述符用于通信
	int ret;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr;//创建server地址结构
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr.s_addr);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);



	cfd=socket(AF_INET,SOCK_STREAM,0);
	if (cfd == -1)
	{
		sys_err("socket error");
	}

	ret=connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if (ret != 0)
	{
		sys_err("connect error");
	}

	printf("connect server successfully\n");

	while (1)
	{
		gets(buf);
		write(cfd,buf,strlen(buf));
	}

	close(cfd);

	return 0;
}