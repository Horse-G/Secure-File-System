#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	DIR *dir;
	struct dirent *ent;
	char c,*uname=NULL,*fname;
	int lflag=0;
	
	while ((c = getopt (argc, argv,"u:l")) != -1){
		//printf("option:%c\n",c);
	    switch (c)
	      {
	      case 'u':
	        uname=optarg;
	        break;
	      case 'l':
		  	lflag=1;
	        break;
	      case '?':
	        fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
			break;
	      default:
	        abort ();	
	      }  
	  }
	  if(uname==NULL){
		  uname=(char*)malloc(50);
		  sprintf(uname,"%d",getuid());
	  }
	  strncat(uname,".",1);
	  //printf("uname: %s, lflage: %d\n", uname,lflag);
	  
	if ((dir = opendir (".")) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
		  if(strstr(ent->d_name,uname)!=0){
			printf("%.*s\n",(int)strlen(ent->d_name), ent->d_name+strlen(uname) );
			if(lflag==1){
				printf("size: %d bytes\n",ent->d_reclen);
			}
		  }
	  }
	  closedir (dir);
	  return 0;
	} else {
	  /* could not open directory */
	  perror ("");
	  return EXIT_FAILURE;
	}
}