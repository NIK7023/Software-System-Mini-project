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
#include "admin.h"
// #include "util.h"
void display_student_menu(int client_socket_fd);  
bool student_login(int client_socket_fd)
{
    printf("\n In Student Login\n");
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
    struct student s;
    int flag=0;
    int fd=open("student.txt",O_RDONLY);
    // if(fd==-1)
    // {
    //     perror("Error in file opening:");
    // }
    while(read(fd,&s,sizeof(struct student)!=0))
    {   
        write(1,&s.username,strlen(s.username)); 
        //printf("%s\n",s.username);
        if(strcmp(s.username,username)==0) 
        { 
            write(1,"username found",sizeof("username found"));
            flag =1;
            close(fd);
            break;
        }
    }

    //write(1,&s.username,strlen(s.username));
    //write(1,&s.password,strlen(s.password));
    if(strcmp(s.username,username)==0 && strcmp(s.password,password)==0) return true;
    else return false;

}


void display_student_menu(int client_socket_fd)
{
    char menu[1024]="1\n------------------Welcome student------------------\n1.View Courses\n2.\n3.\n7.Logout and exit\nEnter Choice :";
    
    char buff='0';
    while(buff!='7') 
    {
        
        write(client_socket_fd,menu,sizeof(menu));
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
    }
    
}