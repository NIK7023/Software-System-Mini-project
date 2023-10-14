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
#include "util.h"
#include "faculty.h"


/*
    username : admin
    password : 1234
*/
void addstudent(int client_socket_fd);
int getcount(char *file,char *type);
void viewstudent(int client_socket_fd);
void addfaculty(int client_socket_fd);
void viewfaculty(int client_socket_fd);
void modifystudent(int client_socket_fd);
void modifyfaculty(int client_socket_fd);

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
    
    printf("struct student size %ld",sizeof(struct student));
    printf("username %s",username);
    printf("Password %s",password);   
    
    if(strcmp("admin",username)==0 && strcmp("1234",password)==0) return true;
    else return false;

}


void display_admin_menu(int client_socket_fd)
{
    char menu[1024]="1\n------------------Welcome Admin------------------\n1.Add student\n2.View Student\n3.Add Faculty\n4.View Faculty\n5.Modify Student\n6.Modify Faculty\n7.\n7.Logout and exit\nEnter Choice :";
    
    char buff='\0';
    do
    {
        write(client_socket_fd,menu,sizeof(menu));
        buff='\0';
        read(client_socket_fd,&buff,sizeof(buff));

        switch (buff)
        {
        case '1':   addstudent(client_socket_fd);
            break;
        
        case '2':   viewstudent(client_socket_fd);
            break;
        
        case '3':   addfaculty(client_socket_fd);
            break;
        case '4':   viewfaculty(client_socket_fd);
            break;
        case '5':   modifystudent(client_socket_fd);
            break;
        case '6':   modifyfaculty(client_socket_fd);
            break;
        
        default:
            break;
        }    
        
    }while(buff!='7');  
    
}

void addstudent(int client_socket_fd)
{
    struct student s;
    struct count c;
	int fd = open("count.txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
	if(read(fd,&c,sizeof(struct count)) ==-1)
    {
        perror("Error in reading:");
    }
    c.student++;
    lseek(fd,0,SEEK_SET);
    if(write(fd,&c,sizeof(struct count)) ==-1)
    {
        perror("Error in reading:");
    }
    s.id=c.student;
    close(fd);
    fflush(stdout);
    if(write(1, &s.id,sizeof(s.id))==-1) perror("id error");
    s.active=1;
    sprintf(s.username,"MT%d",s.id);
    fflush(stdout);
    write(1,&s.username,sizeof(s.username));
    strcpy(s.password,"password");
    //memset(&s.course_enrolled,0,sizeof(s.course_enrolled));

    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student Name:",sizeof("1Enter Student Name:"));
    read(client_socket_fd,buff,sizeof(buff));
    buff[sizeof(s.name)-1]='\0';
    strcpy(s.name,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student Email:",sizeof("1Enter Student email:"));
    read(client_socket_fd,buff,sizeof(buff));
    buff[sizeof(s.email)-1]='\0';
    strcpy(s.email,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student age:",sizeof("1Enter Student age:"));
    read(client_socket_fd,buff,sizeof(buff));
    s.age=atoi(buff);

    fd=open("student.txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
    lseek(fd,0,SEEK_END);
    //while(read(fd,&s,sizeof(struct student))!=0);   
    sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %d\n",s.name,s.email,s.age);
        write(client_socket_fd,buff,strlen(buff));
    if(write(fd,&s,sizeof(struct student))==-1)
    {
        perror("Error in writing in file: ");
        exit(0);
    }
    close(fd);
    write(client_socket_fd,"0Student added successfully",sizeof("0Student added successfully"));
}

// int getcount(char *file,char *type)
// {
//     struct count c;
// 	int fd = open(file,O_RDWR);
//     if(fd==-1)
//     {
//         perror("Error in opeing file: ");
//         exit(0);
//     }
// 	if(read(fd,&c,sizeof(struct count)) ==-1)
//     {
//         perror("Error in reading:");
//     }
//     if(strcmp(type,"student")==0)
//     {
//         c.student++;
//         if(write(fd,&c,sizeof(struct count)) ==-1)
//         {
//             perror("Error in reading:");
//         }
//         return c.student;        
//     }


// }



void viewstudent(int client_socket_fd)
{
    struct student s;
    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student ID:",strlen("1Enter Student ID:"));
    read(client_socket_fd,&buff,sizeof(buff));
    write(1,buff,sizeof(buff));
    int id=atoi(buff);
    //write(1,&id,sizeof(id));

    
    //printf("ID=%d",id);
    int fd=open("student.txt",O_RDONLY);
    if(fd==-1)
    {
        perror("Error in file opening:");
    }
    int flag=0;
    memset(buff,0,sizeof(buff));
    lseek(fd,0,SEEK_SET);
    while(read(fd,&s,sizeof(struct student))>0)
    {
        sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %d",s.name,s.email,s.age);
        // write(client_socket_fd,buff,strlen(buff));
        write(1,buff,strlen(buff));
        //write(1,&s.name,strlen(s.name));

        if(s.id==id) 
        { 
            write(1,"student found",sizeof("student found"));
            flag =1;
            //close(fd);
            break;
        }
        
    }
    close(fd);
    memset(buff,0,sizeof(buff));
    if(flag==0) write(client_socket_fd,"0student not found",strlen("0student not found"));
    else{
        sprintf(buff,"0Name : %s\nEmail : %s\nAge : %d\n",s.name,s.email,s.age);
        write(client_socket_fd,buff,strlen(buff));
    }



}


void addfaculty(int client_socket_fd)
{
    struct faculty f;
    struct count c;
	int fd = open("count.txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
	if(read(fd,&c,sizeof(struct count)) ==-1)
    {
        perror("Error in reading:");
    }
    c.faculty++;
    lseek(fd,0,SEEK_SET);
    if(write(fd,&c,sizeof(struct count)) ==-1)
    {
        perror("Error in reading:");
    }

    f.id=c.faculty;
    close(fd);
    fflush(stdout);
    if(write(1, &f.id,sizeof(f.id))==-1) perror("id error");
    sprintf(f.username,"%d",f.id);
    fflush(stdout);
    write(1,&f.username,sizeof(f.username));
    strcpy(f.password,"password");


    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter faculty Name:",sizeof("1Enter faculty Name:"));
    read(client_socket_fd,buff,sizeof(buff));
    buff[sizeof(f.name)-1]='\0';
    strcpy(f.name,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter faculty Email:",sizeof("1Enter faculty email:"));
    read(client_socket_fd,buff,sizeof(buff));
    buff[sizeof(f.email)-1]='\0';
    strcpy(f.email,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter faculty age:",sizeof("1Enter faculty age:"));
    read(client_socket_fd,buff,sizeof(buff));
    f.age=atoi(buff);

    fd=open("faculty.txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
    lseek(fd,0,SEEK_END);
    //while(read(fd,&s,sizeof(struct student))!=0);   
    sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %d\n",f.name,f.email,f.age);
        write(client_socket_fd,buff,strlen(buff));
    if(write(fd,&f,sizeof(struct faculty))==-1)
    {
        perror("Error in writing in file: ");
        exit(0);
    }
    close(fd);
    write(client_socket_fd,"0Faculty added successfully",sizeof("0Faculty added successfully"));   
}


void viewfaculty(int client_socket_fd)
{
    struct faculty f;
    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter faculty ID:",strlen("1Enter faculty ID:"));
    read(client_socket_fd,&buff,sizeof(buff));
    write(1,buff,sizeof(buff));
    int id=atoi(buff);
    //write(1,&id,sizeof(id));

    
    //printf("ID=%d",id);
    int fd=open("faculty.txt",O_RDONLY);
    if(fd==-1)
    {
        perror("Error in file opening:");
    }
    int flag=0;
    memset(buff,0,sizeof(buff));
    lseek(fd,0,SEEK_SET);
    while(read(fd,&f,sizeof(struct faculty))>0)
    {
        sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %d",f.name,f.email,f.age);
        // write(client_socket_fd,buff,strlen(buff));
        write(1,buff,strlen(buff));
        //write(1,&s.name,strlen(s.name));

        if(f.id==id) 
        { 
            write(1,"faculty found",sizeof("faculty found"));
            flag =1;
            //close(fd);
            break;
        }
        
    }
    close(fd);
    memset(buff,0,sizeof(buff));
    if(flag==0) write(client_socket_fd,"0faculty not found",strlen("0faculty not found"));
    else{
        sprintf(buff,"0Name : %s\nEmail : %s\nAge : %d\n",f.name,f.email,f.age);
        write(client_socket_fd,buff,strlen(buff));
    }
}



void modifystudent(int client_socket_fd)
{
    struct student s;
    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Student ID:",strlen("1Enter Student ID:"));
    read(client_socket_fd,&buff,sizeof(buff));
    write(1,buff,sizeof(buff));
    int id=atoi(buff);
    //write(1,&id,sizeof(id));

    
    //printf("ID=%d",id);
    int fd=open("student.txt",O_RDWR);

    if(fd==-1)
    {
        perror("Error in file opening:");
    }
    //read lock
    struct flock read_lock={F_RDLCK,SEEK_SET,0,0,getpid()};
    int lock_status=fcntl(fd,F_SETLKW,&read_lock);
    if(lock_status==-1) 
    {
        perror("Student file lock error:");
        exit(0);
    }
    int flag=0;
    memset(buff,0,sizeof(buff));
    lseek(fd,0,SEEK_SET);
    while(read(fd,&s,sizeof(struct student))>0)
    {
        //sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %d",s.name,s.email,s.age);
        // write(client_socket_fd,buff,strlen(buff));
        //write(1,buff,strlen(buff));
        //write(1,&s.name,strlen(s.name));

        if(s.id==id) 
        { 
            write(1,"student found",sizeof("student found"));
            flag =1;            
            break;
        }
        
    }
    read_lock.l_type=F_UNLCK;
    lock_status=fcntl(fd,F_SETLKW,&read_lock);
    //Read unlock
       
    memset(buff,0,sizeof(buff));
    if(flag==0) write(client_socket_fd,"0student not found",strlen("0student not found"));
    else{

        memset(buff,0,sizeof(buff));
        write(client_socket_fd,"1Enter Student Name:",sizeof("1Enter Student Name:"));
        read(client_socket_fd,buff,sizeof(buff));
        buff[sizeof(s.name)-1]='\0';
        strcpy(s.name,buff);

        memset(buff,0,sizeof(buff));
        write(client_socket_fd,"1Enter Student Email:",sizeof("1Enter Student email:"));
        read(client_socket_fd,buff,sizeof(buff));
        buff[sizeof(s.email)-1]='\0';
        strcpy(s.email,buff);

        memset(buff,0,sizeof(buff));
        write(client_socket_fd,"1Enter Student age:",sizeof("1Enter Student age:"));
        read(client_socket_fd,buff,sizeof(buff));
        s.age=atoi(buff);
        //write locl
        lseek(fd,-sizeof(struct student),SEEK_CUR);
        int lock_length = sizeof(struct student);
        struct flock write_lock = {F_WRLCK,SEEK_CUR,0,lock_length,getpid()};
        lock_status=fcntl(fd,F_SETLKW,&read_lock);
        if(lock_status==-1) 
        {
            perror("Student file lock error:");
            exit(0);
        }
        if(write(fd,&s,sizeof(struct student))==-1)
        {
            perror("Error in writing in file: ");
            exit(0);
        }
        //unlock
        write_lock.l_type=F_UNLCK;
        lock_status=fcntl(fd,F_SETLKW,&read_lock);

        sprintf(buff,"0Name : %s\nEmail : %s\nAge : %d\n",s.name,s.email,s.age);
        write(client_socket_fd,buff,strlen(buff));
    }

    
close(fd);
}


void modifyfaculty(int client_socket_fd)
{
    struct faculty f;
    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter faculty ID:",strlen("1Enter faculty ID:"));
    read(client_socket_fd,&buff,sizeof(buff));
    write(1,buff,sizeof(buff));
    int id=atoi(buff);
    //write(1,&id,sizeof(id));

    
    //printf("ID=%d",id);
    int fd=open("faculty.txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in file opening:");
    }
    //Read lock
    struct flock read_lock={F_RDLCK,SEEK_SET,0,0,getpid()};
    int lock_status=fcntl(fd,F_SETLKW,&read_lock);
    if(lock_status==-1) 
    {
        perror("Faculty file lock error:");
        exit(0);
    }

    int flag=0;
    memset(buff,0,sizeof(buff));
    lseek(fd,0,SEEK_SET);
    while(read(fd,&f,sizeof(struct faculty))>0)
    {
        sprintf(buff,"0\nName : %s\nEmail : %s\nAge : %d",f.name,f.email,f.age);
        // write(client_socket_fd,buff,strlen(buff));
        write(1,buff,strlen(buff));
        //write(1,&s.name,strlen(s.name));

        if(f.id==id) 
        { 
            write(1,"faculty found",sizeof("faculty found"));
            flag =1;
            break;
        }
        
    }
    read_lock.l_type=F_UNLCK;
    lock_status=fcntl(fd,F_SETLKW,&read_lock);
    //Read UNlock
    memset(buff,0,sizeof(buff));
    if(flag==0) write(client_socket_fd,"0faculty not found",strlen("0faculty not found"));
    else{  

        memset(buff,0,sizeof(buff));
        write(client_socket_fd,"1Enter faculty Name:",sizeof("1Enter faculty Name:"));
        read(client_socket_fd,buff,sizeof(buff));
        buff[sizeof(f.name)-1]='\0';
        strcpy(f.name,buff);

        memset(buff,0,sizeof(buff));
        write(client_socket_fd,"1Enter faculty Email:",sizeof("1Enter faculty email:"));
        read(client_socket_fd,buff,sizeof(buff));
        buff[sizeof(f.email)-1]='\0';
        strcpy(f.email,buff);

        memset(buff,0,sizeof(buff));
        write(client_socket_fd,"1Enter faculty age:",sizeof("1Enter faculty age:"));
        read(client_socket_fd,buff,sizeof(buff));
        f.age=atoi(buff);

        //write lock
        lseek(fd,-sizeof(struct faculty),SEEK_CUR);
        int lock_length = sizeof(struct faculty);
        struct flock write_lock = {F_WRLCK,SEEK_CUR,0,lock_length,getpid()};
        lock_status=fcntl(fd,F_SETLKW,&read_lock);
        if(lock_status==-1) 
        {
            perror("faculty file lock error:");
            exit(0);
        }
        if(write(fd,&f,sizeof(struct faculty))==-1)
        {
            perror("Error in writing in file: ");
            exit(0);
        }
        //unlock
        write_lock.l_type=F_UNLCK;
        lock_status=fcntl(fd,F_SETLKW,&read_lock);

        sprintf(buff,"0Name : %s\nEmail : %s\nAge : %d\n",f.name,f.email,f.age);
        write(client_socket_fd,buff,strlen(buff));
    }
}