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
void viewenrolledcourse(int client_socket_fd, int studentid);
void enroll_in_newcourse(int client_socket_fd, int studentid);
void unenroll_from_course(int client_socket_fd, int studentid);

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
    char menu[1024] = "1\n------------------Welcome student------------------\n1.View Enrolled Courses\n2.Enroll in new course\n3.Unenroll\n4.\n7.Logout and exit\nEnter Choice :";

    char buff = '0';
    do
    {

        write(client_socket_fd, menu, sizeof(menu));
        buff = '0';
        read(client_socket_fd, &buff, sizeof(buff));

        switch (buff)
        {
        case '1':
            viewenrolledcourse(client_socket_fd, s->id);
            break;

        case '2':
            enroll_in_newcourse(client_socket_fd, s->id);
            write(1, "complete", sizeof("complete"));
            break;

        case '3':
            unenroll_from_course(client_socket_fd, s->id);
            break;

        default:
            break;
        }
    } while (buff != '7');
}

void viewenrolledcourse(int client_socket_fd, int studentid)
{
    int fd = open("studentcourse.txt", O_RDWR);
    if (fd == -1)
    {
        perror("Error in file opening:");
    }

    struct studentcourse sc;
    char buff[1024];
    while (read(fd, &sc, sizeof(struct studentcourse)) > 0)
    {
        //(buff,"0Course name : %s \tFaculty name : %s\t Dept : %s\t\n",sc.coursename,sc.facultyname,sc.dept);
        // write(1,buff,strlen(buff));
        memset(buff, 0, sizeof(buff));
        if (sc.studentid == studentid && sc.courseid != -1)
        {
            sprintf(buff, "0Course name : %s \tFaculty name : %s\t Dept : %s\t", sc.coursename, sc.facultyname, sc.dept);
            write(1, buff, strlen(buff));
            write(client_socket_fd, buff, strlen(buff));
        }
    }
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

    //     if (c.active != -1)
    //     {
    //         memset(buff, 0, sizeof(buff));
    //         sprintf(buff,"0\ncourse ID : %d\tName : %s\t\tDpt : %s\t\tFaculty Name : %s\t\tAvailable Seats : %d", c.id, c.name, c.dept, c.faculty_name, c.totalseats - c.enrollcount);
    //         write(client_socket_fd, buff, strlen(buff));
    //     }
    // }
    write(client_socket_fd, "1Enter course ID to enroll:", strlen("1Enter course ID to enroll:"));
    // write(client_socket_fd, "1", strlen("1"));
    memset(buff, 0, sizeof(buff));
    int read_byte = read(client_socket_fd, buff, sizeof(buff));
    // printf("read byte %d buff:%s\n",read_byte,buff);
    if (read_byte == -1)
    {
        perror("read error");
    }
    write(1, buff, sizeof(buff));
    // buff[read_byte-1]='\0';
    int courseid = atoi(buff);

    int std_cou_fd = open("studentcourse.txt", O_RDWR);
    if (std_cou_fd == -1)
    {
        perror("Error in file opening studentcourse:");
    }

    int flag = 0;
    struct studentcourse stcu;
    lseek(std_cou_fd, 0, SEEK_SET);
    while (read(std_cou_fd, &stcu, sizeof(struct studentcourse)) > 0)
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
            if (c.id == courseid)
            {
                if (c.active == 1)
                {
                    if ((c.totalseats - c.enrollcount) == 0)
                    {
                        write(client_socket_fd, "0No Seat in selected course\n", strlen("0No Seat in selected course\n"));
                        break;
                    }
                    else
                    {
                        c.enrollcount++;
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &c, sizeof(struct course));
                        struct studentcourse temp;
                        temp.courseid = courseid;
                        temp.studentid = studentid;
                        strcpy(temp.dept, c.dept);
                        strcpy(temp.coursename, c.name);
                        strcpy(temp.facultyname, c.faculty_name);
                        lseek(std_cou_fd, 0, SEEK_END);
                        write(std_cou_fd, &temp, sizeof(struct studentcourse));
                        write(client_socket_fd, "0Enrolled in new course", strlen("0Enrolled in new course"));
                        break;
                    }
                }
                else
                {
                    write(client_socket_fd, "0Selected an inactive course\n", strlen("0Selected an inactive course\n"));
                    break;
                }
            }
        }
    }

    write_lock.l_type = F_UNLCK;
    lock_status = fcntl(fd, F_SETLKW, &write_lock);
    fflush(stdin);
    fflush(stdout);
    close(fd);
    close(std_cou_fd);
}

void unenroll_from_course(int client_socket_fd, int studentid)
{
    char buff[1024];
    write(client_socket_fd, "1Enter course Id to unenroll:", strlen("1Enter course Id to unenroll:"));
    memset(buff, 0, sizeof(buff));
    read(client_socket_fd, buff, sizeof(buff));
    
    
    write(1,buff,strlen(buff));

    int courseid = atoi(buff);
    if (courseid == -1)
    {
        write(client_socket_fd, "0Invalid course id", strlen("0Invalid course id"));
        return;
    }

    int fd = open("studentcourse.txt", O_RDWR);
    if (fd == -1)
    {
        perror("Error in file opening:");
    }

    // read lock
    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, getpid()};
    int lock_status = fcntl(fd, F_SETLKW, &lock);
    if (lock_status == -1)
    {
        perror(" file lock error:");
        exit(0);
    }
    struct studentcourse sc;
    lseek(fd, 0, SEEK_SET);
    int flag = 0;
    while (read(fd, &sc, sizeof(struct studentcourse)) > 0)
    {
        if (sc.studentid == studentid && sc.courseid == courseid)
        {
        printf("sc id:%d sc cid :%d\n",sc.studentid,sc.courseid);
            flag = 1;
            printf("sc id:%d sc cid :%d\n",sc.studentid,sc.courseid);
            break;
        }
    }

    if (flag == 1)
    {
        lseek(fd, -sizeof(struct studentcourse), SEEK_CUR);        

        sc.courseid = -1;
        if (write(fd, &sc, sizeof(struct studentcourse)) == -1)
        {
            perror("Error in writing in file: ");
            exit(0);
        }

        int course_fd = open("course.txt", O_RDWR);
        if (course_fd == -1)
        {
            perror("Error in file opening:");
        }
        struct flock couse_write_lock = {F_WRLCK, SEEK_CUR, 0, 0, getpid()};
        int course_lock_status = fcntl(course_fd, F_SETLKW, &couse_write_lock);
        if (lock_status == -1)
        {
            perror(" file lock error:");
            exit(0);
        }
        struct course c;
        lseek(course_fd, 0, SEEK_SET);
        while (read(course_fd, &c, sizeof(struct course)) > 0)
        {
            if (c.id == courseid)
            {
                lseek(course_fd, -sizeof(struct course), SEEK_CUR);
                c.enrollcount--;

                if (write(course_fd, &c, sizeof(struct course)) == -1)
                {
                    perror("Error in writing in file: ");
                    exit(0);
                }
                write(client_socket_fd, "0Unenrolled", strlen("0Unenrolled"));
                break;
            }
        }

        // unlock
        // close
    }
    else
    {
        write(client_socket_fd, "0Not enrolled in any course with the given courseid", strlen("0Not enrolled in any course with the given courseid"));
    }

    lock.l_type = F_UNLCK;
    lock_status = fcntl(fd, F_SETLKW, &lock);
}