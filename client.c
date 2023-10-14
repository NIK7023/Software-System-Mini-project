#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	int port=atoi(argv[1]);
	struct sockaddr_in server;

	int server_fd =socket(AF_INET,SOCK_STREAM,0);
	if(server_fd==-1)
	{
		perror("Error while creating server socket: ");
		return -1;	
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(port);
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
		memset(writebuff,0,sizeof(writebuff));
		read(server_fd,readbuff,sizeof(readbuff));
		if(readbuff[0]=='0')
		{
			fflush(stdout);
			write(1,&readbuff[1],strlen(readbuff)-1);//+1 to skip type
			fflush(stdout);
			memset(readbuff,0,sizeof(readbuff));		
		}
		else
		{
			fflush(stdout);
			write(1,&readbuff[1],strlen(readbuff)-1);//+1 to skip type
			fflush(stdout);
			memset(readbuff,0,sizeof(readbuff));
			memset(writebuff,0,sizeof(writebuff));
			fflush(stdin);
			int read_byte=read(0,writebuff,sizeof(writebuff));
			//while (getchar()!='\n');			
			fflush(stdin);
			writebuff[read_byte-1]='\0';
		
			//eif( strcmp(writebuff,"exit")==0 ) break;
			write(server_fd,writebuff,strlen(writebuff));
			memset(writebuff,0,sizeof(writebuff));
		}
	}



close(server_fd);

return 0;
}




//1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111