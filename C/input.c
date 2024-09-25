// 동적 메모리 할당 예제(int / char)
#include <stdio.h>
#include <stdlib.h>

int main(){
	int n,i;
	int *intArray;
	char *charArray;

	printf("정수 배열의 크기 입력 : ");
	scanf("%d", &n);

	intArray = (int *)malloc(n * sizeof(int));

	if (intArray == NULL) {
		printf("메모리 할당에 실패\n");
		return 1;
	}
	printf("%d개의 정수를 입력하세요: \n", n);

	for (i=0; i<n ; i++){
		printf("%d 번째 정수 : ",i);
		scanf("%d", &intArray[i]);
	}

	printf("입력된 정수들은:\n");
	for (int i = 0; i <n; i++){
		printf("%d 번지 : %d\n", i, intArray[i]);
	}
	free(intArray);

	printf("배열의 크기를 입력하세요: ");
	scanf("%d", &n);

	charArray = (char *)malloc((n+1) * sizeof(char));
	
	if (intArray == NULL) {
		printf("메모리 할당에 실패\n");
		return 1;
	}

	printf("문자열을 입력하세요 (최대 %d 글자): ", n);
	scanf("%s", charArray);

	for (i=0; i<n;i++){
		printf("%d번지 : %c\n", i, charArray[i]);
	}
	printf("%d번지 : \\0 (NULL 문자)\n", n);
	free(charArray);


	
	return 0;
}
