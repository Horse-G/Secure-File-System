#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int validate_name(char[]);	
int main (int argc, char *argv[])
{
	char ch,c;
	char target_file[30],text[20],cmd[80];
	char *uname=NULL;
	char *gname=NULL;
	char fname[30],source_file[30];
	FILE *source, *target;
	int count,permission;
	
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
		 strncpy(fname,argv[optind],30);
		 strncpy(source_file,argv[optind+1],30);
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
	  if(validate_name(fname)||validate_name(source_file)){
		  fprintf(stderr,"invalide file name: %s||%s", fname,source_file);
		  return 1;
	  }
	  if(validate_name(uname)||validate_name(gname)){
		  fprintf(stderr,"invalide user/group name: %s||%s", uname, gname);
		  return 1;
	  }
	
	//rename target file
  	strncpy(target_file,uname,20);
	strncat(target_file,".",1);
  	strncat(target_file,fname,50);
	
	//check permission
  	strncpy(cmd,"./objtestacl -u",15);
  	strncat(cmd," ",1);
 	strncat(cmd,uname,15);
  	strncat(cmd," -g ",15);
  	strncat(cmd,gname,15);
  	strncat(cmd," -r w",5);
  	strncat(cmd," ",1);
  	strncat(cmd,fname,30);
  	//printf("cmd: %s\n", cmd);
  	permission = system(cmd);
  	//printf("permission: %d\n",permission);
	
  	if(permission!=0){
		//file doesn't exist
		if(access(target_file,F_OK)==-1){
  		  //create default acl
   		  target = fopen("default","w");		  
  		  fprintf(target, "%s.%s rwxpv", uname,gname);
  		  fclose(target);
		  
		  //create dumy target file
		  target = fopen(target_file, "w");
		  fprintf(target, " ");
		  fclose(target);
		  
  		  //set default ACL using system call
  	    	strncpy(cmd,"./objsetacl -u",15);
  	    	strncat(cmd," ",1);
  	   		strncat(cmd,uname,15);
  	    	strncat(cmd," -g ",15);
  	    	strncat(cmd,gname,15);
  	    	strncat(cmd," ",1);
  	    	strncat(cmd,fname,30);
			strncat(cmd," ",1);
			strncat(cmd,"default",30);
  		  	system(cmd);
		}
		else{//access right denied
  		fprintf(stderr,"permission denied! You are not allowed to write to this object.\n");
  		return 1;
		}
	}
	      source = fopen(source_file, "r");
 
		  
	      if( source == NULL )
	      {
		      fclose(source);
			  perror("Error while opening the file.\n");
	         exit(EXIT_FAILURE);
	      }
 
	      target = fopen(target_file, "w");
 
	      if( target == NULL )
	      {
	         fclose(source);
	         perror("Error while writing the file.\n");
	         exit(EXIT_FAILURE);
	      }
 
	      while( ( ch = fgetc(source) ) != EOF )
	         fputc(ch, target);
 
	      printf("File copied successfully.\n");
 
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

	
		
	


	
	