#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    FILE *fp = fopen("data.csv", "w");
    if (fp == NULL) {
        perror("Unable to open file!");
        return 1;
    }

    fprintf(fp, "Start Time,Bit Rate (Mbits/sec)\n");

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

            // Writing the parsed values to the CSV file
            fprintf(fp, "%.2f,%.2f\n", startTime, bitRate);
        }
    }

    fclose(fp);
    return 0;
}

