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
	struct count c;
	c.student=0;
	c.faculty=0;
	int fd = open("count.txt",O_CREAT | O_RDWR, 0777);
	int fd1 = open("student.txt",O_CREAT | O_RDWR, 0777);
	int fd2 = open("faculty.txt",O_CREAT | O_RDWR, 0777);
	if(fd==-1) perror("count.txt");
	if(fd1==-1) perror("student.txt");
	if(fd2==-1) perror("faculty.txt");

	 int bytewrite=write(fd,&c,sizeof(struct count));
	 if(bytewrite<0) perror("Error while writing in count.txt");
	close(fd);
	close(fd1);
	close(fd2);
return 0;
}