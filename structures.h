struct message
{
    int type;  //0 for read and 1 for write
    char msg[1024];
};


struct student
{
    char username[20];
    char password[20];
    int id;
    bool active;
    char name[20];
    int course_enrolled[30];
};
