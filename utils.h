#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include "process.h"


#define MAX_PROCESSES 100

int io_times[2];

typedef struct {
    int gantt[1000];
    int gantt_idx;
} GanttChart;

void print_gantt_chart(int gantt[], int n);
// void _IO_events(int total_time, int *io_times, int *io_count);

#endif