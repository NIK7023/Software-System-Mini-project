#include "headers.h"
#include "admin.h"
#include "structures.h"
#include "util.h"
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
	server.sin_addr.s_addr=INADDR_ANY;		
	
	bind(server_fd,(struct sockaddr*)&server,sizeof(server));
	printf("Bind complete\n");
	listen(server_fd,100);
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

			char menu[1024]="\n------------------Welcome to Academia------------------\nLogin as\n1.Admin\n2.Proffessor\n3.Student\nEnter Choice :";
			write(client_socket_fd,menu,sizeof(menu));

			char choice;		
			
			if(read(client_socket_fd,&choice,sizeof(choice))<=0)
			{
				perror("client Read error:");
				exit(0);
			}
			switch(choice)
			{
				case '1':	bool login=admin_login(client_socket_fd);
							if(login==true) printf("\nLogin Sucessfull\n");
							else printf("\nInvalid username or password");
							display_admin_menu(client_socket_fd);
							break;

				case '2':

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



