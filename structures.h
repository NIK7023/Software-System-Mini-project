// #include "headers.h"
#include<stdio.h>

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
};
