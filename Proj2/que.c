nclude "queue.h"

void initialize(Queue *q) {
	q->cnt = 0;
	q->front = NULL;
	q->rear = NULL;
}

void push(data d, Queue *q){
	elem *p;

	p = malloc(sizeof(elem));
	p->d = d;
	p->next = NULL;
	if (!empty(q)) {
		q->rear->next = p;
		q->rear = p;
	} else
		q->front = q->rear = p;
	q->cnt++;
}

data pop(Queue *q){
	data d;
	elem *p;
	
	d = q->front->d;
	p = q->front;
	q->front = q->front->next;
	q->cnt--;
	free(p);
	return d;
}

boolean empty(const Queue *q) {
	return (boolean) (q->cnt == EMPTY);
}

boolean full(const Queue *q) {
	return (boolean) (q->cnt == FULL);
}
