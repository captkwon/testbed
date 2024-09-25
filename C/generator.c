/*
	목표 : SIM 카드의 SN값을 생성
	동작 : 정수 n을 입력받아 최대 n문자를 저장할 수 있는 char열 생성 
			- 정수 k를 받아 k개의 char[k] 생성
*/
	

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	int i,k, n;

	printf("생성할 수의 자릿수를 입력하세요 : ");
	scanf("%d", &n);
	printf("문자열 개수를 입력하세요 : ");
	scanf("%d", &k);

	// 전체 메모리 초기화하여 할당
	char **strArray = (char **)calloc(k, sizeof(char *));

	if ( strArray == NULL)  {
		printf("메모리 할당에 실패함\n");
		return 1;
	}
	
	// 각 문자열에 대한 메모리 할당 및 초기화
	for ( i = 0; i< k ; i++){
		strArray[i] = (char *)calloc(n+1, sizeof(char));
		if (strArray[i] == NULL){
			printf("메모리 할당에 실패함\n");
			// 이전에 할당한 메모리 해제
			for (int j=0; j<i; j++){
				free(strArray[j]);
			}
			free(strArray);
			return 1;
		}
	
		// 할당된 메모리를 '0'으로 초기화
		memset(strArray[i], '0', n); //n개의 문자를 '0'으로 초기화
		strArray[i][n] = '\0';	// 마지막은 null문자 추가
	}

	// 초기 상태 출력
	printf("초기 상태 문자열 배열:\n");
	for( i=0; i<k; i++){
		printf("strArray[%d] : %s\n", i, strArray[i]);
	}
/*
	// 정상할당 후 S/N 입력받아 charArray에 저장
	printf("S/N을 입력하세요 (최대 %d 글자) : ", n);
	scanf("%s", charArray);
	// 배열 내용 출력
	for (i=0; i<n ; i++){
		printf("%d : %c\n", i, charArray[i]);
	}
	// 스트링 출력
	printf("%s\n", charArray);
*/
	// 메모리 해제
	for( i=0; i<k; i++){
		free(strArray[i]);	//각 문자열에 대한 메모리 해제
	}
	free(strArray);	// 문자열 배열에 대한 메모리 해제

	return 0;
}
