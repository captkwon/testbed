/* 정규화된 문자열에서 데이터 추출하여 배열에 저장하기 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	char letter[10] = "This is t\0";
	char *lines = &letter[0];
	char *lines2 = "New Test";
//		printf("Enter a character : ");
//		scanf("%s", letter);


	printf("\n char letter[10] = ""This is test "" \n");
	printf("\n\n %%s letter : %s\n", letter);
	printf("%%c letter[0] : %c\n", letter[0]);
	printf("%%s &letter[0] : %s\n", &letter[0]);

	printf(" char *lines = &&letter[0]\n");
	printf("\n\n %%s lines : %s\n", lines);
	printf("%%c lines[0] : %c\n", lines[0]);
	printf("%%s &lines[0] : %s\n", &lines[0]);

	printf(" *lines2 = ""New Test"" \n");
	printf(" \n %%s lines2 : %s\n", lines2);
	printf("%%c lines2[0] : %c\n", lines2[0]);
	printf("%%s &lines2[0] : %s\n", &lines2[0]);
// 각 줄의 시작 위치를 정의한 매크로를 사용하여 접근합니다.

/*
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

	printf("\n sizeof(lines)= %lu",sizeof(lines));
	printf("\n sizeof(lines[0])= %lu",sizeof(lines[0]));
	printf("\n lines[0] = %s", lines[0]);
	for(i=0; i< 80; i++)
		printf("\nlines[%d] = %c", i, *lines[i]);

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
*/	
    return 0;
}

