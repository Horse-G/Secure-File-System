#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

int main(int arc, char *argv[])
{
	
	unsigned char randval[32];
    FILE *f;

    f = fopen("/dev/random", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);
	

    printf("%s\n", randval);
	BIO_dump_fp(stdout, randval, sizeof(randval));

  /* A 256 bit key */
	unsigned char *key=randval;
  //memcpy(key,randval,16);

  /* A 128 bit IV */
	unsigned char iv[16];
	memcpy(key,&randval[15],16);
  
  printf("key: %s, iv: %s\nkey:\n",key,iv);
  BIO_dump_fp(stdout, key, sizeof(key));
  printf("iv:\n");
  BIO_dump_fp(stdout, iv, sizeof(iv));
  
  //unsigned char key[] = "0123456789abcdeF";
  //unsigned char iv[] = "1234567887654321";
  
  
	FILE *in = fopen("testfile.txt","r");
	FILE *out = fopen("text.ept","w");
	
	do_file_crypt(in,out,key,iv,1);
	fclose(in);
	fclose(out);
	
	in = fopen("text.ept","r");
	out = fopen("decephered.txt","w");
	do_file_crypt(in,out,key,iv,0);
	

}


int do_file_crypt(FILE *in, FILE *out, unsigned char key[],unsigned char iv[],int do_encrypt)
        {
        /* Allow enough space in output buffer for additional block */
        unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
        int inlen, outlen;
        EVP_CIPHER_CTX ctx;
        EVP_CIPHER_CTX_init(&ctx);
        EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL,
                do_encrypt);
        OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
        OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);
        /* Now we can set key and IV */
        EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);
        for(;;)
                {
                inlen = fread(inbuf, 1, 1024, in);
                if(inlen <= 0) break;
                if(!EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, inlen))
                        {
                        /* Error */
                        EVP_CIPHER_CTX_cleanup(&ctx);
                        return 0;
                        }
                fwrite(outbuf, 1, outlen, out);
                }
        if(!EVP_CipherFinal_ex(&ctx, outbuf, &outlen))
                {
                /* Error */
                EVP_CIPHER_CTX_cleanup(&ctx);
                return 0;
                }
        fwrite(outbuf, 1, outlen, out);
        EVP_CIPHER_CTX_cleanup(&ctx);
        return 1;
        }