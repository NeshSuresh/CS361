#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "cipher.h"

FILE *fin;
FILE *fout;

int main(int argc, char* argv[])
{
	float given[26];
	float found[26];
	float f;
	int i, count;
	float tmp;
	int key;

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	if (fin ==  NULL) {
		printf("Please enter file to decrypt and optionally an output file\n");
		printf("if no output file is given, decryption will print to stdout.\n");
		exit(1);
	}

	readFreq(given);
	calcFreq(found);
	key = findKey(given, found);

	rewind(fin);

	decrypt(key);

	return 0;
}

void decrypt(int key, char fname[]) {
	char ch;
	if (fout == NULL) {
		while (fscanf(fin,"%c", &ch) != EOF) {
			if (isalpha(ch)){
				if (islower(ch))
					printf("%c",((ch-'a'+key)%26)+'a');
				else
					printf("%c",((ch-'A'+key)%26)+'A');
			}
			else
				printf("%c",ch);
		}
	}

	else {
		while (fscanf(fin,"%c", &ch) != EOF) {
			if (isalpha(ch)){
				if (islower(ch))
					fprintf(fout,"%c",((ch-'a'+key)%26)+'a');
				else
					fprintf(fout,"%c",((ch-'A'+key)%26)+'A');
			}
			else
				fprintf(fout,"%c",ch);
		}
	}

}

int findKey(float given[], float found[]) {
	float sum = 0;
	float min = 0;
	int key,k,i;
	float tmp;
	for (i=0;i<26;i++) {
		tmp = given[i]-found[i];
		min += (tmp<0)? -tmp:tmp;
	}

	k = 1;
	for (k=1;k<26;k++) {
		sum = 0;
		for (i=0;i<26;i++) {
			tmp = given[(i+k)%26]-found[i];
			sum += (tmp<0)? -tmp:tmp;
		}

		if (sum < min) {
			min = sum;
			key = k;
		}
		sum = 0;
	}
	return key;
}

void readFreq(float given[], char fname[]) {
	float f;
	char ch;
	FILE* letFreq = fopen("LetFreq.txt", "r");
	int i = 0;
	while (fscanf(letFreq, "%c %f\n", &ch, &f) != EOF) {
		given[i] = f;
		i++;
	}
}

void calcFreq(float found[], char fname[]){
	char ch;
	int count = 0;
	int i;
	while (fscanf(fin,"%c", &ch) != EOF) {
		if (isalpha(ch)) {
			count++;
			if (islower(ch)) {
				found[ch-'a']++;
			}
			else {
				found[ch-'A']++;
			}
		}
	}
	for (i=0;i<26;i++){
		found[i] /= count;
	}
}
