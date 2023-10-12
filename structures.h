// #include "headers.h"
#include<stdio.h>

struct student
{
    char username[20];
    char password[20];
    int id;
    int active;         //0 inactive 1 active
    char name[20];
    char email[20];
    char age[3];
    char course_enrolled[10][30];       //can enroll in 10 course
};
