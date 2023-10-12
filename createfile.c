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

#include "structures.h"

//student count
int main()
{
	int ticket=0;
	
	int fd1 = open("facultycount.txt",O_RDWR);
    int fd2 = open("studentcount.txt",O_RDWR);
	write(fd1,&ticket,sizeof(ticket));
    write(fd2,&ticket,sizeof(ticket));
	
	close(fd1);
    close(fd2);
return 0;
}