
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALPH 26

char encrypt (char ch, int k);
void readFreq (float given[], char fname[]);
void calcFreq (float found[], char fname[]);
void rotate (float given[]);
int findKey (float given[], float found[]);
void decrypt (int key, char fname[], char out[]);

int
main (int argc, char *argv[])
{
  int choice, key;
  char ch;
  FILE *fin, *fout;
  float given[26] = { 0 };
  float found[26] = { 0 };

  //Print parameter data
  if (argc != 5)
    {
      printf ("Parameters: cipher option, key, input file, output file\n");
      printf ("Cipher option 1 : encryption\tCipher option 2 : decryption\n");

      exit (1);
    }

  //Converts choice and key to ints
  choice = atoi (argv[1]);
  key = atoi (argv[2]);

  //Decryption
  if (choice == 2)
    {
      //Load letter frequencies
      readFreq (given, "LetFreq.txt");

      //Calculate encrypted file frequencies
      calcFreq (found, argv[3]);

      //find Key 
      int key = findKey (given, found);

      //Print key
      printf ("Found Key : %i\n\n", key);

      //Decrypt the message
      decrypt (key, argv[3], argv[4]);
    }
  //Encryption
  else
    {
      //Input Output files
      fin = fopen (argv[3], "r");
      fout = fopen (argv[4], "w");

      //Checks if files exist
      if (fin == NULL || fout == NULL)
	{
	  printf ("File could not be opened\n");
	  exit (1);
	}

      //Prints encrypted text to file
      while (fscanf (fin, "%c", &ch) != EOF)
	{
	  fprintf (fout, "%c", encrypt (ch, key));
	}

      //Closes files
      fclose (fin);
      fclose (fout);
    }

  return 0;
}

char
encrypt (char ch, int k)
{
  if (k < 0)
    k = k + 26;

  if (isupper (ch))
    return (ch - 'A' + k) % 26 + 'A';

  if (islower (ch))
    return (ch - 'a' + k) % 26 + 'a';

  return ch;
}

// Load array given with the letter frequencies for English from file LetFreq.txt
void
readFreq (float *given, char fname[])
{
  FILE *freq;
  freq = fopen (fname, "r");


  if (freq == NULL)
    {
      printf ("Error:  Could not open LetFreq.txt!!\n");
      exit (0);
    }

  //breaks each freq line into tokens, only grabs the numbers.
  char buf[1024];
  int cnt = 1, i = 0;
  while ((fgets (buf, 1024, freq)) != NULL)
    {
      char *tok;
      tok = strtok (buf, "\n ");
      tok = strtok (NULL, "\n ");
      given[i] = atof (tok);
      i++;
    }
}

// Read the encoded text from an input file and accumulate the letter frequency
// data for the encoded text. Store the frequency data in array found.
void
calcFreq (float found[], char fname[])
{
  FILE *freq;
  freq = fopen (fname, "r");
  if (freq == NULL)
    {
      printf ("Error: Could not open encrypted file data.out!");
      exit (0);
    }

  int i, charCount = 0;

  //initializes found to 0's
  for (i = 0; i < ALPH; i++)
    found[i] = 0;

  //counts the number of times each letter appears in the ciphered text
  char tmp = fgetc (freq);
  while (tmp != EOF)
    {
      if (isalpha (tmp))
	{
	  charCount++;
	  tmp = toupper (tmp);
	  found[tmp - 'A']++;
	}
      tmp = fgetc (freq);
    }

  //updates the found array with the letter frequencies
  for (i = 0; i < ALPH; i++)
    {
      found[i] /= charCount;
    }
}

// Rotate the character in parameter ch down the alphabet for the number of
// positions as given in parameter num and return the resulting character.
void
rotate (float given[])
{

  int i;
  float tmp, letterZ = given[ALPH - 1];

  //moves each element 1 spot down the array
  for (i = ALPH - 1; i >= 0; i--)
    {
      tmp = given[i - 1];
      given[i] = tmp;
    }
  given[0] = letterZ;
}

float
sqr (float s)
{
  return s * s;
}

// Compare the data in array found with the frequency data in array given, looking
// for a key that will give you the best match. To do this, try each of the 26 rotations,
// and remember which gives the smallest difference between the frequencies you
// observed and the frequencies given. Return the key.
int
findKey (float given[], float found[])
{

  int i, j, key = 0, firstTime = 1;
  float sum = 0, lowSum;

  //tries all 26 rotations
  for (j = 0; j < ALPH; j++)
    {
      sum = 0;

      //sums the differences between the arrays
      for (i = 0; i < ALPH; i++)
	sum += sqr (found[i] - given[i]);

      if (firstTime)
	{
	  lowSum = sum;
	  firstTime = 0;
	}

      if (sum < lowSum)
	{
	  lowSum = sum;
	  key = j;
	}

      rotate (given);
    }

  return key;
}

// Decrypt the encoded text in the input file using the key and display the decoded text
void
decrypt (int key, char fname[], char out[])
{
  //Read file and use rotate(key) on each char to get the message



  FILE *ifp, *ofp;
  ifp = fopen (fname, "r");
  ofp = fopen (out, "w");

  if (key < 0)
    key = key + 26;

  char tmp = fgetc (ifp);
  while (tmp != EOF)
    {

      if (isupper (tmp))
	fprintf (ofp, "%c", (tmp - key + ALPH - 'A') % ALPH + 'A');
      else if (islower (tmp))
	fprintf (ofp, "%c", (tmp - key + ALPH - 'a') % ALPH + 'a');
      else
	fprintf (ofp, "%c", tmp);

      tmp = fgetc (ifp);
    }
}
