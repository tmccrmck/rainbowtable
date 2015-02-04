#include <stdio.h>
#include "math.h"
#include "stdlib.h"
#include "aes.h"
#include "string.h"

unsigned char asciitohex(char c);
int cmphex(unsigned char *first, unsigned char *second);

void main(int argc, char *argv[]){
        int n = atoi(argv[1]);
        int s = atoi(argv[2]);
	unsigned char *hash = argv[3];
	unsigned char hex_hash[16];
	int i, j;
	int count;	

	/* Converts ascii char array to hex. Provided by Tygar */
        for (i=2, j=0; j<16; i += 2, j++)
		hex_hash[j] = (asciitohex(hash[i])<<4) | (asciitohex(hash[i+1]));
	
	aes_context ctx;
	unsigned char buffer[24] = {};
	unsigned char pass[8] = {};
	unsigned char last[16] = {};	
	unsigned char ciphertext[16] = {};
	unsigned char plaintext[16] = {};
	
	FILE *rainbow = fopen("rainbow", "r");	

	fread(buffer, 24*sizeof(char), 1, rainbow);
	memcpy(pass, buffer, 8*sizeof(char));
	memcpy(last, &buffer[8], 16*sizeof(char));
	
	/* Same reduce function as from gentable.c */
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
	
	while(cmphex(hex_hash, last) != 0){
		//reduce(hash, n, i);
		//aes_setkey_enc(&ctx, hash, 128);
                //aes_crypt_ecb(&ctx, AES_ENCRYPT, plaintext, ciphertext);		
	
		fread(buffer, 24*sizeof(char), 1, rainbow);
		memcpy(pass, buffer, 8 * sizeof(char));
		memcpy(last, &buffer[8], 16*sizeof(char));			
		
		count++;		
	}
	
	printf("Password is 0x");
	for(i = 0; i < 8; i++){
		printf("%02x", pass[i]);
	}
	printf(". AES was evaluated %d times", count);
	
	fclose(rainbow);
}
/* From Tygar */
unsigned char asciitohex(char c){
    if (('0'<=c) && (c<='9'))
        return (c-'0');
    else if (('a'<=c) && (c<='f'))
        return (c-'a'+10);
    else if (('A'<=c) && (c<='F'))
        return (c-'A'+10);
    else
        error("Hash value can contain digits 0-9, a-f only");
}
/* wrote this to compare arrays of hex values */
int cmphex(unsigned char *first, unsigned char *second){
        int i;
        for(i = 0; i < 16; i++){
                if((int)first[i] != (int)second[i]){
                        return -1;
                }
        }
        return 0;
}

