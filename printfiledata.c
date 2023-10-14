#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
struct count
{
    int student;
    int faculty;
    int course;
};


struct student
{
    char username[20];
    char password[20];
    int id;
    int active;         //0 inactive 1 active
    char name[20];
    char email[20];
    int age;
    int course_enrolled[5];       //can enroll in 10 course
};

struct faculty
{
    char username[20];
    char password[20];
    int id;
    char name[20];
    char email[20];
    int age;
    int course[10];
};


struct course
{
    int id;
    int faculty_id;
    char faculty_name[20];
    int active;
    char name[20];
    char dept[20];
    int totalseats;     //Max No of enrollment
    int enrollcount;    //No of student enrolled
};


struct studentcourse
{
    int studentid;
    int courseid;
    char coursename[20];
    char facultyname[20];
    char dept[20];
};

int main()
{
    int fd1= open("studentcourse.txt",O_CREAT | O_RDWR, 0777);
    struct studentcourse sc;
    while (read(fd1,&sc,sizeof(struct studentcourse))>0)
    {
        printf("\nstudent id:%d \tcourse id:%d",sc.studentid,sc.courseid  );
    }
    
    printf("\n\n--------------------------------------------------\n\n");

    int fd2= open("student.txt",O_CREAT | O_RDWR, 0777);
    struct student s;
    while (read(fd2,&s,sizeof(struct student))>0)
    {
        printf("\n Student Username:\t%s ",s.username );
    }
    printf("\n\n--------------------------------------------------\n\n");
    
    int fd3= open("faculty.txt",O_CREAT | O_RDWR, 0777);
    struct faculty f;
    while (read(fd3,&f,sizeof(struct faculty))>0)
    {
        printf("\nFaculty Username:\t%s",f.username );
    }
    printf("\n\n--------------------------------------------------\n\n");
    
    int fd4= open("course.txt",O_CREAT | O_RDWR, 0777);
    struct course c;
    while (read(fd4,&c,sizeof(struct course))>0)
    {
        printf("course ID : %d\tName : %s\t\tDpt : %s\t\tFaculty Name : %s\t\tAvailable Seats : %d\n", c.id, c.name, c.dept, c.faculty_name, c.totalseats - c.enrollcount);
    }




return 0;
}