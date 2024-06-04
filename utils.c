#include "utils.h"

void print_gantt_chart(int gantt[], int n) {
    printf("Gantt Chart:\n");
    printf("+");
    for (int i = 0; i < n; i++) {
        printf("----+");
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        if (gantt[i] == -1)
            printf("| -- ");
        else
            printf("| P%d ", gantt[i]);
    }
    printf("|\n");

    printf("+");
    for (int i = 0; i < n; i++) {
        printf("----+");
    }
    printf("\n");

    for (int i = 0; i <= n; i++) {
        printf("%-5d", i);
    }
    printf("\n");

    // Print I/O events row
    for (int i = 0; i < n; i++) {
        int io_event_found = 0;
        for (int k = 0; k < 2; k++) {
            if (i == io_times[k]) {
                printf("%-5s", "I/O");
                io_event_found = 1;
                break;
            }
        }
        if (!io_event_found) {
            printf("     ");
        }
    }
    printf("\n\n");
}