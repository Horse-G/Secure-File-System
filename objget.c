#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int validate_name(char[]);	
int main(int argc, char *argv[]){
	FILE *fp;
	char buff[255],cmd[80];
	char c;
	char *uname=NULL,*gname=NULL,*passphrase=NULL;
	char target_file[50],fname[30];
	int permission,bflag;
	
	
	//argument parsing
    opterr = 0;
    while ((c = getopt (argc, argv, "u:g:k:")) != -1)
      switch (c)
        {
        case 'u':
  	  	uname = optarg;
          bflag = 1;
          break;
        case 'g':
          gname = optarg;
  		bflag = 1;
          break;
        case 'k':
          passphrase = optarg;
        case '?':
          if (optopt == 'u'||optopt == 'g')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          else
            fprintf (stderr,
                     "Unknown option character `\\x%x'.\n",
                     optopt);
          return 1;
        default:
          abort ();
        }
		if(optind>=argc){
			fprintf(stderr,"file name required!\n");
			return 1;
		}else{
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
	  //file name validation
	  if(validate_name(fname)){
		  fprintf(stderr,"invalide file name: %s", fname);
		  return 1;
	  }
	
	//check permission
	  strncpy(cmd,"./objtestacl -u",15);
	  strncat(cmd," ",1);
	  strncat(cmd,uname,15);
	  strncat(cmd," -g ",15);
	  strncat(cmd,gname,15);
	  strncat(cmd," -r x",5);
	  strncat(cmd," ",1);
	  strncat(cmd,fname,30);
	  permission = system(cmd);
	
	  if(permission!=0){
	  	fprintf(stderr,"permission denied! You are not allowed to get this object.\n");
	  	return 1;
	  }
	  
	//renaming file name
  	strncpy(target_file,uname,20);
	strncat(target_file,".",1);
  	strncat(target_file,fname,35);
	
	fp = fopen(target_file, "r");
	if( fp != NULL ){
	      while ( !feof(fp ) ){
	         memset(buff, '\0', sizeof( buff) );
	         fgets(buff, 255, (FILE*)fp);
	         printf("%s", buff );
	      }
	      fclose(fp);
	   }
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
