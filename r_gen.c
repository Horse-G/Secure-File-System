#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void generate_random(int length, char *buffer);

int main(int arg, char * args[]){
	char data[30];
	generate_random(30,data);
	printf("random data: %s\n",data);
	return 0;
}

void generate_random(int length, char *buffer){
	FILE *fp;
	fp=fopen("dev/urandom", "r");
	if(fp==NULL)
		fprintf(stderr,"can't read random!");
	fread(&buffer, 1, length, fp);
	fclose(fp);
}