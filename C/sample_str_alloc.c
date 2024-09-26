#include <stdio.h>
#include <stdlib.h>
#include "str_alloc.h"
//#include <string.h>

int main() {

	int n = 5; // 문자열의 길이는 5, 널 문자 고려하면 n+1
	char *str = str_alloc(n);
	if (str == NULL){
		return 1;	// 메모리 할당 실패시 프로그램 종료
	}
	// 출력
	printf("문자열 str : %s\n", str);

	// 문자열 수정
	printf("최대 5글자를 입력하세요");
	scanf("%s", str);	// scanf()의 인자는 주소이며, 포인터에는 주소가 저장되어 있음.
	printf("새 문자열 str : %s\n", str);

	// 동적 할당된 메모리 해제
	free(str);


	n=10 ; // 널 문자 포함 최대 길이
	int k=5; // 문자열의 개수
	char **str_array = str_array_alloc(n , k);
	// 에러 확인
	if (str == NULL){
		return 1;	// 메모리 할당 실패시 프로그램 종료
	}
	// str_array 사용하기
	for(int i=0; i<k; i++){
		printf("[%2d] : %d번 문자를 입력하세요 : ", i, i+1);
		scanf("%s", str_array[i]); // str_array에 입력할 시작 주소가 저장되어 있음
	}

	// 출력하기

	printf("입력한 문자열을 출력합니다\n");
	for(int i=0; i < k; i++){
			printf("[%2d] %d번 : %s\n", i, i+1, str_array[i]);
	}
	// 메모리 해제
	free_str_array(str_array, n, k);
	return 0;
}
