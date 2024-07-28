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
	double datas[4][10];
	char units[2][10];
	int i;

    for (i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
		const char* line = lines[i];
        //if (strncmp(line, "[SUM]", 5) == 0) {
            // Parsing the line
            // Printing the parsed values
            sscanf(line, "[SUM] %lf-%lf sec %lf %cBytes %lf %cbits/sec", &datas[0][i], &datas[1][i], &datas[2][i], &units[0][i], &datas[3][i], &units[1][i]);
            sscanf(line, "[Total] %lf-%lf sec %lf %cBytes %lf %cbits/sec", &datas[0][i], &datas[1][i], &datas[2][i], &units[0][i], &datas[3][i], &units[1][i]);
            printf("Parsed from line: %s, i= %d \n", line, i);
      	//}
	}

	printf("\ndata[i] | startTime | endTime | byteSeze | byteUnit | bitRate | bitUnit\n");
	for (i=0; i<sizeof(lines) / sizeof(lines[0]); i++){
		printf("    %d   |   %2.2f   |  %2.2f  |  %2.2f   |     %c    |  %.2f |  %c\n", i, datas[0][i], datas[1][i], datas[2][i], units[0][i], datas[3][i], units[1][i]);
	}

    return 0;
}

