#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	struct sockaddr_in server;
	
	int server_fd =socket(AF_INET,SOCK_STREAM,0);
	if(server_fd==-1)
	{
		perror("Error while creating server socket: ");
		return -1;	
	}
	
	server.sin_family=AF_INET;
	server.sin_port=htons(8888);
	server.sin_addr.s_addr=INADDR_ANY;		//INADDR_LOOPBACK=127.0.0.1

	int connect_fd=connect(server_fd,(struct sockaddr*)&server,sizeof(server));
	
	if(connect_fd==-1)
	{
		perror("Error while connecting server:");
		return -1;
	}
	printf("Connection complete\n");
	
	// char menu[1024];
	// if(read(server_fd,&menu,sizeof(menu))<=0) {perror("Error while reading from server:"); exit(0);}
	// write(1,menu,sizeof(menu));
	// char choice;
	// read(0,&choice,sizeof(choice));
	// write(server_fd,&choice,sizeof(choice));

	char readbuff[1024],writebuff[1024];
	while(1)
	{
		memset(readbuff,0,sizeof(readbuff));
		read(server_fd,readbuff,sizeof(readbuff));
		
		write(1,readbuff,sizeof(readbuff));

		memset(writebuff,0,sizeof(writebuff));
		fflush(stdin);
		read(0,writebuff,sizeof(writebuff));
//		if( strcmp(writebuff,"exit")==0 ) break;
		write(server_fd,writebuff,sizeof(writebuff));	


	}
	
	
	// while(1)
	// {
	// 	read
	// }


close(server_fd);

return 0;
}


/*
	int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	
	
	
*/	
	

