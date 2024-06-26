#include "cpu.h"

// 환경 설정
void config(Queue* ready_queue, Queue* waiting_queue, GanttChart* gantt_chart) {
    // reset ready queue/waiting queue
    initQueue(ready_queue);
    initQueue(waiting_queue);

    // reset gantt chart
    for (int i = 0; i < 1000; i++) {
        gantt_chart->gantt[i] = -1;
    }
    gantt_chart->gantt_idx = 0;

    // process_arr
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_arr[i].pid = -1;  // 프로세스 ID 초기화
        process_arr[i].priority = 0;
        process_arr[i].cpu_burst = 0;
        process_arr[i].io_burst = 0;
        process_arr[i].arrival_time = 0;
        process_arr[i].start_time = 0;
        process_arr[i].end_time = 0;
        process_arr[i].remaining_time = 0;
        process_arr[i].io_interrupt_time = 0;
        process_arr[i].status = 0;  // 프로세스 상태 초기화
        process_arr[i].waiting_time = 0;
        process_arr[i].turnaround_time = 0;
        process_arr[i].in_ready_queue = 0;
    }
}


// 알고리즘 선택
void select_alg(int sel) {
    int time_quantum;
    int preemptive;

    switch(sel) {
        case FCFS:
            _FCFS(&ready_queue, &gantt_chart);
            break;
        case SJF:
            _SJF(&ready_queue, &gantt_chart);
            break;
        case PRI:
            _PRI(&ready_queue, &gantt_chart);
            break;
        case RR:
            printf("Enter time quantum for Round Robin: ");
            scanf("%d", &time_quantum);
            _RR(&ready_queue, &gantt_chart, time_quantum);
            break;
        case PRE_SJF:
            _P_SJF(&ready_queue, &gantt_chart);
            break;
        case PRE_PRI:
            preemptive = 1;
            _P_PRI(&ready_queue, &gantt_chart);
            break;
        default:
            printf("Invalid selection. Please restart the program.\n");
    }
}

// IO operation 배열 만들기
void IO_events(Queue* q, int* io_time) {
    for (int j = 0; j < q->count; j++) {
        if (q->process_arr[j].io_interrupt_time != 0 && q->process_arr[j].io_burst != 0) {
            *io_time = q->process_arr[j].io_interrupt_time;
            break;
        }
    }
    printf("\nI/O Event: %d\n", *io_time);
}


// waiting queue에서 IO operation 찾기
int _IO_operation(Queue* ready_queue, Queue* waiting_queue, int current_time) {
    int io_event_occurred = 0;
    for (int i = 0; i < waiting_queue->count; i++) {
        Process *p = &waiting_queue->process_arr[i];
        // WAITING -> READY
        if (p->status == WAITING && current_time - p->io_interrupt_time >= p->io_burst) {
            p->status = READY;
            dequeue(waiting_queue);
            enqueue(ready_queue, *p);
            io_event_occurred = 1;
        }
    }
    return io_event_occurred;
}

// // FCFS (First Come First Served)
// void _FCFS_v1(Queue* ready_queue, GanttChart* gantt_chart) {
//     int current_time = 0;
//     int completed = 0;
//     int total = ready_queue->count;

//     while (completed < total) {
//         if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
//             continue;
//         }

//         int found_process = 0;

//         for (int i = 0; i < total; i++) {
//             Process *p = &ready_queue->process_arr[i];
//             if (p->arrival_time <= current_time && p->remaining_time > 0) {   
//                 found_process = 1;
                
//                 if (p->start_time == -1) {
//                     p->start_time = current_time;
//                 }

//                 for (int j = 0; j < p->remaining_time; j++) {
//                     gantt_chart->gantt[gantt_chart->gantt_idx++] = p->pid;
//                     current_time++;

//                     // I/O interrupt 발생 시  
//                     if(current_time == p->io_interrupt_time && p->io_burst > 0) {
//                         enqueue(&waiting_queue, *p);
//                         p->status = WAITING;
//                         break;
//                     }

//                     // I/O interrupt 탐색
//                     if (_IO_operation(ready_queue, &waiting_queue, current_time)){
//                         break;
//                     }
//                 }
//                 p->remaining_time = 0;
//                 p->end_time = current_time;
//                 p->turnaround_time = p->end_time - p->arrival_time;
//                 p->waiting_time = p->turnaround_time - p->cpu_burst;
//                 p->status = TERMINATED;
//                 process_arr[completed++] = *p;
//                 break;
//             }
//         }
//         if (!found_process) {
//             gantt_chart->gantt[gantt_chart->gantt_idx++] = -1;
//             current_time++;
//         }
//     }
// }

// FCFS (First Come First Served)
void _FCFS(Queue* ready_queue, GanttChart* gantt_chart) {
    int current_time = 0;
    int completed = 0;
    int total = ready_queue->count;

    while (completed < ready_queue->count) {

        int found_process = 0;

        int m_arrival_time = 10000;
        int arrival_index = -1;

        // 가장 빠른 arrival time 찾기
        for (int i = 0; i < total; i++) {
            Process *p = &ready_queue->process_arr[i];
            if (p->arrival_time <= current_time && p->remaining_time > 0 && p->arrival_time < m_arrival_time) {
                m_arrival_time = p->arrival_time;
                arrival_index = i;
            }
        }

        // 아직 도착한 프로세스 없음
        if (arrival_index == -1) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = -1;
            current_time++;
            continue;
        }

        Process *p = &ready_queue->process_arr[arrival_index];

        // 처음 도착한 프로세스일 시
        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        // 하나의 프로세스 진행
        for (int j = 0; j < p->remaining_time; j++) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = p->pid;
            current_time++;
        }
        
        p->remaining_time = 0;
        p->end_time = current_time;
        p->turnaround_time = p->end_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->cpu_burst;
        p->status = TERMINATED;
        process_arr[completed++] = *p;
    }
}

// SJF (Shortest Job First)
void _SJF(Queue *ready_queue, GanttChart* gantt_chart) {
    int current_time = 0;
    int completed = 0;

    while (completed < ready_queue->count) {
        if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
            continue;
        }

        int shortest_time = 10000;
        int shortest_index = -1;

        // shortest job을 찾기 (remaining job에 따라)
        for (int i = 0; i < ready_queue->count; i++) {
            Process *p = &ready_queue->process_arr[i];
            if (p->arrival_time <= current_time && p->remaining_time > 0 && p->remaining_time < shortest_time) {
                shortest_time = p->remaining_time;
                shortest_index = i;
            }
        }

        // 아직 도착한 프로세스 없음
        if (shortest_index == -1) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = -1;
            current_time++;
            continue;
        }

        Process *p = &ready_queue->process_arr[shortest_index];

        // 처음 도착한 프로세스일 시
        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        // 하나의 프로세스 진행
        for (int j = 0; j < p->remaining_time; j++) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = p->pid;
            current_time++;
            // I/O interrupt 발생 시  
            if(current_time == p->io_interrupt_time && p->io_burst > 0) {
                enqueue(&waiting_queue, *p);
                p->status = WAITING;
            }
            if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
                break; 
            }
        }
        p->remaining_time = 0;
        p->end_time = current_time;
        p->turnaround_time = p->end_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->cpu_burst;
        p->status = TERMINATED;
        process_arr[completed++] = *p;
        
    }
}

// Preemptive SJF (Shortest Job First)
void _P_SJF(Queue *ready_queue, GanttChart* gantt_chart) {
    int current_time = 0;
    int completed = 0;

    while (completed < ready_queue->count) {
        if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
            continue;
        }

        int shortest_time = 10000;
        int shortest_index = -1;

        // shortest job을 찾기 (remaining job에 따라)
        for (int i = 0; i < ready_queue->count; i++) {
            Process *p = &ready_queue->process_arr[i];
            if (p->arrival_time <= current_time && p->remaining_time > 0 && p->remaining_time < shortest_time) {
                shortest_time = p->remaining_time;
                shortest_index = i;
            }
        }

        // 아직 도착한 프로세스 없음
        if (shortest_index == -1) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = -1;
            current_time++;
            continue;
        }

        Process *p = &ready_queue->process_arr[shortest_index];

        // 처음 도착한 프로세스일 시
        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        // preemptive
        gantt_chart->gantt[gantt_chart->gantt_idx++] = p->pid;
        current_time++;
        p->remaining_time--;
        if (p->remaining_time == 0) {
            p->end_time = current_time;
            p->turnaround_time = p->end_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->cpu_burst;
            p->status = TERMINATED;
            process_arr[completed++] = *p;
        }
        
    }
}

// PRIORITY
void _PRI(Queue *ready_queue, GanttChart* gantt_chart) {
    int current_time = 0;
    int completed = 0;

    while (completed < ready_queue->count) {
        if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
            continue; 
        }

        int highest_priority = 10000;
        int highest_index = -1;

        for (int i = 0; i < ready_queue->count; i++) {
            Process *p = &ready_queue->process_arr[i];
            if (p->arrival_time <= current_time && p->remaining_time > 0 && p->priority < highest_priority) {
                highest_priority = p->priority;
                highest_index = i;
            }
        }

        if (highest_index == -1) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = -1; // IDLE
            current_time++;
            continue;
        }

        Process *p = &ready_queue->process_arr[highest_index];

        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        
        for (int j = 0; j < p->remaining_time; j++) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = p->pid;
            current_time++;
            // I/O interrupt 발생 시  
            if(current_time == p->io_interrupt_time && p->io_burst > 0) {
                enqueue(&waiting_queue, *p);
                p->status = WAITING;
            }
            if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
                break; 
            }
        }
        p->remaining_time = 0;
        p->end_time = current_time;
        p->turnaround_time = p->end_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->cpu_burst;
        p->status = TERMINATED;
        process_arr[completed++] = *p;
    }

}

// Preemptive PRIORITY
void _P_PRI(Queue *ready_queue, GanttChart* gantt_chart) {
    int current_time = 0;
    int completed = 0;

    while (completed < ready_queue->count) {
        if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
            continue; 
        }

        int highest_priority = 10000;
        int highest_index = -1;

        for (int i = 0; i < ready_queue->count; i++) {
            Process *p = &ready_queue->process_arr[i];
            if (p->arrival_time <= current_time && p->remaining_time > 0 && p->priority < highest_priority) {
                highest_priority = p->priority;
                highest_index = i;
            }
        }

        if (highest_index == -1) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = -1; // IDLE
            current_time++;
            continue;
        }

        Process *p = &ready_queue->process_arr[highest_index];

        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        // preemptive
        gantt_chart->gantt[gantt_chart->gantt_idx++] = p->pid;
        current_time++;
        p->remaining_time--;
        if (p->remaining_time == 0) {
            p->end_time = current_time;
            p->turnaround_time = p->end_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->cpu_burst;
            p->status = TERMINATED;
            process_arr[completed++] = *p;
        }
    }

}

// Round Robin
void _RR(Queue *ready_queue, GanttChart* gantt_chart, int time_quantum) {
    int current_time = 0;
    int completed = 0;
    int total = ready_queue->count;

    while (completed < total) {
        if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
            continue; 
        }

        int idle = 1;

        for (int i = 0; i < ready_queue->count; i++) {
            Process p = dequeue(ready_queue);
            if (p.arrival_time <= current_time && p.remaining_time > 0) {   
                // 도착한 프로세스 있음
                idle = 0; 
                if (p.start_time == -1) {
                    p.start_time = current_time;
                }

                // time quantum에 따른 time slice
                int time_slice = (p.remaining_time > time_quantum) ? time_quantum : p.remaining_time;

                // time slice만큼 프로세스 진행
                for (int j = 0; j < time_slice; j++) {
                    gantt_chart->gantt[gantt_chart->gantt_idx++] = p.pid;
                    current_time++;
                    if (_IO_operation(ready_queue, &waiting_queue, current_time)) {
                        break; 
                    }
                }
                p.remaining_time -= time_slice;

                // 프로세스가 끝나면
                if (p.remaining_time == 0) {
                    p.end_time = current_time;
                    p.turnaround_time = p.end_time - p.arrival_time;
                    p.waiting_time = p.turnaround_time - p.cpu_burst;
                    p.status = TERMINATED;
                    process_arr[completed++] = p;
                }
                else {
                    enqueue(ready_queue, p);
                }
            }
            // 도착한 프로세스가 없으면
            else {
                // 다시 넣기
                enqueue(ready_queue, p);
            }
        }
        // 실행 가능한 프로세스 없음
        if (idle) {
            gantt_chart->gantt[gantt_chart->gantt_idx++] = -1;
            current_time++;
        }
    }
}

void evaluate(Process process_arr[], int num) {
    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    double avg_turnaround_time, avg_waiting_time;

    for (int i = 0; i < num; i++) {
        total_turnaround_time += process_arr[i].turnaround_time;
        total_waiting_time += process_arr[i].waiting_time;
    }

    avg_turnaround_time = (double)total_turnaround_time / num;
    avg_waiting_time = (double)total_waiting_time / num;

    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
}

int main() {
    int sel;
    int num;

    printf("Welcome to the CPU Scheduler Simulation!\n");
    printf("1 - FCFS (First-Come, First-Served)\n");
    printf("2 - SJF (Shortest Job First)\n");
    printf("3 - PRI (Priority)\n");
    printf("4 - RR (Round Robin)\n");
    printf("5 - PRE_SJF (Preemptive SJF)\n");
    printf("6 - PRE_PRI (Preemptive Priority)\n");
    printf("Enter your choice of scheduling algorithm (1-6): ");
    scanf("%d", &sel);
    printf("Enter the number of processes (MAX: 100): ");
    scanf("%d", &num);

    // Queue NewQueue;
    
    config(&ready_queue, &waiting_queue, &gantt_chart);
    
    create_process(&ready_queue, num);  // Create array of process pointers
    // create_process(&NewQueue, num);  // Create array of process pointers
    // ready_queue = NewQueue;
    
    IO_events(&ready_queue, &io_time);

    
    printf("\n========================= PROCESS INFO =========================\n");
    printf("%-10s%-10s%-10s%-10s%-10s%-10s\n", "PID", "Arrival", "CPU", "I/O", "Priority", "Interrupt");
    for (int i = 0; i < num; i++) {
        Process p = ready_queue.process_arr[i];
        printf("%-10d%-10d%-10d%-10d%-10d%-10d\n", 
        p.pid, p.arrival_time, p.cpu_burst, p.io_burst, p.priority, p.io_interrupt_time);}
    printf("================================================================\n");
    char* alg_list[] = {"FCFS", "SJF", "PRI", "RR", "PRE_SJF", "PRE_PRI"};
    select_alg(sel);
    // for (int j = 1; j <= 6; j++) {
    //     printf("\n+++++++++++++++++++++++++%s+++++++++++++++++++++++++\n", alg_list[j-1]);
    //     select_alg(j);
    //     printf("\n=================== RESULT =====================\n");
    //     printf("%-13s%-13s%-13s%-13s\n", "PID", "Remaining", "Start", "End");
    //     for (int i = 0; i < num; i++) {
    // //         Process p = process_arr[i];
    //         printf("%-13d%-13d%-13d%-13d\n", 
    //         p.pid, p.remaining_time, p.start_time, p.end_time);}
    //     printf("================================================\n");

    //     print_gantt_chart(gantt_chart.gantt, gantt_chart.gantt_idx);

    //     evaluate(process_arr, num);

    //     config(&ready_queue, &waiting_queue, &gantt_chart);
    //     ready_queue = NewQueue;

    // }

    printf("\n=================== RESULT =====================\n");
    printf("%-13s%-13s%-13s%-13s\n", "PID", "Remaining", "Start", "End");
    for (int i = 0; i < num; i++) {
        Process p = process_arr[i];
        printf("%-13d%-13d%-13d%-13d\n", 
        p.pid, p.remaining_time, p.start_time, p.end_time);}
    printf("================================================\n");

    print_gantt_chart(gantt_chart.gantt, gantt_chart.gantt_idx);

    evaluate(process_arr, num);

    return 0;
}