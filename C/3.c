#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum width and height of the graph
#define WIDTH 70
#define HEIGHT 20

// Function to find the maximum value in an array
double find_max(double *array, int size) {
    double max = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

// Function to plot the graph
void plot_graph(double *x, double *y, int size) {
    char graph[HEIGHT][WIDTH];
    memset(graph, ' ', sizeof(graph));

    // Find the maximum values for scaling
    double max_x = find_max(x, size);
    double max_y = find_max(y, size);

    // Plot the points on the graph
    for (int i = 0; i < size; i++) {
        int plot_x = (int)(x[i] / max_x * (WIDTH - 1));
        int plot_y = (int)(y[i] / max_y * (HEIGHT - 1));
        graph[HEIGHT - 1 - plot_y][plot_x] = '*';
    }

    // Print the graph
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(graph[i][j]);
        }
        putchar('\n');
    }

    // Print x-axis labels
    printf("\nStart Time (sec)\n");
    printf("0");
    for (int i = 1; i < WIDTH - 2; i++) {
        if (i % 10 == 0) {
            printf("|");
        } else {
            printf("-");
        }
    }
    printf("> %.2f\n", max_x);

    // Print y-axis labels
    printf("Bit Rate (Mbits/sec)\n");
    for (int i = 0; i < HEIGHT; i++) {
        if (i % 2 == 0) {
            printf("| %.2f\n", max_y * (HEIGHT - 1 - i) / (HEIGHT - 1));
        } else {
            printf("|\n");
        }
    }
}

int main() {
    const char* lines[] = {
        "[SUM]   0.00-0.19   sec  0.00 Bytes  0.00 bits/sec", 
        "[SUM]   0.19-0.21   sec  0.00 Bytes  1.00 Gbits/sec", 
        "[SUM]   0.21-0.31   sec  1.12 MBytes  91.8 Mbits/sec", 
        "[SUM]   0.31-0.41   sec  5.50 MBytes   447 bits/sec", 
        "[SUM]   0.41-0.51   sec  10.9 MBytes   904 kbits/sec", 
        "[SUM]   9.70-9.82   sec  10.0 MBytes   729 Mbits/sec", 
        "[SUM]   9.82-9.91   sec  8.12 MBytes   784 Mbits/sec", 
        "[SUM]   9.91-10.01  sec  9.38 MBytes   760 Mbits/sec", 
        "[Total]   0.00-10.03  sec   897 MBytes   750 Mbits/sec", 
        "[Total]   0.00-10.01  sec   890 MBytes   746 Mbits/sec"
    };

    double startTimes[10];
    double bitRates[10];
    int index = 0;

    for (int i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
        const char* line = lines[i];
        if (strncmp(line, "[SUM]", 5) == 0) {
            double startTime, endTime, byteSize, bitRate;
            char byteUnit[10], bitUnit[10];

            // Parsing the line
            sscanf(line, "[SUM] %lf-%lf sec %lf %sBytes %lf %sbits/sec", &startTime, &endTime, &byteSize, byteUnit, &bitRate, bitUnit);

            // Convert bitRate to Mbits/sec if necessary
            if (strcmp(bitUnit, "G") == 0) {
                bitRate *= 1000;  // Convert Gbits to Mbits
            } else if (strcmp(bitUnit, "k") == 0) {
                bitRate /= 1000;  // Convert kbits to Mbits
            }

            // Store the parsed values
            startTimes[index] = startTime;
            bitRates[index] = bitRate;
            index++;
        }
    }

    // Print the parsed values
    printf("Start Time (sec) | Bit Rate (Mbits/sec)\n");
    printf("-----------------|----------------------\n");
    for (int i = 0; i < index; i++) {
        printf("%15.2f | %20.2f\n", startTimes[i], bitRates[i]);
    }
    printf("\n");

    // Plot the graph
    plot_graph(startTimes, bitRates, index);

    return 0;
}

