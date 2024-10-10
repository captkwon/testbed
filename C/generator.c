// 헤더파일 및 상수, 전역변수 선언
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>	// uint8_t 자료형 사용
#include <time.h>	// mk_sn_info()에서 사용
#include <ctype.h>	// isxdigit() 사용
#include "tiny-AES-c/aes.h"
			
#define HEX_SET "0123456789ABCDEF"
#define IIN "898299"	// Issuer Identification Number : SJ ???
#define PLMN "45040"
#define SN 12
#define ICCID 20
#define IMSI 15
#define KOP 32

int cnt=0;	// 개수

// 함수원형 선언
void help();
int save(char **sn, char **iccid, char **imsi, char **ki, char **opc, char *op);
void show_arr(char **str, int);
void comp_arr(char **arr1, int len1, char **arr2, int len2 );
char** arr_gen(char *name, int len);	// 제목문자열, 길이를 인수로 받음
int mk_sn_info(char **sn, int len);
int mk_iccid_info(char **iccid, int len, char **sn);
	char luhn(char* data);
int mk_imsi_n_ki(char **imsi, char **ki, char **iccid);
int mk_op(char* op, int len, char *sn, char *iccid, uint8_t *opb);
int mk_opc_info(uint8_t *opb, int len_b, char** ki, char** opc);
int edit_value(char **str, int len);
	int get_index(int len, int *size, int *end);	// edit_value()에서 일련번호, 난수 부여시 사용

// main 함수
int main(){

	char **sn, **iccid, **imsi, **ki, *op, **opc;	// Serial No., ICCID, IMSI, Ki
	int error = 0;	// 에러 플래그
	uint8_t opb[KOP/2];

	// 개수 설정
	printf(" 세종텔레콤 SIM data generator를 구동합니다.\n\n");
	printf("SIM 몇 개를 만들겠습니까? ");
	scanf("%d", &cnt);
	printf("SIM card 정보 %d개를 생성합니다.\n", cnt);
	cnt++;	// 머릿글행 용 1행 추가 

	// 동적 공간 할당 및 배열 생성
	sn = arr_gen("[S/N No]", SN);
	iccid = arr_gen("[ ICCID ]", ICCID);
	imsi = arr_gen("[ IMSI ]", IMSI);
	ki = arr_gen("[  Ki  ]", KOP);
	opc = arr_gen("[ OPc ]", KOP);
	if ( sn == NULL || iccid == NULL || imsi == NULL || ki == NULL || opc == NULL ){
		printf("배열 생성에 실패했습니다");
		return 1;
	}
	
	// SN 생성
	error = mk_sn_info(sn, SN);
	if ( error == 1){
		printf("종료합니다");
		return 1;
	}
	error = mk_iccid_info(iccid, ICCID, sn);
	error = mk_imsi_n_ki(imsi, ki, iccid);
	
	// OP 생성
	error = mk_op(op, KOP, sn[1], iccid[1], opb);

	// opc 생성 전
	//comp_arr(ki, KOP,opc, KOP);
	//error = mk_opc_info(ki, opc, KOP/2, op);
	error = mk_opc_info(opb, KOP/2, ki, opc);

	comp_arr(ki, KOP, opc, KOP);

	// Test
	error = save(sn, iccid, imsi, ki, opc, op);
	help();
	for( int i = cnt; i >= 0 ; i--){
		free(sn[i]);
		free(iccid[i]);
		free(imsi[i]);
		free(ki[i]);
		free(opc[i]);
	}
	free(sn);
	free(iccid);
	free(imsi);
	free(ki);
	free(opc);
	free(op);

	return 0;
}


// 함수 작성 
// 최종 설명 함수
void help(){
	printf("\n결과를 sim_info.csv, hfr_form.csv, ki.txt 로 각각 저장하였습니다.\n ");
	printf(" Serial No. : SJ(세종텔레콤) nn(수요기업 코드) YY(발행년도) nnnnnn(일련번호)\n");
	printf(" ICCID : %s(IIN) vv(코어벤더) nn(사이트) nn(네트워크) nnnnnn(일련번호) C(패리티) F(패딩)\n", IIN);
	printf("  - IIN 구성 : MII 89 (ISO/IEC 7812-1), Country code 82 (ITU-T Rec. E.164)\n  - 9 (세종텔레콤)\n");
	printf("  - 패리티 : Luhn 알고리즘 사용(ITU-T Rec. E.118)\n");
	printf(" IMSI : PLMN %s nnnnnn(일련번호)\n", PLMN);
	printf(" Ki : random hex열 \n");
	printf(" OP : 세종텔레콤 생성 random hex열 \n");
	printf(" OPc : OP와 Ki를 이용 milenage 알고리즘으로 생성(3GPP TS 33.501)\n\n");
	printf("                         2024 세종텔레콤 사업개발팀 \n\n");
}
// 파일로 저장하는 함수
int save(char **sn, char **iccid, char **imsi, char **ki, char **opc, char *op){
	int i;
	FILE *fp_csv = fopen("sim_info.csv", "w");	// 쓰기 모드로 파일 열기
	FILE *fp_hfr = fopen("sim_hfr.csv", "w");
	FILE *fp_txt = fopen("ki.txt", "w");
	if (fp_csv == NULL || fp_hfr == NULL || fp_txt == NULL){
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}
	fprintf(fp_txt, "OP : %s\n", op);
	for (i = 0; i < cnt ; i++){
		fprintf(fp_csv, "%d,%s,%s,%s,%s,%s\n", i, sn[i], iccid[i], imsi[i], ki[i], opc[i]);
		fprintf(fp_hfr, "nano\n%s\nmilenage\n%s\n%s\n%s\n'%s\n", iccid[i], opc[i], ki[i], imsi[i], imsi[i]+4 );
		fprintf(fp_txt, "%s\n", ki[i]);
	}
	fprintf(fp_csv, "\n OP : %s", op);

	fclose(fp_csv);	// 파일 닫기
	fclose(fp_hfr);
	fclose(fp_txt);
	
	return 0;
}

// OPc 산출 함수
int mk_opc_info(uint8_t *opb, int len_b, char** ki, char** opc) {
	uint8_t kib[len_b], ope[len_b];
	struct AES_ctx ctx;

	// step 1. 바이너리로 변환된 OP 확인 *opb로 인수 확인
	char* temp = (char*)malloc( 3 * sizeof(char));
	for (int i=1; i < cnt; i++) {	
		// step 2. ki를 바이너리로 변환
		for (int j = 0; j < len_b ; j++){
			temp[0] = ki[i][2*j];
			temp[1] = ki[i][2*j+1];
			temp[2] = '\0';	// 두 글자씩 슬라이스
			kib[j] = (uint8_t)strtol(temp, NULL, 16);	// 16진수로 변환
		} 
		// step3. kib[i]를 이용하여 AES 초기값 생성
		AES_init_ctx(&ctx, kib);

		// step4. opb를 ope로 카피
		memcpy(ope, opb, 16);

		// step5. ope를 ECB 모드로 암호화
		AES_ECB_encrypt(&ctx, ope);

		// step6. ope를 opb와 XOR 연산 수행하여 ope에 저장
		for (int j = 0; j < len_b ; j++){
			ope[j] = ope[j] ^ opb[j];
			opc[i][j*2] = (ope[j] >> 4) + ((ope[j] >>4) < 10 ? '0' : 'a' - 10);
			opc[i][j*2 +1] = (ope[j] & 0x0F ) + ((ope[j] & 0x0F) < 10 ? '0' : 'a' - 10);
		}
		opc[i][len_b*2]='\0';
	}
	free(temp);
	return 0;
}
// OP를 생성하는 함수
int mk_op(char* str, int len, char *sn, char *iccid, uint8_t *opb){
	int j, seed = 0;
	for (j=0; j < ICCID; j++){
		if(j < SN){
			seed += sn[j];
		}
		seed += iccid[j];
	}
	srand(seed);	// 위에서 구한 값을 seed로 사용
	for (j=0; j < KOP ; j++){
		str[j] = HEX_SET[rand() % 16]; // 랜덤 인덱스 할당
	}
	str[KOP] = '\0';
	printf("\n현재 OP는 %s입니다. 다른 값으로 ", str);

	int choice;
	do{
		printf("바꾸시겠습니까? (0. 아니오, 1. 입력값으로 대치) : ");
		scanf("%d", &choice);
		while(getchar() != '\n');
		if (choice == 1) {	// 변경할 경우 
			char temp[KOP+1];	// null 포함 33자 저장 공간
			printf("바꿀 %d자리 HEX값을 입력하세요. : " , KOP );
			// 0x53 0x4a 0x35 0x47 0x5f 0x4b 0x4f 0x4c 0x4f 0x4e 0x47 0x4c 0x4f 0x42 0x41 0x4c
			printf("SJ5G_KOLONGLOBAL (ASCII) : 53 4a 35 47 5f 4b 4f 4c 4f 4e 47 4c 4f 42 41 4c \n");
			fgets(temp, sizeof(temp), stdin);
			temp[strcspn(temp, "\n")] = '\0';	// 개행 문자 포함시 제거
			if ( strlen(temp) != KOP){	// OP가 짧을 경우
				printf("입력한 %s는 %lu 문자입니다. 바꿀수 없습니다. 다시 ", temp, strlen(temp));
			} else { // OP 자릿수가 맞다면
				for (j=0; j<strlen(temp); j++)	{
					if ( !isxdigit(temp[j] )) {
						printf("%d번째 문자 %c는 16진수가 아닙니다. 다시 ", j+1, temp[j]);
					choice = -1;	// 입력이 잘못되면 choice 변경하고 종료
					break;	// 더이상 for 반복 불필요
					}
				}
				if (choice == 1 ) { 	// 여전히 choice가 1이면(입력이 정상이면)
					printf("%s에서 ", str);
					memcpy(str, temp, KOP+1);
					printf("%s로 바꿨습니다. 다시 ", str);
				}	
			}	
		}
	} while ( choice != 0 );	// 입력이 0이 아니면 반복(0일때 종료)
		printf("3GPP TS 33.501 에 따라 milenage 알고리즘으로 OPc를 만들겠습니다.\n");

	// op를 바이너리로 변환하여 opb 생성
	char temp[3];
	for (int j = 0; j < KOP/2 ; j++){
		temp[0] = str[2*j];
		temp[1] = str[2*j+1];
		temp[2] = '\0';	// 두 글자씩 슬라이스
		opb[j] = (uint8_t)strtol(temp, NULL, 16);	// 16진수로 변환
	}

	return 0;
}

// IMSI 정보를 생성하는 함수
int mk_imsi_n_ki(char** imsi, char** ki, char **iccid){
	int j, seed=0;
	short set_size = sizeof(HEX_SET) -1;	// \0을 제외한 크기

	// 시드 초기화 -> Random 함수 변경 필요
	srand(seed);

	for (int i=1; i<cnt; i++){
		strcpy(imsi[i], PLMN);
		for(int j=5 ; j< IMSI ; j++){	// 시작값 45040 할당
			imsi[i][j] = iccid[i][j+3];	// IMSI 세팅
		}
		imsi[i][IMSI] = '\0';	// 종료문자 배치

		for (j=0; j < ICCID ; j++){		// seed 세팅
			if(i % 5 == 0){
				seed = (seed % 10 ) * 5;
			}
			seed += iccid[i][j];	// ICCID의 checksum에 의해 임의 변화 누적
		}
		srand(seed);		// 행마다 seed 재생성

		for(j=0; j< KOP ; j++){	// Ki 세팅
			ki[i][j] = HEX_SET[rand() % set_size];	// 랜덤 인덱스 할당
		}
		ki[i][KOP] = '\0';
	}


	// 생성한 IMSI와 Ki정보 확인
	printf("완성된 ICCID를 이용해 IMSI와 Ki의 초안을 만들었습니다.");
	comp_arr(imsi, IMSI, ki, KOP);

	// 데이터 수정 블록
	printf("IMSI를 ");
	int error = edit_value(imsi, IMSI);

	printf("Ki를 ");
	error = edit_value(ki, KOP); 

	return error;
}

// 배열에 ICCID를 생성하여 채우는 함수
int mk_iccid_info(char **iccid, int len, char **sn){
	int i, j;
	char temp[3];
	int option[3];	//[0] 코어종류 [1] 현장번호 [2] 슬라이스번호

	printf("ICCID를 생성하겠습니다. ");
	// 장비제조사 입력
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("Core 제조사를 입력하세요. (1.삼성, 2.NOKIA, 3.HFR) : ");	// 시작자리를 start에 입력
		scanf("%d", &option[0]);
	} while ( option[0] < 1 || option[0] > 3 );	// 범위 밖이면 반복
	// 현장번호 입력
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("세종텔레콤의 몇 번째 특화망 사이트입니까? (0-99) : ");	// 시작자리를 start에 입력
		scanf("%d", &option[1]);
	} while ( option[1] < 0 || option[1] > 99 );	// 범위 밖이면 반복
	// 슬라이스번호 입력
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("현재 Core의 몇번째 네트워크입니까? (0-99) : ");	// 시작자리를 start에 입력
		scanf("%d", &option[2]);
	} while ( option[2] < 0 || option[2] > 99 );	// 범위 밖이면 반복

	// 초기 ICCID 입력
	for (i=1; i<cnt; i++){
		for( j=0 ; j< len-2 ; j++){	// parity와 padding digit 제외 
			if(j < 6){
				iccid[i][j] = IIN[j];	// IIN 할당
			} else if (j < 8){
				snprintf(temp, 3, "%02d", option[0]);	// 입력한 정수를 %02d 형식의 문자렬로 변환
				iccid[i][j] = temp[j-6];
			} else if (j < 10){
				snprintf(temp, 3, "%02d", option[1]);
				iccid[i][j] = temp[j-8];
			} else if (j < 12){
				snprintf(temp, 3, "%02d", option[2]);
				iccid[i][j] = temp[j-10];
			}
			else { 
				iccid[i][j] = sn[i][j-6];		// sim의 일련번호 6자리 채우기
			}
		}
		iccid[i][ICCID-1] = '\0';
	}

	printf("입력한 정보와 Serial No. 뒷 6자리에 따라 ICCID 초안을 만들었습니다. \n");
	// 생성한 ICCID정보 확인
	comp_arr(sn, SN, iccid, ICCID);

	// 데이터 수정 블록
	int error = edit_value(iccid, len);
	
	// 패리티 비트 부여
	for (i = 1; i<cnt; i++){
		iccid[i][ICCID-2] = luhn(iccid[i]);
		iccid[i][ICCID-1] = 'F';
		iccid[i][ICCID] = '\0';
	}
	printf("ITU-T Rec E.118에 의한 parity비트와 padding을 부여했습니다. \n");
	comp_arr(sn, SN, iccid, ICCID);

	return error;

}
	// Luhn Check digit 생성함수
	char luhn(char* data){
	    int sum = 0;
		int check_digit;
		int length = strlen(data);
	
	    for (int j = length - 1; j >= 0; j--) {
	        int num = data[j] - '0';
	        // 오른쪽에서 두번째 자리마다 두 배로 처리
	        if ((length - j) % 2 != 0) {
	            num *= 2;
	            if (num > 9) {
	                num -= 9;
	            }
	        }
	        sum += num;
	    }

		check_digit = 10 - sum % 10;
		if (check_digit == 10){
			check_digit = 0;
		}
	    // 체크 디지트 계산 (10 - (합계 % 10))
	    return check_digit+48;
	}

// 배열을 입력받고 Serial No.를 생성하여 채우는 함수
int mk_sn_info(char** sn, int len){
	int i, j, name=3;
	char input[name];	// 수요처 이름 입력
	time_t t = time(NULL);	// 현재 시간을 time_t 형식으로 반환
	struct tm *local_time = localtime(&t);	// 시간정보가 포함된 tm 구조체 포인터를 반환

	// 수요처 이름 입력받기	
	printf("수요처 기호를 입력하세요.(최대 %d자리) : ", name-1);
	while(getchar() != '\n');
	fgets(input, name, stdin);	// temp에 문자 2자리 입력받고,
	for (j=0; j< (name-1); j++){
		if (input[j] == '\n' || input[j] == '\0'){	// 입력이 \n이나 \0이면
			input[j] = ' ';	// 공백으로 대치
		}
	}
	while(getchar() != '\n');
	input[(name-1)] = '\0'; //마지막 자리에 종료문자 삽입

	// 수요처와 당해년도에 따라 일련번호 생성
	for( int i=1; i<cnt ; i++ ){
		snprintf(sn[i], len+1, "SJ%s%02d%06d", input, (local_time->tm_year % 100), i);	// 길이는 len+1(널포함)
	}

	// 내용확인
	printf("Serial No.를 일괄 생성했습니다.\n");
	show_arr(sn, SN);
	// 생성한 SIM정보 확인
	//show_arr(str, len, cnt);


	// 데이터 수정 블록
	int error = edit_value(sn, len);
	if( error == 1)
		return error;

	printf(" \n");

	return 0;
}

// 전달받은 배열의 값을 수정하는 함수
int edit_value(char **arr, int len){
	int choice, i, j;
	int start, end, size, value;	// 시작번지, 종료번지, 크기, 시작값
	
	do{
		printf("수정하시겠습니까? (0. 아니오, 1. 입력값으로 대치, 2. 일련번호 부여, 3. 난수부여) : ");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("특정 자리를 입력한 값으로 변경합니다. ");
			while (getchar() != '\n');	// scanf 입력의 개행문자 제거
			do{
				printf("변경 시작할 자리를 입력하세요. (0 입력시 변경 취소) : ");	// 시작자리를 start에 입력
				scanf("%d", &start);
			} while ( start < 0 || start > len );	// 입력값은 0이상이며 문자길이보다 작아야 함
	
			if (start != 0){	// 정상적인 주소입력시
				int size, end; // 크기, 종료 인덱스 결정
				size = (end = len) -start + 1;	// 끝자리와 사이즈 지정
				char* temp = (char *)malloc( (size+1)* sizeof(char));	// size만큼 공간 할당
				if (temp == NULL){
					printf("문자열을 입력할 공간 할당에 실패했습니다");
					return 1;
				}
				printf("바꿀 최대 %d자리 문자열을 입력하세요. : " , size );
				while (getchar() != '\n');
				scanf("%s", temp);
				if (strlen(temp) < size){ // 입력값이 size보다 작으면 size를 입력길이로 수정
					size = (end=start + strlen(temp) - 1) - start + 1;	// 시작, 끝 크기 변경
				}
				for (i=1; i<cnt; i++){	// 제목 다음행부터
					for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
						arr[i][j] = temp[j-start+1];	// 지정 값 대치
					}
				}	
				free(temp);	// 문자열 할당 해제
			}
	
			// 내용확인
			printf("수정했습니다.\n");
			show_arr(arr,len);
			break;

		case 2:
			printf("지정한 자리부터 연속값을 부여합니다. ");
			start = get_index(len, &size, &end);
			char* temp = (char *)malloc( (size+1)* sizeof(char));	// size만큼 공간 할당
			if (temp == NULL){
				printf("문자열을 입력할 공간 할당에 실패했습니다");
				return 1;
			}
			if(start != 0){	// 시작번지가 0이 아닐때(정상일때)
				printf("일련번호 첫 값을 입력하세요 : ");
				scanf("%d", &value);	// 시작값을 no에 입력받음
				int pow = 1;	
				for(i=0; i<size; i++){	// 10의 몇승인가?(자릿수)
					pow *= 10;
				}
				value = value % pow;	// no의 버퍼 오버플로우 대비
				for (i=1; i<cnt; i++){	// 제목 다음행부터
					snprintf(temp, size+1, "%0*d", size, value);	// 입력받은 no를 %0d 형식으로 temp에 저장 
					for (j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
						arr[i][j] = temp[j-start+1];	// 해당 자릿값을 temp로 대체
					}
					value++;	// 다음 자리수
					if (value/pow > 0 && i+1 < cnt){	// 자릿수에 따라서
						printf("\n주의 : %d 행에서 Buffer Overflow가 발생했습니다!!!\n", i+1);
					}
				}
				free(temp);	// temp 메모리 공간 해제
			}	

			// 내용확인
			printf("수정했습니다.\n");
			show_arr(arr,len);
			break;
		case 3:
			printf("지정한 자리를 임의값으로 변경합니다.");
			start = get_index(len, &size, &end);
			if(start != 0){
				srand(time(NULL));	// 시드 초기화
				value = sizeof(HEX_SET) -1;	// 널 문자를 제외한 크기
				for (i=1; i< cnt ; i++){
					for (int j=start-1; j < end ; j++){
						arr[i][j] = HEX_SET[rand() % value];	// 랜덤 인덱스 할당
					}
					arr[i][len] = '\0';
				}		
			}

			// 내용확인
			printf("수정했습니다.\n");
			show_arr(arr,len);
			break;
		default:
			break;
		}

	} while ( choice != 0 );	// 입력이 0이 아니면 반복(0일때 종료)

	return 0;
}

	// 입력값에 따라 시작/종료번지와 길이 계산하는 함수
	int get_index(int len, int *size, int *end){
		int start;
		while (getchar() != '\n');	// scanf 입력의 개행문자 제거
		do{
			printf("변경 시작할 자리를 입력하세요. (0 입력시 변경 취소) : ");	// 시작자리를 start에 입력
			scanf("%d", &start);
		} while ( start < 0 || start > len );	// 0보다 작거나, len 이상이면 반복
		if (start != 0){	// start가 1~len이하이면
			do{
				printf("변경할 문자 길이를 입력하세요 : ");	// 일련번호 자릿수를 size에 입력(인덱스+1)
				scanf("%d", size); // size 변수에 저장
			} while ( *size < 1 );	// 입력된 문자길이가 1이하면 반복
			*end = start + *size -1;	// 끝번지 결정
			if ( *end > len){	// 끝자리가 문자길이보다 크면
				*end = len; // 끝번지를 문자길이로 바꾸고
				*size = *end - start + 1;	// 문자열 크기도 수정
			}
		}
		printf("%d번째부터 %d번째까지 %d개 문자를 대치합니다. ", start, *end, *size);

		return start;	// 시작값 반환(size와 end는 주소로 접근하여 직접 수정
}

// 전달받은 길이만큼 string 배열을 생성하고, 초기화하여 반환하는 함수
char** arr_gen(char *name, int len){	// 제목문자열, 길이를 인수로 받아 string 배열 생성
	int i, j;

	// string배열과 string 의 공간 할당
	char **str_array = (char **)malloc( cnt * sizeof(char*));	// string 포인터 cnt개의 배열 공간 할당
	if(str_array == NULL){
		printf("string 배열을 위한 메모리 할당 실패\n");
		return NULL;
	}
	
	for ( i=0; i < cnt ; i++){	// cnt 개의 행에 대해 수행
		str_array[i] = (char *)malloc( (len+1) * sizeof(char));	// 길이+1('\0')개의 string 공간 생성
		if(str_array[i] == NULL) {	// string 할당에 한 번 이라도 실패하면
			printf("string을 위한 메모리 할당 실패\n");
			for(int j = 0; j < i; j++){	// 0~(i-1) 까지, 즉 현재까지 수행횟수만큼
				free(str_array[j]);	// 할당된 string 해제
			}
			free(str_array);	// 할당된 문자열 배열 해제
			return NULL;	// 비정상 종료
		}
	}

	// 배열 초기화
	strcpy(str_array[0], name);	// 전달받은 string로 배열 첫행(str_array[0])을 초기화
	for( j=(int)strlen(str_array[0]) ; j < len; j++){
		str_array[0][j] = '_';	// 전달받은 string 초과 문자는 '_'으로 초기화
	}
	str_array[0][len] = '\0';
	for ( i=1; i < cnt ; i++){	// 두번째 행 부터 갯수까지 수행(불필요)
		memset(str_array[i], ' ', (len) * sizeof(char)); // 갯수만큼 지정문자로 초기화
		str_array[i][len] = '\0';	// 종료문자 부여
	}

	return str_array;
}

// 배열 확인 함수
void show_arr(char **arr, int len){
	char index[50] = "1234567890123456789012345678901234567890";
	printf("\n       ");	// 인덱스 출력 위치 설정
	for (int j = 0; j < len; j++){
		printf("%c", index[j]);
	}
	printf("\n");
	for (int i = 0; i < cnt ; i++){
		printf("%5d: %s\n", i, arr[i]);
	}
}

// 배열 비교 함수
void comp_arr(char **arr1, int len1, char **arr2, int len2 ){
	char index[50] = "1234567890123456789012345678901234567890";

	printf("\n       ");	// 인덱스 출력
		for (int j = 0; j < len1 ; j++){
			printf("%c", index[j]);
		}
	printf("\t");
	for (int j = 0 ; j < len2 ; j++){
		printf("%c", index[j]);
	}
	printf("\n");
	for (int i = 0; i < cnt ; i++){
		printf("%5d: %s\t%s\n", i, arr1[i], arr2[i]);
	}
	printf("\n");
}

