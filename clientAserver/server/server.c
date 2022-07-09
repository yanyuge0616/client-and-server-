#include<stdio.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

#define SERV_PORT 1344      //宏定义服务器的端口号为常量

void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int lfd = 0;  //creat sockfd,用于接收新套接字的fd，后续要传入bind
	int cfd = 0; //这是用于接收accept()返回的file descriptor,用于通信

	int i;
	int ret;         //用于返回read()实际读到的字节数
	char buf[BUFSIZ],client_IP[BUFSIZ];//BUFSIZ是头文件中的宏定义

	struct sockaddr_in serv_addr;//创建服务器地址结构（IP+port）,且类型为sockaddr_in
	struct sockaddr_in clit_addr;//创建客户端地址结构
	socklen_t clit_addr_len;     //创建客户端地址长度变量为accept()服务

	/*serv_addr的初始化*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	lfd=socket(AF_INET,SOCK_STREAM,0);
	if (lfd == -1)
	{
		sys_err("socket error");
	}                          //到此socket创建成功，得到sockfd

	/*绑定IP地址和端口号*/
	bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));//此处应该得到一个返回值看看地址是否绑定成功，但我没写

	/*设置监听上限*/
	listen(lfd,128);//同样，检查返回值，我没写

	/*阻塞监听客户端建立连接*/
	clit_addr_len = sizeof(clit_addr);//先求出客户端地址长度才能将值传入accept()
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