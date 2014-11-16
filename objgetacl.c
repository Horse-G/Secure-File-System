#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	char c,ch[50],cmd[80];
	char acl_file[50];
	char *uname=NULL, *gname=NULL,*fname;
	char *buname,*bgname,*list,*res;	
	int permission;
	FILE *acl;
	
	while ((c = getopt (argc, argv,"u:g:")) != -1){
		//printf("option:%c\n",c);
	    switch (c)
	      {
	      case 'u':
	        uname=optarg;
	        break;
	      case 'g':
	        gname=optarg;
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
		 fname=(char*)malloc(50);
  		 strncpy(fname,argv[optind],30);
  	}
 
    if(uname==NULL){
  	  uname=(char*)malloc(50);
  	  sprintf(uname,"%d",getuid());
    }
    if(gname==NULL){
  	  gname=(char*)malloc(50);
  	  sprintf(gname,"%d",getgid());
    }
	
	  strncpy(acl_file,fname,30);
	  strncat(acl_file,".acl",5);
	
	  //permission validation
  		strncpy(cmd,"./objtestacl -u",15);
  		strncat(cmd," ",1);
 		strncat(cmd,uname,15);
  		strncat(cmd," -g ",15);
  		strncat(cmd,gname,15);
  		strncat(cmd," -r v",5);
  		strncat(cmd," ",1);
  		strncat(cmd,fname,30);
		permission=system(cmd);
	
		if(permission!=0){
			fprintf(stderr, "You are not allowed to view the ACL!\n");
			return 1;
		}
	
	//potential work imprve the readin file machenism
	acl = fopen(acl_file,"r");
	
    if( acl == NULL )
    {
      fclose(acl);
	  perror("No ACL for such file. \n");
      exit(EXIT_FAILURE);
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
				printf("%s\n",list);
				return 0;
			}		
		}
		
	}
	//no matched acl
	printf("no defined ACL found!\n");
	return 1;
	
}