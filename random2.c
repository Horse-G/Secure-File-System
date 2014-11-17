#include <stdio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

int
main(int argc, char **argv)
{
    
	unsigned char randval[128];
    FILE *f;

    f = fopen("/dev/random", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);
	

    printf("%s\n", randval);
	BIO_dump_fp(stdout, randval, sizeof(randval));
    return 0;
}