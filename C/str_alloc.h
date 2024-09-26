/* 문자열 처리를 위해 동적 공간 할당을 위한 헤더파일입니다 */
/* str_alloc() 함수는 한 개의 인자를 받아, 인자 길이+1의 공간을 할당하고 그 주소를 반환합니다*/
/* str_array_alloc() 함수는 두 개의 인자를 받아,
	첫 번째 인자 길이+1의 문자열을 두번째 인자만큼 생성하고,
	해당 포인터 (즉, 첫 문자열의 시작주소를 담은 포인터변수의 주소)를 반환합니다.*/
/* 두 함수는, 메모리공간 할당 실패시 NULL을 반환하며, 이 경우 함수를 호출한 코드에서 확인해야 합니다
	ex.
	char **single_string = str_alloc(length);
	if (single_string == NULL){
		return 1;
	}

	char **string_array = str_array_alloc(length, number);
	if (string_array == NULL){
		return 1;
	} */
/* 또한, 사용 종료 후 메모리공간을 반환해야 합니다. free(single_string);*/
/* str_array_alloc()는 문자열배열과 각 문자열의 공간을 모두 해제해야 합니다.*/
/* free_str_array()는 이 작업을 처리합니다. */

char* str_alloc(int);	// 문자열 저장공간을 동적으로 할당하고, 주소를 반환하는 함수
char** str_array_alloc(int, int);	// 문자열 배열 공간을 동적으로 할당하고, 포인터를 반환
void free_str_array(char**, int, int);	// 이중포인터와 배열 크기, 문자열크기를 받아 메모리를 해제합니다.

char* str_alloc(int n){
	/* 동적 메모리 할당과정
		1. malloc( (n+1) * sizeof(char));	char은 1B이므로 총 6B의 메모리를 heap에 할당
		2. malloc()는 할당한 메모리의 주소를 void* 타입으로 반환함
			즉, 메모리 주소이므로 포인터이고, 타입 미정이므로 void임
		3. 할당된 공간에 문자를 저장하기 위해 (char *)이용하여 문자열 포인터로 형 변환
		4. str 이라는 포인터에 할당한 공간의 주소를 저장함.
		주. str은 주소를 담는 변수(포인터), *str은 해당주소의 데이터, &str은 포인터 자체의 주소임 */

	// 포인터 str을 선언하고, n+1개의 문자를 저장할 공간 할당
	char* str = (char *)malloc( (n+1) * sizeof(char));
	
	// 메모리 확인
	if(str == NULL){
		printf("메모리 할당 실패\n");
		return NULL;
	}

	return str; // 할당된 메모리 공간의 시작주소를 포인터 형식으로 반환
}


char** str_array_alloc(int n, int k){

	// 문자열 포인터 배열 동적 할당 시작
	/* 동적 메모리 할당과정
	cf. str = (char *)malloc( (n+1) * sizeof(char));
		1. malloc( (n+1) * sizeof(char));	char은 1B이므로 총 6B의 메모리를 heap에 할당
		2. malloc()는 할당한 메모리의 주소를 void* 타입으로 반환함
			즉, 메모리 주소이므로 포인터이고, 타입 미정이므로 void임
		3. 할당된 공간에 문자를 저장하기 위해 (char *)이용하여 문자열 포인터로 형 변환
		4. str 이라는 포인터에 할당한 공간의 주소를 저장함.
		주. str은 주소를 담는 변수(포인터), *str은 해당주소의 데이터, &str은 포인터 자체의 주소임 
			즉 문자열 시작주소 -> str

		1. k * sizeof(char *) : 문자포인터(문자열 시작주소) * k개 
		2. malloc() : 위 크기의 공간을 할당하고 그 주소를 void*로 반환
			형태는 char로 변환하되, *은 문자열시작주소, **은 문자열배열의 시작주소
		3. str_array = (char **)malloc() : 주소의 주소를 이중포인터에 저장*/
	char **str_array = (char **)malloc( k* sizeof(char *));

	if(str_array == NULL) {
		printf("메모리 할당 실패\n");
		return NULL;
	}

	// 문자열배열의 원소인 각 문자열 공간을 할당
	for (int i = 0; i<k ; i++) {	// 크기가 k이므로 0 ~ k-1 까지 반복수행
		str_array[i] = (char *)malloc( (n +1 ) * sizeof(char));
			// 문자 n+1개의 메모리 공간을 할당하고 주소를 str_array[i]로 반환
			// str_array는 이중 포인터이며, 각 원소는 포인터임
		if(str_array[i] == NULL){	// 문자열 할당에 한번이라도 실패한다면
			printf("메모리 할당 실패\n");
			// for 문 내에서 이전에 할당한 메모리가 있다면 모두 해제
			for (int j=0; j<i ; j++){	// 0 ~ i-1 까지, 즉 현재까지 수행횟수만큼
				free(str_array[j]);	// 할당된 문자열 해제
			}
			free(str_array);	// 문자열배열에 할당된 공간도 해제
			return NULL;	//비정상 종료
		}
	}
	return str_array;
}

void free_str_array(char** str_array, int n, int k){	// 이중포인터와 배열 크기, 문자열크기를 받아 메모리를 해제합니다.
	for(int i=0; i < k ; i++){
		free(str_array[i]);
	}
	free(str_array);
}
