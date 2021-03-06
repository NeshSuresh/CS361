#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ALPH 26

FILE *fin;
FILE *fout;
int findKey(float given[], float found[]);
void readFreq(float given[]);
void calcFreq(float found[]);
void rotate(float given[]);

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
	printf ("%i\n", key);
    rewind(fin);

    decrypt(key);

	return 0;
}

void decrypt(int key) {
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

void rotate(float given[]){

        int i;
        float tmp, letterZ = given[ALPH-1];

	//moves each element 1 spot down the array
        for(i = ALPH-1; i >= 0; i--){
                tmp = given[i-1];
                given[i] = tmp;
        }
        given[0] = letterZ;
}
float sqr(float s){
	return s*s;
}

int findKey(float given[], float found[]){

	int i, j, key = 0, firstTime = 1;
	float sum = 0, lowSum;

	//tries all 26 rotations
	for(j = 0; j < ALPH; j++){
		sum = 0;

		//sums the differences between the arrays
		for(i = 0; i < ALPH; i++)
			sum += sqr(found[i] - given[i]);

		if(firstTime){
			lowSum = sum;
			firstTime = 0;
		}

		if(sum < lowSum){
			lowSum = sum;
			key = j;
		}

		rotate(given);
	}

	return key;
}


void readFreq(float *given) {
    float f;
    char ch;
    FILE* letFreq = fopen("LetFreq.txt", "r");
    int i = 0;
    while (fscanf(letFreq, "%c %f\n", &ch, &f) != EOF) {
        given[i] = f;
        i++;
    }
}

void calcFreq(float *found){
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
