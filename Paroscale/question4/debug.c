// To fix the bug, we need to add a check to ensure that head is not NULL before accessing its data. In the fixed code, we willl add a check in the remove_data function to verify if head is NULL before accessing its data. If head is NULL, we can handle the case by returning an error value or implementing appropriate error handling logic. In this example, we return -1 as an error value, which can be checked in the consumer function to print an error message.
// Additionally, we will add a call to free(temp) to release the memory occupied by the removed node.
// With these changes, the code should now work correctly without the segmentation fault when trying to remove data from an empty buffer.Here's the fixed code:

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 10

typedef struct node {
int data;
struct node *next;
} Node;

Node *head = NULL, *tail = NULL;
int count = 0;
sem_t full, empty;
pthread_mutex_t lock;

void add_data(int data) {
Node *new_node = (Node*)malloc(sizeof(Node));
new_node->data = data;
new_node->next = NULL;

if (tail == NULL) {
head = tail = new_node;
} else {
tail->next = new_node;
tail = new_node;
}
count++;
}

int remove_data() {
if (head == NULL) {
return -1;
}


Node *temp = head;
int data = temp->data;
head = head->next;
free(temp);
count--;
return data;
}

void *producer(void *arg) {
int i, data;
for (i = 0; i < 100; i++) {
data = rand() % 100;
sem_wait(&empty);
pthread_mutex_lock(&lock);
add_data(data);
printf("Produced: %d\n", data);
pthread_mutex_unlock(&lock);
sem_post(&full);
}
}

void *consumer(void *arg) {
int i, data;
for (i = 0; i < 100; i++) {
sem_wait(&full);
pthread_mutex_lock(&lock);
data = remove_data();
if (data != -1) {
printf("Consumed: %d\n", data);
} else {
printf("Error: Tried to consume from an empty buffer\n");
}
pthread_mutex_unlock(&lock);
sem_post(&empty);
}
}

int main() {
pthread_t producer_thread, consumer_thread;
sem_init(&full, 0, 0);
sem_init(&empty, 0, BUFFER_SIZE);
pthread_mutex_init(&lock, NULL);
pthread_create(&producer_thread, NULL, producer, NULL);
pthread_create(&consumer_thread, NULL, consumer, NULL);
pthread_join(producer_thread, NULL);
pthread_join(consumer_thread, NULL);
return 0;
}
