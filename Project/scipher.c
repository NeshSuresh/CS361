/*************************************************
*	Project 1 CIS 361
*
*	Sean Crowley
*
*	This program encrypts and decrypts ceasar
*	  ciphers without a key.
*************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char encrypt(char ch, int k);
void decrypt(int key, char fname[], char oname[]);
int findKey(float given[], float found[]);
void readFreq(float given[], char fname[]);
void calcFreq(float found[], char fname[]);
char rotate(char ch, int num);

int main(int argc, char* argv[])
{  
	int choice, key;
	char ch;
	FILE *fin, *fout;
	float given[26] = {0};
	float found[26] = {0};

	//Print parameter data
	if (argc != 5)
	{
		printf ("Parameters: cipher option, key, input file, output file\n");
		printf ("Cipher option 1 : encryption\tCipher option 2 : decryption\n");

		exit(1);
	}
	
	//Converts choice and key to ints
	choice = atoi(argv[1]);
	key = atoi(argv[2]);

	//Decryption
	if (choice == 2)
	{
		//Load letter frequencies
		readFreq(given, "LetFreq.txt");

		//Calculate encrypted file frequencies
		calcFreq(found, argv[3]);

		//Calculate the key
		int key = findKey(given, found);

		//Print key
		printf("Found Key : %i\n\n", key);

		//Decrypt the message
		decrypt(key, argv[3], argv[4]);	
	}
	//Encryption
	else
	{
		//Input Output files
		fin = fopen(argv[3], "r");
		fout = fopen(argv[4], "w");

		//Checks if files exist
		if (fin ==  NULL || fout == NULL) 
		{
			printf("File could not be opened\n");
			exit(1);
		}

		//Prints encrypted text to file
		while ( fscanf(fin, "%c", &ch) != EOF )
		{
			fprintf(fout, "%c", encrypt(ch, key));
		}

		//Closes files
		fclose(fin);
		fclose(fout);
	}

	return 0;
}

//Encrypts a message
char encrypt(char ch, int k)
{
	if (k < 0)
		k = k + 26;

	if (isupper(ch))
		return (ch - 'A' + k) % 26 + 'A';
	
	if (islower(ch))
		return (ch - 'a' + k) % 26 + 'a';
	
	return ch;
}

// Load array given with the letter frequencies for English from file LetFreq.txt
void readFreq(float *given, char fname[]){
    char * line = NULL;
    size_t len = 26;
	int i = 0;

	FILE * freqFile = fopen(fname, "r");

	//Loads frequcencies into an array
    while (getline(&line, &len, freqFile) != -1) {
		strtok (line, " ");
		given[i] = atof(line+2);
		i++;
    }

    fclose(freqFile);
}

// Read the encoded text from an input file and accumulate the letter frequency                  
// data for the encoded text. Store the frequency data in array found.
void calcFreq(float found[], char fname[]){
	int chars = 0;
    int c;
	int i;

    FILE *file = fopen(fname, "r");

	//Calculates the number of times a character appears in a file
    while ((c = fgetc(file)) != EOF)
    {
		c = tolower((char) c) - 97;

		if(c >= 0 && c <=25){
        	found[c] = found[c] + 1;
			chars++;
		}
    }

	//Divides frequencies by total number of characters
	for(i=0;i<26;i++){
		found[i] = found[i] / chars;
	}

	fclose(file);
}

// Rotate the character in parameter ch down the alphabet for the number of   
// positions as given in parameter num and return the resulting character.
char rotate(char ch, int num){
	int i;

	ch = tolower(ch);

	//Rotates the array
	for(i=0; i<num; i++){
		ch++;
		if(ch > 122){
			ch -= 26;
		}
	}

	return ch;
}

// Compare the data in array found with the frequency data in array given, looking
// for a key that will give you the best match. To do this, try each of the 26 rotations,
// and remember which gives the smallest difference between the frequencies you
// observed and the frequencies given. Return the key.
int findKey(float given[], float found[]){
	int key = 0;
	float greatest = 0;
	int sum[26] = {0};
	int i;
	int j;

	//Compares the frequencies and adds to sum
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			int giv = (int) trunc(1000000. * given[j]);
			int fou = (int) trunc(1000000. * found[rotate((char) j + 97, i) -97]);

			sum[i] += (giv - fou) * (giv -fou);
		}
	}

	//Finds the greatest sum of frequencies
	for(i=0;i<26;i++){
		if(sum[i] > greatest){
			greatest = sum[i];
			key = i;
		}
	}

	return key;
}

// Decrypt the encoded text in the input file using the key and display the decoded text
void decrypt(int key, char fname[], char oname[]){
    int c;

	FILE *fout = fopen(oname, "w");
    FILE *fin = fopen(fname, "r");

	//Prints the decrypted message to terminal and file
    while ((c = fgetc(fin)) != EOF)
    {
		c = tolower(c);
		if(c >= 97 && c <= 122){
			putchar(rotate(c, 26 - key));
			fprintf(fout, "%c", rotate(c, 26 - key));
		}else{
			putchar(c);
			fprintf(fout, "%c", c);
		}
    }
	printf("\n\n");

	fclose(fout);
	fclose(fin);
}

