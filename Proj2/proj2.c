/******************************************
*Project 2
* 
*Name: Vignesh Suresh
*
*Event-Driven Simulation and Bash Script
*******************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//Information
double totalWaitTime = 0;
double maxWaitTime = 0;
double avgWaitTime = 0;
int totalServed = 0;
int maxLength = 0;

//Stores Data
typedef int	Data;

//Boolean
typedef enum {
	false,
	true
} boolean;

//Queue node
typedef struct Node {
	struct Node *next;
	int d;
} Node;

//Queue
typedef struct Queue {
	int total;
	Node *front;
	Node *rear;
} Queue;

//Update the waiting time
void updateWaitTime(double time){
	totalWaitTime += time;
	if (time > maxWaitTime)
		maxWaitTime = time;
}

//Update the line length
void updateline(int length) {
	avgWaitTime = (avgWaitTime + length) / 2.0;
	if (length > maxLength)
		maxLength = length;
}

//Update total served
void updateServed(int number){
	totalServed += number;
}

//Create the queue
void createQueue(Queue *q) {
	q->total = 0;
	q->front = NULL;
	q->rear = NULL;
}

//Checks if queue is empty
boolean empty(const Queue *q) {
	return (boolean) (q->total == 0);
}

//Checks if queue is full
boolean full(const Queue *q) {
	return (boolean) (q->total == 1000);
}

//Pushs node onto stack
void push(Data d, Queue *q){
	Node *p;

	//Allocates memory for node
	p = malloc(sizeof(Node));
	p->d = d;
	p->next = NULL;

	//Pushs onto queue
	if (!empty(q)) {
		q->rear->next = p;
		q->rear = p;
	} else
		q->front = q->rear = p;

	//Increments total nodes in queue
	q->total++;
}

//Pops a node off the stack
Data pop(Queue *q){
	Data d;
	Node *p;

	//Retrieves the node
	d = q->front->d;
	p = q->front;
	q->front = q->front->next;
	q->total--;

	//Frees node memory
	free(p);

	return d;
}

//Random function
double expdist(double mean) {
	double r = rand();
	return -mean * log(r/RAND_MAX);
}

//Loads data from data file
void loadData(int Data[]) {
	int i;
	int lines;
	int max = 0;
	int arriving = 0;

	//Opens file
	FILE *read;
	read = fopen("data.dat", "r");

	//Checks if file exists
	if(read == NULL) {
		printf("File could not be opened\n");
		exit(1);
	}

	//Loads data from file
	while(fscanf(read, "%d %d", &arriving, &lines) != EOF){
		for (i = max; i < max + lines; i++)
			Data[i] = arriving;
		max += lines;
	} 
}

//Prints information
void printData(){
	printf("Total Served : %d\n", totalServed);
	printf("Wait Time Average : %f\n", (totalWaitTime / totalServed));
	printf("Wait Time Max : %f\n", maxWaitTime);
	printf("Line Length Average : %f\n", avgWaitTime);
	printf("Line Length Max : %d\n", maxLength);
}

//Simulates the program
void simulation (int numOfTellers){
	double tellers[numOfTellers];
	int Data[100];
	int time;
	int i;

	//Creates queue
	Queue line;
	createQueue(&line);

	//Loads data from file
	loadData(Data);
	
	//Runs the number of hours in work day (8 hours)
	for (time = 1; time < 480; time++){
		int randTime = Data[rand() % 100];
		updateServed(randTime);

		//Pushes customers onto line
		for (randTime = randTime; randTime > 0; randTime--){
			push(time, &line);
		}

		//Simulates customers being served
		for (i = 0; i < numOfTellers; i++){
			if(!empty(&line) && (tellers[i] < 0 || (tellers[i] -=1.0) < 0)){ 
				int arrival = pop(&line);
				updateWaitTime(((double)(time-arrival))-tellers[i]);
				tellers[i] += expdist(2);
			}
		}
		//Update the length of line
		updateline(line.total);
	}
	//Print final data
	printData();
}

//Starts program
int main () {
	int tellers;

	//Allows the random function to be random
	srand(time(NULL));

	//Runs simulation for 4-7 tellers
	for (tellers = 4; tellers < 8; tellers++){
		printf("\nNumber of Tellers : %d\n", tellers);
		simulation(tellers);
	}

	return 0;                       
}
