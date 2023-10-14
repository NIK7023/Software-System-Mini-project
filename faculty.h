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
#include <sys/select.h>
#include "structures.h"

void display_faculty_menu(int client_socket_fd);  

bool faculty_login(int client_socket_fd)
{
    printf("\n In faculty Login\n");
    char username[1024],password[1024];
    char ubuff[1024]="1Enter Username :\n";
    char pbuff[1024]="1Enter Password :\n";
    
    memset(username,0,sizeof(username));
    memset(password,0,sizeof(password));
    
    write(client_socket_fd,ubuff,strlen(ubuff));
    //sleep(3);
    read(client_socket_fd,username,sizeof(username));
    //sleep(3);
    write(client_socket_fd,pbuff,strlen(pbuff));
    //sleep(3);
    read(client_socket_fd,password,sizeof(password));
    printf("username %s",username);
    printf("Password %s\n",password);   
    //sleep(3);
    //write(1,password,sizeof(password));
    //printf("\nPassword %s",password);

    //write(1,username,s(username));
    struct faculty f;
    int flag=0;
    int fd=open("faculty.txt",O_RDONLY);
    if(fd==-1)
    {
        perror("Error in file opening:");
    }
    while(read(fd,&f,sizeof(struct faculty))>0)
    {   
        //write(1,&s.username,strlen(s.username)); 
        //printf("%s\n",s.username);
        if(strcmp(f.username,username)==0) 
        { 
            write(1,"username found",sizeof("username found"));
            flag =1;
            close(fd);
            break;
        }
    }

    //write(1,&s.username,strlen(s.username));
    //write(1,&s.password,strlen(s.password));
    if(strcmp(f.username,username)==0 && strcmp(f.password,password)==0) return true;
    else return false;

}


void display_faculty_menu(int client_socket_fd)
{
    char menu[1024]="1\n------------------Welcome student------------------\n1.Add Courses\n2.\n3.\n7.Logout and exit\nEnter Choice :";
    
    char buff='0';
    do
    {
        
        write(client_socket_fd,menu,sizeof(menu));
        buff='0';
        read(client_socket_fd,&buff,sizeof(buff));

        switch (buff)
        {
        case '1':   
            break;
        
        case '2':   
            break;
        
        default:
            break;
        }    
    }while(buff!='7');
    
}