#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "str_alloc.h"
#define HEX_SET "0123456789ABCDEF"

char** input_func(int *, int *);
char** initialized(char **str, int, int);
char** mk_sn(char **str, int, int);
void show_arr(char **str, int, int);
int mk_csv(char **str, int, int);
int load_csv();

int main(){
	int i;
	int length, count;
	short action;
	char** numbers;

	// 데이터 입력 부분 (문자열 길이와 개수를 인자로 전달하고 문자열배열 반환
	numbers = input_func(&length, &count);
	if (numbers == NULL){
		printf("데이터 입력 실패");
		return 1;
	}

	// 데이터 초기화
	numbers = initialized(numbers, length, count);
	if (numbers == NULL){
		printf("초기화 실패");
		return 1;
	}
	show_arr(numbers, length, count);

	// 값 채우기
	do{ 
		numbers = mk_sn(numbers, length, count);
		if(numbers == NULL){
			printf("값 채우기 실패");
			return 1;
		}

		// 내용 확인
		show_arr(numbers, length, count);

		printf("종료하려면 0을 입력하세요");
		scanf("%hd", &action);
	}
	while ( action != 0);

	printf("저장합니다");
	mk_csv(numbers, length, count);

	printf("저장한 파일을 로드합니다");
	load_csv();

	// 사용한 메모리 해제
	free_str_array(numbers, length, count);
	return 0;
}
// CSV 파일을 읽어오는 함수
int load_csv(){
	FILE *fp = fopen("list.csv", "r");
	if(fp == NULL){
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	char line[1024];	// 한줄을 저장할 버퍼
	while (fgets(line, sizeof(line), fp)) {
		// 줄 끝의 개행 문자 제거
		/* size_t strcspn(const char *str1, const char *str2);
			문자열 str1에서 문자들의 집합 str2가 처음 등장하는 위치를 찾고, 문자가 나올때 까지의 길이를 반환
		*/
		line[strcspn(line, "\n")] = 0;

		// 쉼표로 구분된 각 값을 처리
		/* char *strtok(chat *str, const char *delim);
			문자열을 구분자를 사용하여 토큰 단위로 분리하는 함수
			첫 호출에서 문자열과 구분자를 전달하고, 이후에는 NULL을 전달하여 남는 연을 계속 분리
			str : 분리할 대상 문자열. 첫 호출에서 전달하지만 이후에는 NULL을 전달하여 문자열을 이어서 처리함
			delim : 구분자, 여기 있는 문자 중 하나라도 만나면 분리
			반환값 : 분리된 토큰(문자열의 일부), 더 이상 구분자가 없으면 NULL 반환
		*/
		char *token = strtok(line, ",");
		while(token != NULL){
			printf("%s ", token);	// 쉼표로 구분된 각 값을 출력
			token = strtok(NULL, ",");
		}
		printf("\n");
	}
	fclose(fp);
	return 0;
}

// 배열을 CSV로 저장하는 함수
int mk_csv(char **str, int length, int count){
	FILE *fp = fopen("list.csv", "w");	// 쓰기 모드로 파일 열기
	if (fp == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}
	//배열의 내용을 파일로 저장
	for (int i=0; i<count; i++){
		fprintf(fp, "%s\n", str[i]);	// 각 문자열을 줄바꿈으로 구분하여 저장
	}
	
	fclose(fp);	// 파일 닫기
	printf("결과를 list.csv 파일로 저장하였습니다.\n");

	return 0;
}

// 입력한 데이터 확인하는 함수
void show_arr(char **str, int length, int count){
	printf("       %s\n", str[0]);
	printf("       1234567890123456789012345678901234567890\n");
	for (int i = 1; i < count ; i++){
		printf("%5d: %s\n", i, str[i]);
	}
}

char** mk_sn(char **str, int length, int count){
	// 일련번호 부여함수
	char* temp;
	int start, size, end;
	short data_type;
	unsigned int no;
	int i, j;

	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("변경값의 시작 위치를 입력하세요");	// 시작자리를 start에 입력
		scanf("%d", &start);
	} while ( start < 1 || start > length );
	
	do{
		printf("문자 길이를 입력하세요");	// 일련번호 자릿수를 size에 입력(인덱스+1)
		scanf("%d", &size); 
	} while ( size < 1 );
	end = start + size -1;
	if ( end > length){	// 끝나는 자리가 length보다 크면
		end = length; // 끝자리를 length로 고정
		size = end - start +1;	// 따라서 사이즈도 수정
	}
	temp = str_alloc(size);	// 자릿수만큼 문자열공간 할당(실제로는 \0포함 size+1)
	if (temp == NULL) {
		printf("임시 문자열 할당 실패");
		return NULL;
	}

	// 생성할 데이터 종류 결정

	do{
		printf("입력할 데이터 종류 선택 (1: Fixed No. 2: Serial No. 3: Random HEX No.) : ");
		scanf("%hd", &data_type);
	} while( data_type <1 || data_type > 3 ) ;	// 정상 입력이면 종료

	switch (data_type){	// 입력에 따라 이름 결정 
		case 1:
			printf("선택한 자리를 특정 문자열로 대치합니다\n");
			printf("문자열을 입력하세요");
			while (getchar() != '\n');
			scanf("%s", temp);
/* 이 블럭은 엔터치면 에러남 (fgets()는 \n도 입력)
			if (fgets(temp, size+1, stdin) != NULL ){
				printf("123456789012345678901234567890\n");
				printf("%s 가 입력되었습니다\n", temp);
			} else{
				return NULL;
			} // 대치할 문자열이 temp에 저장
*/
			if (strlen(temp) < size){ // temp가 size보다 작으면
				for(i=0, j=strlen(temp); j < size ; i++, j++){
					temp[j] = temp[i];
				}
				temp[size] = '\0';
			}
			for (i=1; i<count; i++){	// 제목열 제거
				for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
					str[i][j] = temp[j-start+1];
				}
			}
			while (getchar() != '\n');	// scanf 입력의 개행문자 제거
			break;
		case 2:
			printf("선택한 자리를 일련번호로 대치합니다\n");
			printf("일련번호 시작값을 입력하세요");
			scanf("%d", &no);	// 큰 수 입력 대비 unsigned로 입력

			/* int snprintf(char *str, size_t size, const char *format, ...);
				문자열을 지정된 형식대로 버퍼에 출력 
				1. 반환할 문자열은 *str에 저장
				2. size_t size 는 버퍼에 저장할 최대 길이로 널 문자 포함됨
				3. const char *format 는 변환할 데이터의 형식, *을 사용하면 뒤에 *의 내용 필요
					(*에 대한 변수명)
				4. ... 변환할 실제 값
				5. int 형식의 반환값은 문자열의 길이임(무시 가능)
				즉, snprintf(temp, size+1, "%0*d", size, i);
					: temp에 저장하는 문자열은 i를 문자로 바꾼 값인데,
						"%0*d" 에 의해 전체 길이는 size가 되도록 유효숫자를 0으로 채우고,
							temp의 크기는 size+1 임(null 포함)
			*/
				   
			for (i=1; i<count; i++){	// 제목열 제거
				snprintf(temp, size+1, "%0*d", size, no);
				for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
					str[i][j] = temp[j-start+1];
				}
				no++;
			}
			break;
		case 3:
			printf("선택한 자리를 임의의 문자열로 대치합니다\n");
			srand(time(NULL));	// 시드 초기화
			short set_size = sizeof(HEX_SET) -1;	// 널 문자를 제외한 크기
			for (int i=1; i<count ; i++){
				for (j=start-1; j < end ; j++){
					str[i][j] = HEX_SET[rand() % set_size];	// 랜덤 인덱스 할당
				}
			}
			break;
	}
	free(temp);

	return str;
}


// 배열 초기화 함수
char** initialized(char **str, int length, int count){
	char* temp;

	temp= str_alloc(length);	// 초기값 할당을 위한 메모리 공간 할당
		if(temp ==NULL) return NULL;
	printf("초기화할 값을 입력하세요");

	/* fgets로 문자열 입력받기
		형식 fgets(char *str, int size, FILE *stream);
		- stream은 입력을 받을 스트림으로, stdin 을 사용하면 키보드 입력을 처리함
		- size는 입력받을 최대 문자수로, \n이 포함되므로 실제 입력문자는 size -1임. 주로 sizeof(함수와 함께 사용)
		- str은 입력을 저장할 문자 배열임
		- 입력에 \n이 입력되면 그것도 문자열이며, 입력문자가 size-1이 되거나, \n을 만나면 입력을 종료함.
		- 즉, 엔터만 치면 \n\0 을 str에 저장하고, str(실패시 NULL)을 반환 
		getchar() 이용 입력 버퍼 처리하기
		- scanf()는 입력 후\n을 처리하지 않으므로, 버퍼의 문자 제거 필요
*/
	while(getchar() != '\n');	// 하나의 문자를 반복 읽으면서 개행문자이면 버퍼를 비우고 종료
	if( fgets(temp, length+1, stdin) != NULL) { 	// fgets로 \0포함 입력받고 null이 아니면
		for (int i=0; i<length; i++){
			if (temp[i] == '\n')	// 입력된 temp의 각 자리를 확인
				temp[i] = '\0';		// \n(엔터)가 입력되었으면 그자리를 종료로
			if (temp[i] == '\0')	// 입력이 종료이면
				temp[i] = ' ';	// 그자리를 '_'으로 채우기
		}
		temp[length] = '\0';	// length +1 번째 값은 종료문자
	} else{
		return NULL;	// 할당 실패시 NULL 반환
	}
	// 문자열 temp 확인
// 배열 수를 길이만큼 입력하면 초기화시 에러 발생 -> 디버깅할 것
	for (int i=1; i<count; i++){	// 첫행은 제목임
		strcpy(str[i], temp);	// \0포함하여 temp의 문자열을 str[i]에 카피
	}
	// 메모리 temp 해제
	free(temp);
	return str;
}

// 데이터 입력함수
char** input_func(int *str_len, int *str_cnt){
	char str_name[6];	// 입력할 데이터 이름
	short choice;	// 입력할 데이터 종류
	char** str_array;	// 생성할 배열(반환값)

	// 생성할 데이터 테이블 결정
	do{
		printf("입력할 데이터 선택(1: S/N 2: ICCID, 3: IMSI, 4: K, 5: OPc) : ");
		scanf("%hd", &choice);
	}
	while( choice <1 || choice >5 ) ;	// 정상 입력이면 종료

	switch (choice){	// 입력에 따라 이름 결정 
		case 1:
			strcpy(str_name, "S/N");
			*str_len = 10;
			break;
		case 2:
			strcpy(str_name, "ICCID");
			*str_len = 16;
			break;
		case 3:
			strcpy(str_name, "IMSI");
			*str_len = 16;
			break;
		case 4:
			strcpy(str_name, "K");
			*str_len = 32;
			break;
		case 5:
			strcpy(str_name, "OPc");
			*str_len = 32;
			break;
	}
	printf("길이 %d인 %s를 선택하였습니다. 몇 개 생성할까요?", *str_len, str_name);
	scanf("%d", str_cnt);
	*str_cnt = *str_cnt+1;	// 제목 1행 추가

	str_array = str_array_alloc(*str_len, *str_cnt);	// str_len+1 * str_cnt 인 문자열 할당
	
	if (str_array == NULL){
		return NULL;
	}
	strcpy(str_array[0], str_name);
	/* 5dd 길이 확인용 임시(제목행 길이를 선택크기만큼 맞춤)*/
	for(int i= strlen(str_name); i< *str_len; i++){
		str_array[0][i] = '_';
	}
	str_array[0][*str_len] = '\0';

	return str_array;
}
