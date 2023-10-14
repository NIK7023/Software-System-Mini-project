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
void display_student_menu(int client_socket_fd, struct student *s);
void enroll_in_newcourse(int client_socket_fd, int studentid);

bool student_login(int client_socket_fd)
{
    printf("\n In Student Login\n");
    char username[1024], password[1024];
    char ubuff[1024] = "1Enter Username :\n";
    char pbuff[1024] = "1Enter Password :\n";

    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));

    write(client_socket_fd, ubuff, strlen(ubuff));
    // sleep(3);
    read(client_socket_fd, username, sizeof(username));
    // sleep(3);
    write(client_socket_fd, pbuff, strlen(pbuff));
    // sleep(3);
    read(client_socket_fd, password, sizeof(password));
    printf("username %s", username);
    printf("Password %s\n", password);
    // sleep(3);
    // write(1,password,sizeof(password));
    // printf("\nPassword %s",password);

    // write(1,username,s(username));
    struct student s;
    int flag = 0;
    int fd = open("student.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error in file opening:");
    }
    while (read(fd, &s, sizeof(struct student)) > 0)
    {
        // write(1,&s.username,strlen(s.username));
        // printf("%s\n",s.username);
        if (strcmp(s.username, username) == 0)
        {
            write(1, "username found", sizeof("username found"));
            flag = 1;
            
            break;
        }
    }
    close(fd);
    // write(1,&s.username,strlen(s.username));
    // write(1,&s.password,strlen(s.password));
    if (strcmp(s.username, username) == 0 && strcmp(s.password, password) == 0)
    {
        display_student_menu(client_socket_fd, &s);
        return true;
    }
    else
        return false;
}

void display_student_menu(int client_socket_fd, struct student *s)
{
    char menu[1024] = "1\n------------------Welcome student------------------\n1.View Enrolled Courses\n2.Enroll in new course\n3.\n7.Logout and exit\nEnter Choice :";

    char buff = '0';
    do
    {

        write(client_socket_fd, menu, sizeof(menu));
        buff = '0';
        read(client_socket_fd, &buff, sizeof(buff));

        switch (buff)
        {
        case '1':
            break;

        case '2':
            enroll_in_newcourse(client_socket_fd, s->id);
            write(1,"complete",sizeof("complete"));
            break;

        default:
            break;
        }
    } while (buff != '7');
}

void enroll_in_newcourse(int client_socket_fd, int studentid)
{
    int fd = open("course.txt", O_RDWR);
    if (fd == -1)
    {
        perror("Error in file opening:");
    }
    // write lock
    struct flock write_lock = {F_WRLCK, SEEK_CUR, 0, 0, getpid()};
    int lock_status = fcntl(fd, F_SETLKW, &write_lock);
    if (lock_status == -1)
    {
        perror(" file lock error:");
        exit(0);
    }

    struct course c;
    char buff[1024];
    lseek(fd, 0, SEEK_SET);
    // while (read(fd, &c, sizeof(struct course)) > 0)
    // {
        
    //     if (c.active != 0)
    //     {
    //         memset(buff, 0, sizeof(buff));
    //         sprintf(buff,"0\ncourse ID : %d\tName : %s\t\tDpt : %s\t\tFaculty Name : %s\t\tAvailable Seats : %d", c.id, c.name, c.dept, c.faculty_name, c.totalseats - c.enrollcount);
    //         write(client_socket_fd, buff, strlen(buff));
    //     }
    // }
    write(client_socket_fd, "1Enter course ID to enroll:", strlen("1Enter course ID to enroll:"));
    //write(client_socket_fd, "1", strlen("1"));
    memset(buff, 0, sizeof(buff));
    int read_byte= read(client_socket_fd, buff, sizeof(buff));
    //printf("read byte %d buff:%s\n",read_byte,buff);
    if(read_byte==-1)
    {
        perror("read error");
    }
    write(1, buff, sizeof(buff));
    //buff[read_byte-1]='\0';
    int courseid = atoi(buff);

    int std_cou_fd = open("studentcourse.txt", O_RDWR);
    if (std_cou_fd == -1)
    {
        perror("Error in file opening studentcourse:");
    }

    int flag = 0;
    struct studentcourse stcu;
        lseek(std_cou_fd, 0, SEEK_SET);
    while (read(std_cou_fd, &stcu, sizeof(struct studentcourse)) >0)
    {
        memset(buff, 0, sizeof(buff));
        if (stcu.studentid == studentid && stcu.courseid == courseid)
        {
            flag = 1;
            break;
        }
    }

    if (flag == 1)
    {
        write(client_socket_fd, "0Already Enrolled in course\n", strlen("0Already Enrolled in course\n"));
    }
    else
    {
        lseek(fd, 0, SEEK_SET);
        while (read(fd, &c, sizeof(struct course)) > 0)
        {
            memset(buff, 0, sizeof(buff));
            sprintf(buff, "0\ncourse ID : %d\tName : %s\t\tDpt : %s\t\tFaculty Name : %s\t\tAvailable Seats : %d", c.id, c.name, c.dept, c.faculty_name, c.totalseats - c.enrollcount);
            write(1, buff, strlen(buff));
            if (c.id==courseid)
            {
                if(c.active==1)
                {
                    if( (c.totalseats-c.enrollcount)==0 )
                    {
                        write(client_socket_fd,"0No Seat in selected course\n",strlen("0No Seat in selected course\n"));
                        break;
                    }
                    else
                    {
                        c.enrollcount++;
                        lseek(fd,-sizeof(struct course),SEEK_CUR);
                        write(fd,&c,sizeof(struct course));
                        struct studentcourse temp;
                        temp.courseid=courseid;
                        temp.studentid=studentid;
                        lseek(std_cou_fd,0,SEEK_END);
                        write(std_cou_fd,&temp,sizeof(struct studentcourse));
                        write(client_socket_fd,"0Enrolled in new course",strlen("0Enrolled in new course"));
                        break;
                    }
                }
                else
                {
                    write(client_socket_fd,"0Selected an inactive course\n",strlen("0Selected an inactive course\n"));
                    break;
                }
            }
        }
    }

write_lock.l_type=F_UNLCK;
lock_status=fcntl(fd,F_SETLKW,&write_lock);
fflush(stdin);
fflush(stdout);
close(fd);
close(std_cou_fd);
}