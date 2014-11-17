#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int generate_random(int length, char *buffer);

int main(int arg, char * args[]){
	
	int randomData = open("/dev/random", O_RDONLY);
	int myRandomInteger;
	size_t randomDataLen = 0;
	while (randomDataLen < sizeof myRandomInteger)
	{
	    ssize_t result = read(randomData, ((char*)&myRandomInteger) + randomDataLen, (sizeof myRandomInteger) - randomDataLen);
	    if (result < 0)
	    {
	        // error, unable to read /dev/random 
	    }
	    randomDataLen += result;
	}
	close(randomData);
	/*
	char data[30];
	generate_random(30,data);
	printf("random data: %s\n",data);
	*/
	return 0;
	
}

int generate_random(int length, char *buffer){
	FILE *fp;
	fp=fopen("/dev/urandom", "r");
	if(fp==NULL){
		fprintf(stderr,"can't read random!");
		return 1;
	}
	fread(&buffer, 1, length, fp);
	fclose(fp);
	return 0;
}