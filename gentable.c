#include <stdio.h>
#include "aes.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

void assign(unsigned char *pass, unsigned long long val);
void reduce(unsigned char *pass, int bits, int num);

int main(int argc, char *argv[]){
	int n = atoi(argv[1]);
	int s = atoi(argv[2]);
	
	FILE *rainbow = fopen("rainbow", "w");	
	
	unsigned long long i;
	
	aes_context ctx;	
	unsigned char pass[16] = {};	
	unsigned char plaintext [16]= {};
	unsigned char ciphertext[16] = {};
	
	unsigned char buffer[24] = {}; 
	
	int *bitmap = calloc(exp2(n), sizeof(int));
	int j;
	for(i = 0; i < exp2(n)-1; i++){
		if(bitmap[i] == 0){
			assign(pass, i);
			memcpy(buffer, &pass[8], 8*sizeof(char));
			for(j = 0; j < exp2(n-s); j++){
				bitmap[i] = 1;

				aes_setkey_enc(&ctx, pass, 128);
				aes_crypt_ecb(&ctx, AES_ENCRYPT, plaintext, ciphertext);
				
				memset(pass, 0, sizeof(pass));
				strcpy(pass, ciphertext);
				reduce(pass, n, j);	
				//converts back to an integer
				i = (long long)strtol(pass, NULL, 16);
			}
			memcpy(&buffer[8], ciphertext, 16*sizeof(char));
			fwrite(buffer, sizeof(buffer), 1, rainbow);
		}
	}  
	
	fclose(rainbow);
}
/*
Following two functions provided by Tygar
*/
void assign(unsigned char *pass, unsigned long long val){
        int i;
        for (i = 15; i >= 8; i--){
		pass[i] = (unsigned char) val & 0xFF;
		val >>= 8;
        }
	for (i =7; i >= 0; i--){
		pass[i] = 0;
	}
}

void reduce(unsigned char *pass, int bits, int num){
	int i;
	if (bits < 0 || bits > 128){
		fprintf(stderr, "Error: invalid value in reduce:  %d\n", bits);
		exit (-1);
	}

        for (i=15; i >= 0; i--, bits -= 8)
                if (bits <= 0)
                        pass[i] = 0;
                else if (bits < 8)
                        pass[i] &= (0xFF >> (8-bits));
	pass[15] = (char) num;
}
