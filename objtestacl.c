#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	char c;
	char ch[50];
	char acl_file[50];
	char *buname,*bgname,*list,*res;
	char *uname=NULL,*gname=NULL,*operation;
	FILE *acl;
	
	//argument parsing
	while ((c = getopt (argc, argv,"u:g:r:")) != -1){
		//printf("option:%c\n",c);
	    switch (c)
	      {
	      case 'u':
	        uname=optarg;
	        break;
	      case 'g':
	        gname=optarg;
	        break;
	      case 'r':
	        operation=optarg;
	        break;	
	      case '?':
	        fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
			break;
	      default:
	        abort ();
			
	      }  
	  }
	 
	if(optind>=argc){
		fprintf(stderr,"file name required!\n");
		return 1;
	}else{
		 strncpy(acl_file,argv[optind],30);
		 strncat(acl_file,".acl",8);
	}
 
  if(uname==NULL){
	  uname=(char*)malloc(50);
	  sprintf(uname,"%d",getuid());
  }
  if(gname==NULL){
	  gname=(char*)malloc(50);
	  sprintf(gname,"%d",getgid());
  }
	
	
  
	//potential work imprve the readin file machenism
	acl = fopen(acl_file,"r");
	
    if( acl == NULL )
    {
      //fclose(acl);
      fprintf(stderr,"No ACL for such file. \n");
      return 1;
    }
	
	while(fgets(ch,50,acl)!= NULL)
	{
		//parsing the entry into username, groupname and ACL
		res=strtok(ch,".");
		buname=res;
		res=strtok(NULL," ");
		bgname=res;
		res=strtok(NULL," ");
		list=res;
		
		//check entry fields
		if((strcmp(buname,uname) * strcmp(buname,"*"))==0){
			if((strcmp(bgname,gname) * strcmp(bgname,"*"))==0){
				if(strpbrk(list,operation)){
					printf("allowed\n");
					return 0;
				}else{
					printf("denied\n");
					return 1;
				}
			}		
		}
		
	}
	//no matched acl
	printf("no defined ACL found!\n");
	return 1;
	
}
