#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define COUNT 100

char path[COUNT];

int main (int argc, char** argv){

	if(getcwd(path, sizeof(path)) < 0 )
		perror("GETCWD ERROR");
	if(write(1,path,sizeof(path)) < 0 )
		perror("Write Error");
	if(write(1,"\n",1) < 0)
	{
		perror("Write Error");
	}
}
