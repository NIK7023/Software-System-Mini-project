#include "headers.h"
/*
    username : admin
    password : 1234
*/
void addstudent(int client_socket_fd);

bool admin_login(int client_socket_fd)
{
    printf("\n In Admin Login\n");
    char username[1024],password[1024];
    char ubuff[1024]="Enter Username :\n";
    char pbuff[1024]="Enter Password :\n";
    
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
    char menu[1024]="\n------------------Welcome Admin------------------\n1.Add student\n2.\n3.\nEnter Choice :";
    
    char buff;
    write(client_socket_fd,menu,sizeof(menu));
    read(client_socket_fd,&buff,sizeof(buff));

    // switch (buff)
    // {
    // case '1':   void addstudent(client_socket_fd);
    //     break;
    
    // default:
    //     break;
    // }    
    
}

void addstudent(int client_socket_fd)
{
    
}