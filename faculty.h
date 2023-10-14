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

void display_faculty_menu(int client_socket_fd,struct faculty *f);
void addcourse(int client_socket_fd,int facultyid,char *facultyname);
void viewcourse(int client_socket_fd,int facultyid);


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
    if(strcmp(f.username,username)==0 && strcmp(f.password,password)==0) 
    {
        display_faculty_menu(client_socket_fd,&f);
        return true;
    }
    else return false;

}


void display_faculty_menu(int client_socket_fd,struct faculty *f)
{
    char menu[1024]="1\n------------------Welcome student------------------\n1.Add Courses\n2.View Courses\n3.\n7.Logout and exit\nEnter Choice :";
    
    char buff='0';
    do
    {
        
        write(client_socket_fd,menu,sizeof(menu));
        buff='0';
        read(client_socket_fd,&buff,sizeof(buff));

        switch (buff)
        {
        case '1':   addcourse(client_socket_fd,f->id,f->name);
            break;
        
        case '2':   viewcourse(client_socket_fd,f->id);
            break;
        
        default:
            break;
        }    
    }while(buff!='7');
    
}



void addcourse(int client_socket_fd,int facultyid,char *facultyname)
{
    struct course cou;
    struct count c;
	int fd = open("count.txt",O_RDWR);
    
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
    //write lock
        lseek(fd,0,SEEK_SET);
        int lock_length = sizeof(struct count);
        struct flock write_lock = {F_WRLCK,SEEK_CUR,0,lock_length,getpid()};
        int lock_status=fcntl(fd,F_SETLKW,&write_lock);
        if(lock_status==-1) 
        {
            perror("count file lock error:");
            exit(0);
        }
	if(read(fd,&c,sizeof(struct count)) ==-1)
    {
        perror("Error in reading:");
    }
    c.course++;
    lseek(fd,0,SEEK_SET);
    if(write(fd,&c,sizeof(struct count)) ==-1)
    {
        perror("Error in reading:");
    }
        write_lock.l_type=F_UNLCK;
        lock_status=fcntl(fd,F_SETLKW,&write_lock);
    //write unlock

    cou.id=c.course;
    close(fd);

    cou.faculty_id=facultyid;
    strcpy(cou.faculty_name,facultyname);
    cou.active=1;
    cou.enrollcount=0;

    char buff[1024];
    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter course Name:",sizeof("1Enter course Name:"));
    read(client_socket_fd,buff,sizeof(buff));
    buff[sizeof(cou.name)-1]='\0';
    strcpy(cou.name,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter course Dept:",sizeof("1Enter course Dept:"));
    read(client_socket_fd,buff,sizeof(buff));
    buff[sizeof(cou.dept)-1]='\0';
    strcpy(cou.dept,buff);

    memset(buff,0,sizeof(buff));
    write(client_socket_fd,"1Enter Total Seats:",sizeof("1Enter Total Seats:"));
    read(client_socket_fd,buff,sizeof(buff));
    cou.totalseats=atoi(buff);

    
    fd = open("course.txt",O_RDWR);    
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
    //write lock
        write_lock.l_type=F_WRLCK;
        write_lock.l_whence=SEEK_END;
        write_lock.l_start=0;
        write_lock.l_len=sizeof(struct course);
        write_lock.l_pid=getpid();
        
        lock_status=fcntl(fd,F_SETLKW,&write_lock);
        if(lock_status==-1) 
        {
            perror("course file lock error:");
            exit(0);
        }
    lseek(fd,0,SEEK_END);
    if(write(fd,&cou,sizeof(struct course))==-1)
    {
        perror("Error in writing in file: ");
        exit(0);
    }
    //unlock
    write_lock.l_type=F_UNLCK;
    lock_status=fcntl(fd,F_SETLKW,&write_lock);
    write(client_socket_fd,"0Course added successfully",sizeof("0Course added successfully"));   

    sprintf(buff,"0Name : %s\ndept : %s\nFaculty Name : %s\n",cou.name,cou.dept,cou.faculty_name);
        write(client_socket_fd,buff,strlen(buff));

    close(fd);

}


void viewcourse(int client_socket_fd,int facultyid)
{
    
    struct course c;
    char buff[1024];
    // memset(buff,0,sizeof(buff));
    // write(client_socket_fd,"1Enter Course ID:",strlen("1Enter course ID:"));
    // read(client_socket_fd,&buff,sizeof(buff));
    // write(1,buff,sizeof(buff));
    // int id=atoi(buff);

    int fd = open("course.txt",O_RDONLY);
    
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
    //write lock
        lseek(fd,0,SEEK_SET);
        struct flock read_lock = {F_RDLCK,SEEK_CUR,0,0,getpid()};
        int lock_status=fcntl(fd,F_SETLKW,&read_lock);
        if(lock_status==-1) 
        {
            perror("course file lock error:");
            exit(0);
        }
    lseek(fd,0,SEEK_SET);
    int flag=0;
    while(read(fd,&c,sizeof(struct course))>0)
    {
        if(c.active!=0)
        {
        sprintf(buff,"\n0Name : %s\t\tDpt : %s\t\tTotal Seats : %d\t\tAvailable Seats : %d\n",c.name,c.dept,c.totalseats,c.totalseats-c.enrollcount);
        write(client_socket_fd,buff,strlen(buff));
        }
        // if(c.id==id && c.faculty_id==facultyid ) 
        // { 
        //     write(1,"course found",sizeof("course found"));
        //     flag =1;
        //     break;
        // }        
    }
    //Unlock 
    read_lock.l_type=F_UNLCK;
    lock_status=fcntl(fd,F_SETLKW,&read_lock);

    // memset(buff,0,sizeof(buff));
    // if(flag==0) write(client_socket_fd,"0Course not found",strlen("0Course not found"));
    // else{
    //     sprintf(buff,"0Name : %s\nDpt : %s\nTotal Seats : %d\nAvailable Seats : %d",c.name,c.dept,c.totalseats,c.totalseats-c.enrollcount);
    //     write(client_socket_fd,buff,strlen(buff));
    // }

}