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

#include "structures.h"
#include "util.h"
/*
    username : admin
    password : 1234
*/
void addstudent(int client_socket_fd);
void viewstudent(int client_socket_fd);

bool admin_login(int client_socket_fd)
{
    printf("\n In Admin Login\n");
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
    //sleep(3);
    //write(1,password,sizeof(password));
    //printf("\nPassword %s",password);

    //write(1,username,s(username));
    printf("username %s",username);
    printf("Password %s",password);   

    if(strcmp("admin",username)==0 && strcmp("1234",password)==0) return true;
    else return false;

}


void display_admin_menu(int client_socket_fd)
{
    char menu[1024]="1\n------------------Welcome Admin------------------\n1.Add student\n2.View Student\n3.\n7.Logout and exit\nEnter Choice :";
    
    char buff='0';
    while(buff!='7') 
    {
        write(client_socket_fd,menu,sizeof(menu));
        read(client_socket_fd,&buff,sizeof(buff));

        switch (buff)
        {
        case '1':   addstudent(client_socket_fd);
            break;
        
        case '2':   viewstudent(client_socket_fd);
            break;
        
        default:
            break;
        }    
    }
    
}

void addstudent(int client_socket_fd)
{
    struct student s;
    s.id=getcount("studentcount.txt");
    s.active=1;
    sprintf(s.username,"MT%d",s.id);
    strcpy(s.password,"password");
    //memset(&s.course_enrolled,0,sizeof(s.course_enrolled));

    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student Name:",sizeof("1Enter Student Name:"));
    read(client_socket_fd,&buff,sizeof(buff));
    strcpy(s.name,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student Email:",sizeof("1Enter Student email:"));
    read(client_socket_fd,&buff,sizeof(buff));
    strcpy(s.email,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student age:",sizeof("1Enter Student age:"));
    read(client_socket_fd,&buff,sizeof(buff));
    strcpy(s.age,buff);

    int fd=open("student.txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
    lseek(fd,0,SEEK_END);
    if(write(fd,&s,sizeof(struct student))==-1)
    {
        perror("Error in writing in file: ");
        exit(0);
    }
    
    write(client_socket_fd,"0Student added successfully",sizeof("0Student added successfully"));
}



void viewstudent(int client_socket_fd)
{
    struct student s;
    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student ID:",sizeof("1Enter Student ID:"));
    read(client_socket_fd,&buff,sizeof(buff));
    write(1,buff,sizeof(buff));
    int id=atoi(buff);
    write(1,&id,sizeof(id));
    // int id,i=0,d=1;
    // while(buff[i])
    // {
    //     id+=d*buff[i];
    //     d*=10;
    //     i++;
    // }
    
    //printf("ID=%d",id);
    int fd=open("student.txt",O_RDONLY);
    int flag=0;
    memset(buff,0,sizeof(buff));
    while(read(fd,&s,sizeof(struct student)!=0))
    {
        //sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %s\n",s.name,s.email,s.age);
        // write(client_socket_fd,buff,strlen(buff));
        //write(1,buff,strlen(buff));
        write(1,&s.name,strlen(s.name));

        if(s.id==id) 
        { 
            write(1,"student found",sizeof("student found"));
            flag =1;
            break;
        }
    }

    if(flag==0) write(client_socket_fd,"0student not found",sizeof("0student not found"));
    else{
        sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %s\n",s.name,s.email,s.age);
        write(client_socket_fd,buff,strlen(buff));
    }



}