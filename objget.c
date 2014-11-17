#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#if defined(__APPLE__)
#  define COMMON_DIGEST_FOR_OPENSSL
#  include <CommonCrypto/CommonDigest.h>
#  define SHA1 CC_SHA1
#else
#  include <openssl/md5.h>
#endif

int validate_name(char[]);	
char *str2md5(const char *str, int length);

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
		  break;
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
	
  	//MD5 Passphrase
  	printf("original passphrase: %s\n",passphrase);
  	passphrase=str2md5(passphrase,strlen(passphrase));
  	printf("encrypted passphrase: %s\n",passphrase);
	
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

char *str2md5(const char *str, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
}
