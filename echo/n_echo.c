#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main (int argc, char** argv){


	for(int i = 1 ; i < argc ; i++)
	{
		int len = 0 ;
		while(argv[i][len] != '\0')
			len++;
		if(write(1,argv[i],len) < 0  || write(1," ",1) < 0)
		{
			perror("Write Error");
		}
	}

	if(write(1,"\n",1) < 0)
	{
		perror("Write Error");
	}
}
