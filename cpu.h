#ifndef __CPU_H__
#define __CPU_H__


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

#define FCFS 1
#define SJF 2
#define PRI 3
#define RR 4
#define PRE_SJF 5
#define PRE_PRI 6

// ready queue and waiting queue
Queue ready_queue;
Queue waiting_queue;

// Gantt Chart
GanttChart gantt_chart;

// Executed process saved array
Process process_arr[MAX_PROCESSES];

void config(Queue* ready_queue, Queue* waiting_queue, GanttChart* gantt_chart);
void select(int sel);
void evaluate(Process process_arr[], int num);
void IO_events(Queue* q, int *io_times);
int _IO_operation(Queue* ready_queue, Queue* waiting_queue, int current_time);

void _FCFS(Queue* ready_queue, GanttChart* gantt_chart);
void _SJF(Queue *ready_queue, int preemptive, GanttChart* gantt_chart);
void _PRI(Queue *ready_queue, int preemptive, GanttChart* gantt_chart);
void _RR(Queue *ready_queue, int time_quantum, GanttChart* gantt_chart);

#endif