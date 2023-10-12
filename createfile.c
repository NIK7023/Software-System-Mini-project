#include "headers.h"

//student count
int main()
{
	int ticket=0;
	
	int fd = open("studentcount.txt",O_RDWR);
	write(fd,&ticket,sizeof(ticket));
	
	close(fd);
return 0;
}