#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h>

#include <sys/stat.h>
	
// #include "admin.h"
#include "student.h"

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
	server.sin_addr.s_addr=INADDR_ANY;		

	if(bind(server_fd,(struct sockaddr*)&server,sizeof(server))==-1) 
	{
		perror("Bind error:");
		exit(0);
	}
	printf("Bind complete\n");
	if(listen(server_fd,100)==-1)
	{
		perror("Listen error:");
		exit(0);
	}
	printf("Listen\n");
	struct sockaddr_in client_address;
	int client_socket_fd;

	socklen_t client_address_len=sizeof(client_address);;
	while(1)
	{
		client_socket_fd=accept(server_fd,(struct sockaddr*)&client_address,&client_address_len);
		if(client_socket_fd==-1)
		{
			perror("Error while creating server socket: ");
			continue;
		}
		else 
		{
			if(fork()==0)
			{
			close(server_fd);
			printf("Client connected\n");

			char menu[1024]="1\n------------------Welcome to Academia------------------\nLogin as\n1.Admin\n2.Proffessor\n3.Student\nEnter Choice :";
			write(client_socket_fd,menu,sizeof(menu));

			char choice;		

			if(read(client_socket_fd,&choice,sizeof(choice))<=0)
			{
				perror("client Read error:");
				exit(0);
			}
			bool login;
			switch(choice)
			{
				case '1':	login=admin_login(client_socket_fd);
							if(login==true) printf("\nLogin Sucessfull\n");
							else {	printf("\nInvalid username or password"); exit(0);}
							 display_admin_menu(client_socket_fd);
					break;

				case '2':	login=faculty_login(client_socket_fd);
							if(login==true) printf("\nLogin Sucessfull\n");
							else {	printf("\nInvalid username or password"); exit(0);}
							 display_faculty_menu(client_socket_fd);
					break;

				case '3'	:login=student_login(client_socket_fd);
							if(login==true) printf("\nLogin Sucessfull\n");
							else {	printf("\nInvalid username or password"); exit(0);}
							 display_student_menu(client_socket_fd);
					break;

			}



			close(client_socket_fd);
			exit(0);	
			}		
			else close(client_socket_fd); //close in parent
		}

	}
close(client_socket_fd);
return 0;
}


