#include <stdio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void generate_random(char *randval);
int
main(int argc, char **argv)
{
    unsigned char randval[128];
    generate_random(randval);

    printf("main: %s\n", randval);
	BIO_dump_fp(stdout, randval, sizeof(randval));
    return 0;
}

void generate_random(char *randval){
    
    FILE *f;

    f = fopen("/dev/random", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);

    printf("method: %s\n", randval);

}