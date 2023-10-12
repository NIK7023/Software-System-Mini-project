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



//get student and faculty count
int getcount(char *file)
{
    int count;
	int fd = open(file,O_RDWR);
    if(fd==-1)
    {
        perror("Error in opeing file: ");
        exit(0);
    }
	if(read(fd,&count,sizeof(count)) ==-1)
    {
        perror("Error in reading:");
    }
    count=count+1;
    lseek(fd,0,SEEK_SET);
	if(write(fd,&count,sizeof(count)) ==-1)
    {
        perror("Error in writing:");
    }
	close(fd);
return count;
}
