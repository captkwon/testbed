/*
	목표 : SIM 카드의 SN값을 생성
	동작 : 정수 n을 입력받아 최대 n문자를 저장할 수 있는 char열 생성 
			- 정수 k를 받아 k개의 char[k] 생성
*/
	

#include <stdio.h>
#include <stdlib.h>

int main(){

	int i, n;
	char *charArray;

	printf("생성할 수의 자릿수를 입력하세요 : ");
	scanf("%d", &n);

	/* n개 문자를 저장할 메모리 공간을 할당하고, 그 주소를 charArray에 반환 
	charArray = (char *)malloc((n+1) * sizeof(char));
	// 메모리 초기화 위해 memset() 사용하려면 string.h 필요
	if (charArray != NULL) {
		memset(charArray, '\0',n+1);
	}*/
	// 메모리 초기화하여 할당
	charArray = (char *)calloc((n+1), sizeof(char));
	// 할당실패시
	if ( charArray == NULL)  {
		printf("메모리 할당에 실패함\n");
		return 1;
	}
	
	// 정상할당 후 S/N 입력받아 charArray에 저장
	printf("S/N을 입력하세요 (최대 %d 글자) : ", n);
	scanf("%s", charArray);
	// 배열 내용 출력
	for (i=0; i<n ; i++){
		printf("%d : %c\n", i, charArray[i]);
	}
	// 스트링 출력
	printf("%s\n", charArray);

	// 메모리 해제
	free(charArray);

	return 0;
}
