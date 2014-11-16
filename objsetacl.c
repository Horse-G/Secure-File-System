#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int validate_name(char[]);

int main(int argc, char *argv[]){
	char c,ch,file_name[50],cmd[80];
	FILE *source,*target;
	char *uname=NULL,*gname=NULL,*fname,*aclname;
	char ufname[50];
	int permission;
	
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
	  
	if((optind+1)>=argc){
		fprintf(stderr,"file name required!\n");
		return 1;
	}else{
		 fname=(char*)malloc(50);
		 aclname=(char*)malloc(50);
		 strncpy(fname,argv[optind],30);
		 strncpy(aclname,argv[optind+1],30);
	}
 
  if(uname==NULL){
	  uname=(char*)malloc(50);
	  sprintf(uname,"%d",getuid());
  }
  if(gname==NULL){
	  gname=(char*)malloc(50);
	  sprintf(gname,"%d",getgid());
  }
	  
	  //file name validation
	  if(validate_name(fname)||validate_name(aclname)){
		  fprintf(stderr,"invalide file name!");
		  return 1;
	  }
	  if(validate_name(uname)||validate_name(gname)){
		  fprintf(stderr,"invalide user/group name: %s %s", uname, gname);
		  return 1;
	  }
	
  	//don't allow to set ACL if the file doesn't existe
	  strncpy(ufname,uname,20);
	  strncat(ufname,".",1);
	  strncat(ufname,fname,20);

  	  if(access(ufname,F_OK)==-1){
  		fprintf(stderr,"You are not allowed to set ACL if the file doesn't existe!\n");
  		return 1;
  	  }
	  //permission validation
	  //bypass this check if acl doesn't exist
  	  strncpy(file_name,fname,40);
  	  strncat(file_name,".acl",4);
	  
	  if(access(file_name,F_OK)!=-1){
    	strncpy(cmd,"./objtestacl -u",15);
    	strncat(cmd," ",1);
   		strncat(cmd,uname,15);
    	strncat(cmd," -g ",15);
    	strncat(cmd,gname,15);
    	strncat(cmd," -r p",5);
    	strncat(cmd," ",1);
    	strncat(cmd,fname,30);
		permission=system(cmd);
		
		if(permission!=0){
			fprintf(stderr, "You are not allowed to change the ACL!\n");
			return 1;
		}
	}
	
	//writing file machanism
	
	source = fopen(aclname,"r");
	target = fopen(file_name,"w");
	
	if(source== NULL){
		fclose(source);
		perror("Error while opening the source file");
		exit(EXIT_FAILURE);
	}
	
	while((ch = fgetc(source))!= EOF){
		fputc(ch,target);
	}
	
	printf("ACL added successfully.\n");
	
	fclose(source);
	fclose(target);
	
	return 0;
}

int validate_name(char s[]){
	int pt=0;
	while(s[pt]!='\0'){
		if(!((s[pt]>='0'&&s[pt]<='9')
			||(s[pt]>='a'&&s[pt]<='z')	
			||(s[pt]>='A'&&s[pt]<='Z')
			||s[pt]=='_'||s[pt]=='.')){
			return 1;
		} 
	pt++;
	}
	return 0;
}
