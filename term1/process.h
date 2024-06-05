#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0


#define NEW 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define TERMINATED 4

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int priority;
    int cpu_burst;
    int io_burst;
    int arrival_time;
    int start_time;
    int end_time;
    int remaining_time;
    int io_interrupt_time;
    int status;
    // evaluation
    int waiting_time;
    int turnaround_time;
    int in_ready_queue;
} Process;

typedef struct {
    Process process_arr[MAX_PROCESSES];
    int count;
} Queue;

int create_process(Queue* q, int num);
void initQueue(Queue* q);
void enqueue(Queue* q, Process p);
Process dequeue(Queue* q);
int isEmptyQueue(Queue *q);

#endif