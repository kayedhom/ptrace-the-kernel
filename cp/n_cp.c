#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NSIZE_MAX 100

int main (int argc, char** argv ){
	int source_fd, dest_fd;
	char buf[NSIZE_MAX];

	if(argc < 3){
		write(1,"There must be at least 2 arguments\n",35);
		return 1;
	}
	if((source_fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("Open Error");
		return 1;
	}
	if((dest_fd = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, 0644)) < 0)
	{
		perror("Open Error");
		return 1;
	}

	int count = 0 ;
	while (  (count = read(source_fd,buf,NSIZE_MAX)) > 0 ){
		if(write(dest_fd,buf,count) < 0)
			perror("Write Error");
	}

	close(source_fd);
	close(dest_fd);
}
