#include "process.h"

/* 
    Process 
*/
void create_process(Queue* q, int num) {
    // random seed
    srand(time(NULL));
    q->count = num;
    // initialization
    for (int i = 0; i < num; i++) {
        // given values
        q->process_arr[i].pid = i + 1; // pid starts from 1
        q->process_arr[i].priority = rand() % num + 1; // 1 to num
        q->process_arr[i].cpu_burst = rand() % 10 + 1; // 1 to 10
        if (rand() % 2 == 1) {
			q->process_arr[i].io_burst = rand() % 5 + 1; // 1 to 5
		}
		else {
			q->process_arr[i].io_burst = 0;
		}
        q->process_arr[i].arrival_time = rand() % 10 + 1; // 1 to 10
        q->process_arr[i].status = NEW;
        // depending values
        q->process_arr[i].start_time = -1;
        q->process_arr[i].end_time = 0;
        q->process_arr[i].remaining_time = q->process_arr[i].cpu_burst;
        q->process_arr[i].io_interrupt_time = q->process_arr[i].io_burst > 0 ? rand() % 10 + 1 : 0;
        q->process_arr[i].waiting_time = 0;
        q->process_arr[i].turnaround_time = 0;
    }
}

/* 
    Queue
*/

void initQueue(Queue* q) {
    q->count = 0;
}

// enqueue
void enqueue(Queue* q, Process p) {
    q->process_arr[q->count++] = p;
}

// dequeue
Process dequeue(Queue* q) {
    Process p = q->process_arr[0];
    for (int i = 0; i < q->count - 1; i++) {
        q->process_arr[i] = q->process_arr[i + 1];
    }
    q->count--;
    return p;
}



int isEmptyQueue(Queue *q) {
    return q->count == 0;
}