/* 정규화된 문자열에서 데이터 추출하여 배열에 저장하기 */

// 문자열은 배열 char형 포인터배열 lines[]에 저장되어 있음
// char은 1B(문자 하나)이지만, 이를 가리키는 주소는 8B임
// lines[]는 포인터의 배열이므로, 8B*원소개수의 사이즈를 가짐
// 현재 파일은 사전 저장된 배열에서 해당 문자열이
// [SUM]으로 시작할 때, 마지막 bps를 구하여 dts[]와 unts[] 에 저장

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	char* lines[] = {
        "[SUM]   0.00-0.19   sec  0.00 Bytes  0.00 bits/sec", 
        "[SUM]   0.19-0.21   sec  0.00 Bytes  1.00 Gbits/sec", 
        "[SUM]   0.21-0.31   sec  1.12 MBytes  91.8 Mbits/sec", 
        "[SUM]   0.31-0.41   sec  5.50 MBytes   447 bits/sec", 
        "[ABC]",
		"[SUM]   0.41-0.51   sec  10.9 MBytes   904 kbits/sec", 
        "[SUM]   9.70-9.82   sec  10.0 MBytes   729 Mbits/sec", 
        "[SUM]   9.91-10.01  sec  9.38 MBytes   760 Mbits/sec", 
        "[Total]   0.00-10.03  sec   897 MBytes   750 Mbits/sec", 
        "[Total]   0.00-10.01  sec   890 MBytes   746 Mbits/sec"
    };
	float dts[10];
	char unts[10];
	int i;

	// sizeof(lines)/sizeof(lines[0]) = 원소개수*8B / 8B는 원소개수임
	// 즉 lines[]의 원소개수만큼 반복 수행하는 for 문장임
    for (i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
		// 배열 초기화
		unts[i] = 'X';	// 기본값
		
		// strncmp(첫 문장, 두번째 문장, 비교할 개수)
		// 결과 두 문장이 같으면 0, 첫 문장이 크면 양수, 둘째 문장이 크면 음수 반환
		printf("i=%d, string : %s\n", i, lines[i]  );	// 배열 내용 확인

		if (strncmp(lines[i], "[SUM]", 5) == 0){ 	// i번째 라인의 5글자를 비교하여 같으면 0 반환
	        sscanf(lines[i], "%*s %*s %*s %*f %*s %f %cbits/sec", &dts[i], &unts[i]);	// 데이터 파싱
			if (unts[i] == 'b' ) 
				unts[i] = '\0';	// bps의 단위가 없을 경우
		}
		
	}

	//for(i=0; i<sizeof(lines) / sizeof(lines[0]) ; i++)
	//	printf("dts[%d] = %.2f, unts[%d] = %c\n", i, dts[i],i, unts[i]);
	
	// 결과 출력

	for(i=0; i<sizeof(lines) / sizeof(lines[0]) ; i++){
		if(unts[i] != 'X')	// 파싱한 행만 출력
			printf(" %d line : %.2f %cbps\n", i, dts[i], unts[i]);
	}
    return 0;

}
