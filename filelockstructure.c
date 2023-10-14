//file open
int fd=open(".txt",O_RDWR);
    if(fd==-1)
    {
        perror("Error in file opening:");
    }

//file write error
if(write(,&,sizeof(struct ))==-1)
{
    perror("Error in writing in file: ");
    exit(0);
}


//write lock
lseek(,0,);
int lock_length = sizeof(struct );
struct flock write_lock = {F_WRLCK,SEEK_CUR,0,lock_length,getpid()};
int lock_status=fcntl(,F_SETLKW,&write_lock);
if(lock_status==-1) 
{
    perror(" file lock error:");
    exit(0);
}

//write unlock
write_lock.l_type=F_UNLCK;
lock_status=fcntl(,F_SETLKW,&write_lock);


//read lock
struct flock read_lock={F_RDLCK,SEEK_SET,0,0,getpid()};
int lock_status=fcntl(,F_SETLKW,&read_lock);
if(lock_status==-1) 
{
    perror(" file lock error:");
    exit(0);
}

//read unlock
read_lock.l_type=F_UNLCK;
lock_status=fcntl(,F_SETLKW,&read_lock);


//file read
while (read(, &, sizeof(struct )) >0)
{    
    if ()
    {
        
    }
}