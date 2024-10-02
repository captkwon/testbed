#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "str_alloc.h"
#include "tiny-AES-c/aes.h"
#define HEX_SET "0123456789ABCDEF"

#define SN 12
#define ICCID 20
#define IMSI 15
#define K 32
#define OPc 32
#define IIN "898299"	// Issuer Identification Number : SJ ?
#define PLMN "45040"

char** input_func(int *, int *);
char** initialized(char **str, int, int);
char** fill_sn(char **str, int, int);
void show_arr(char **str, int, int);
int mk_csv(char **str, int, int);
int load_csv();

//============ 함수 재선언
char luhn(char*);
void comp_arr(char**, char**, int, int, int);
int get_addr_legacy(int, int*, int*);
char** chg_value(char**, int, int);
char** mk_sn(char**, int, int);
///////// AES용 함수
void comp_arr(char**, char** , int , int, int cnt);
void hex_to_bytes(const char* hex, uint8_t* bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sscanf(hex + 2 * i, "%2hhx ", &bytes[i]);
    }
	printf("\n");
}

void print_hex(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}
void store_hex(char* output, size_t output_size, const uint8_t* data, size_t length) {
	size_t offset = 0;
	for (size_t i=0; i<length ; i++){
		offset += snprintf(output + offset, output_size - offset, "%02X", data[i]);
		if( offset >= output_size){
			break; 	// 만약 버퍼가 꽉 차면 중단
		}
	}
}
char** milenage(char**, int, int);


char** mk_opc_info(char**, char**, int, int, char*);
char** mk_ki_info(char**,int, int);
char** mk_imsi_info(char**, char**, int, int);
char** mk_iccid_info(char**, char**, int, int);
char** mk_sn_info(char** , int , int);
char** edit_info(char**, int, int);
	char** rep_random_value(char**, int, int);
	char** rep_serial_value(char**, int, int);
	char** rep_const_value(char**, int, int);
		int get_index(int, int*, int*);
char** init_arr(char** , int, int);
char** arr_gen(char**, char*, int, int);
//========================
int main(){
	int i;
	int length, count;
	short action;
	char** numbers;

// Test code

//=========== 코드 효율화

	char **sn, **iccid, **imsi, **ki, **opc;
	count = 5;
	ki = arr_gen(ki, "[  K  ]", K, count);
	ki = mk_ki_info(ki, K, count);
	opc = arr_gen(opc, "[  OPc  ]", OPc, count);
	
    char* op = "01020304050607080910111213140000";
	
	opc = mk_opc_info(ki, opc, OPc, count, op);
	//show_arr(iccid, ICCID, count);
	//show_arr(imsi, IMSI, count);
	show_arr(ki, K, count);
//	show_arr(opc, OPc, count);
	comp_arr(ki, opc, K, OPc, count);

	return 0;
//===============기존 코드=========
}

// ========== 함수 효율화
// 임의 값으로 ki 생성함수
/*char** mk_opc_info(char** ki, char **str, int len, int cnt, char* op_hex){
	// 일련번호 부여함수
	printf("mk_opc_info() 함수 실행i- OPc Generator \n");

	printf("문자열 할당\n");
	//show_arr(str, len, cnt);
	str = milenage(str, len, cnt);
//	for(int i = 0; i < 3 ; i++){
	printf("main에서 전달받은 ki와 str 수정본 비교\n");
//	printf("%s\n", str[0]);



//	printf("OPc : %s\n", opc);
	//	milenage(k2_hex);
	//	printf("OPc : %s\n", opc);
	//	milenage(k3_hex);
	//	printf("OPc : %s\n", opc);
//	}
	return str;
}
*/
/*void hex_to_bytes(const char* hex, uint8_t* bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}

void print_hex(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}
void store_hex(char* output, size_t output_size, const uint8_t* data, size_t length) {
	size_t offset = 0;
	for (size_t i=0; i<length ; i++){
		offset += snprintf(output + offset, output_size - offset, "%02X", data[i]);
		if( offset >= output_size){
			break; 	// 만약 버퍼가 꽉 차면 중단
		}
	}
*/
char** milenage(char** str, int len, int cnt){
	return str;
}
char** mk_opc_info(char** ki, char **str, int len, int cnt, char* op_hex){
	
    uint8_t op_b[len/2];	// uint8_t 형식(내부적으로 바이너리 처리)
    uint8_t k_b[len/2];		// ki 값(배열)
	uint8_t temp_b[len/2];	// 임시
	uint8_t encrypted_op[len/2];  // Allocate memory for the encrypted output
	uint8_t opc[len/2];

	hex_to_bytes(op_hex, op_b, len/2);	// op_hex값을 byte로 변환하여 op[]에 저장
for(int i=1; i<5; i++){
	hex_to_bytes(ki[i], temp_b, len/2);		// ki[i]을 byte로 temp에 저장
	// Initialize AES context (from aes.h)
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, temp_b);	// temp 를 k로 이용 eop 생성
	// Encrypt OP using AES in ECB mode op를 암호화하여 encrypted_op에 저장
	memcpy(encrypted_op, op_b, 16); // Copy OP to encrypted_op for encryption
	AES_ECB_encrypt(&ctx, encrypted_op); // Encrypt in place
	// 프린트
	printf("중간점검 \nk : k[i]: %s\n", ki[i]);
	printf("k_bin     : ");
	for (int j=0; j<len/2; j++){
		printf("%02hhx ", temp_b[j]);
	}
	printf("\n");
	printf("OP        : %s\n", op_hex);
	printf("OP _bin   : ");
	for (int j=0; j<len/2; j++){
		printf("%02hhx ", op_b[j]);
	}
	printf("\n");
	printf("OP_encry  : ");
	for (int j=0; j<len/2; j++){
		printf("%02hhx ", encrypted_op[j]);
	}
	printf("\n");

	// print 중간확인

	// k로 암호화된 encrypted_op 와 op 자체를 XOR 연산하여 OPc 구하기
	for(size_t j = 0 ; j< len/2 ; j++){
		opc[j] = encrypted_op[j] ^ op_b[j];
	}
	printf("OPc_b     : ");
		for (int j=0; j<len/2; j++){
			printf("%02hhx ", opc[j]);
			str[i][j*2] = (opc[j] >> 4) + ((opc[j] >> 4) < 10 ? '0' : 'a' - 10); // 첫자리
			str[i][j * 2 + 1] = (opc[j] & 0x0F) + ((opc[j] & 0x0F) < 10 ? '0' : 'a' - 10); // 뒷자리
		}
		str[i][len]='\0';
		printf("\n");
	printf("OPc_   : %s", str[i]);
	//	free(k_hex);
}
	//마일리지 함수에서 수행
	show_arr(str, len, cnt);

	return (str);
	
}

// 임의 값으로 ki 생성함수
char** mk_ki_info(char **str, int len, int cnt){
	short set_size = sizeof(HEX_SET) -1;	// 널 문자를 제외한 크기
	printf("mk_ki_info() ki를 %d개 생성합니다. \n", cnt);

	srand(time(NULL));	// 시드 초기화
	for (int i=1; i<cnt ; i++){
		for (int j=0; j < len ; j++){
			str[i][j] = HEX_SET[rand() % set_size];	// 랜덤 인덱스 할당
		}
		str[i][len] = '\0';
	}

	// 생성한 정보 확인
	show_arr(str, len, cnt);
	// 데이터 수정 블록
	str = edit_info(str, len, cnt);

// 암호화 테스트용 임시 데이터
	for(int i=1; i< 4 ; i++){
		strcpy(str[1], "300b000101112131415161718192a001");
		strcpy(str[2], "300b000101112131415161718192a002");
		strcpy(str[3], "300b000101112131415161718192a003");
	}
	return str;
	
}
// IMSI 정보 생성
char** mk_imsi_info(char** iccid, char** str, int len, int cnt){
	int i, j;
	printf("mk_imsi_info() ICCID를 기반으로 IMSI를 생성합니다. \n");

	for (i=1; i<cnt; i++){
		strcpy(str[i], PLMN);
		for( j=5 ; j< len ; j++){	// 시작값 45040 할당
			str[i][j] = iccid[i][j+3];
			str[i][IMSI] = '\0';
		}
	}

	// 생성한 IMSI 정보 확인
	comp_arr(iccid, str, ICCID, len, cnt);
	// 데이터 수정 블록
	str = edit_info(str, len, cnt);
	return str;
}
// ICCID 정보 생성
char** mk_iccid_info(char** sn, char** str, int len, int cnt){
	int i, j;
	char temp[3];
	int option[3];	//[0] 코어종류 [1] 현장번호 [2] 슬라이스번호

	// 장비제조사 입력
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("장비제조사를 입력하세요. (1.삼성, 2.NOKIA, 3.HFR) : ");	// 시작자리를 start에 입력
		scanf("%d", &option[0]);
	} while ( option[1] < 0 || option[0] > 3 );	// 범위 밖이면 반복
	// 현장번호 입력
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("몇번째로 구축한 사이트입니까? (0-99) :");	// 시작자리를 start에 입력
		scanf("%d", &option[1]);
	} while ( option[1] < 0 || option[1] > 99 );	// 범위 밖이면 반복
	// 슬라이스번호 입력
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("현재 사이트의 몇번째 네트워크입니까? (0-99) :");	// 시작자리를 start에 입력
		scanf("%d", &option[2]);
	} while ( option[2] < 0 || option[2] > 99 );	// 범위 밖이면 반복

	// 초기 ICCID 입력
	for (i=1; i<cnt; i++){
		for( j=0 ; j< len-2 ; j++){	// parity와 padding digit 제외 
			if(j < 6){
				str[i][j] = IIN[j];	// IIN 할당
			} else if (j < 8){
				snprintf(temp, 3, "%02d", option[0]);
				str[i][j] = temp[j-6];
			} else if (j < 10){
				snprintf(temp, 3, "%02d", option[1]);
				str[i][j] = temp[j-8];
			} else if (j < 12){
				snprintf(temp, 3, "%02d", option[2]);
				str[i][j] = temp[j-10];
			}
			else { 
				str[i][j] = sn[i][j-6];
			}
		}
		str[i][ICCID-1] = '\0';
	}

	printf("ICCID 기본값을 생성합니다.\n");
	// 생성한 ICCID정보 확인
	comp_arr(sn, str, SN, len, cnt);

	// 데이터 수정 블록
	str = edit_info(str, len, cnt);
	
	// 패리티 비트 부여
	for (i = 1; i<cnt; i++){
		str[i][ICCID-2] = luhn(str[i]);
		str[i][ICCID-1] = 'F';
		str[i][ICCID] = '\0';
	}
	show_arr(str, ICCID, cnt);

	return str;
}
// SIM카드 S/N 정보 생성
char** mk_sn_info(char** str, int len, int cnt){
	char temp[3];	// 수요처 이름 입력
	time_t t = time(NULL);	// 현재 시간을 time_t 형식으로 반환
	struct tm *local_time = localtime(&t);	// 시간정보가 포함된 tm 구조체 포인터를 반환
	
	printf("수요처 기호를 입력하세요.(최대 2자리) : ");
	while(getchar() != '\n');
	fgets(temp, 3, stdin);	// temp에 문자 2자리 입력
	for (int j=0; j<len; j++){
		if (temp[j] == '\n')	temp[j] = '\0';		// 입력값이 \n이면 그 자리에서 종료
	}
	if ( (int)strlen(temp) < (3-1) ){	// 2자리 이하가 입력되었을 때 자릿수 맞추기
		for(int j = (int)strlen(temp) ; j < (3-1) ; j++){
			temp[j] = ' ';
		}
		temp[(3-1)] = '\0';
	}
	// 수요처와 해당년도에 따라 일련번호 생성
	for( int i=1; i<cnt ; i++ ){
		snprintf(str[i], len+1, "SJ%s%02d%06d", temp, (local_time->tm_year % 100), i);
	}

	// 생성한 SIM정보 확인
	show_arr(str, len, cnt);
	// 데이터 수정 블록
	str = edit_info(str, len, cnt);
	printf(" \n");
	return str;
}

// 배일 수정 함수
char** edit_info(char** str, int len, int cnt){
	int choice;
	do {
		printf("데이터 수정(0:종료, 1:입력값으로 대치, 2:일련번호 부여, 3. 난수부여): ");
		scanf("%d", &choice);
		switch (choice){	// 입력에 따라 행동 수행
		case 1:
			str = rep_const_value(str, len, cnt);
			break;
		case 2:
			str = rep_serial_value(str, len, cnt);
			break;
		case 3:
			str = rep_random_value(str, len, cnt);
			break;
		default :
			break;
		}
		show_arr(str, len, cnt);
	} while( choice >= 1 && choice <= 3 ) ;	// 정상 입력이면 종료
	return str;
}
// 문자열의 특정 범위를 연속값으로 대치
char** rep_random_value(char**str, int len, int cnt){
	int start, size, end;	// 시작점, 길이, 종료점
	char* temp;	// 대치할 임시문자열을 생성값의 최대 크기로 부여

	printf("rep_random_value() 특정 자리를 임의값으로 변경합니다.\n");
	start = get_index(len, &size, &end);	// start, end, size 결정
	temp = str_alloc(size);	// 자릿수만큼 문자열공간 할당(실제로는 \0포함 size+1)
	if (temp == NULL)	return NULL;

	if(start != 0){
		srand(time(NULL));	// 시드 초기화
		int set_size = sizeof(HEX_SET) -1;	// 널 문자를 제외한 크기
		for (int i=1; i< cnt ; i++){
			for (int j=start-1; j < end ; j++){
				str[i][j] = HEX_SET[rand() % set_size];	// 랜덤 인덱스 할당
			}
		}
	}
	free(temp);
	return str;
}
// 문자열의 특정 범위를 연속값으로 대치
char** rep_serial_value(char**str, int len, int cnt){
	int start, size, end;	// 시작점, 길이, 종료점
	char* temp;	// 대치할 임시문자열을 생성값의 최대 크기로 부여
	int i;
	unsigned int no;
	printf("rep_serial_value() 특정 자리를 연속값으로 변경합니다.\n");
	start = get_index(len, &size, &end);	// start, end, size 결정
	temp = str_alloc(size);	// 자릿수만큼 문자열공간 할당(실제로는 \0포함 size+1)
	if (temp == NULL)	return NULL;

	if(start != 0){
		printf("일련번호 시작값을 입력하세요");
		scanf("%d", &no);	// 큰 수 입력 대비 unsigned로 입력
		int pow = 1;
		for(int i=0; i<size; i++){
			pow *= 10;
		}
		no = no % pow;	// no의 버퍼 오버플로우 대비
		
		for (i=1; i<cnt; i++){	// 제목 다음행부터
			snprintf(temp, size+1, "%0*d", size, no);
			for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
				str[i][j] = temp[j-start+1];
			}
			no++;
			if (no/pow > 0 && i+1 <cnt )	printf("\n\n주의 : %d 행에서 Buffer Overflow가 발생했습니다!!!", i+1);
		}
	}
	free(temp);
	return str;
}

// 문자열의 특정 범위를 고정값으로 대치
char** rep_const_value(char**str, int len, int cnt){
	int start, size, end;	// 시작점, 길이, 종료점
	char* temp;	// 대치할 문자열 임시 저장
	unsigned int no;
	int i, j;
	printf("rep_const_value() 특정 자리의 문자를 변경합니다\n");
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("변경시작할 자리를 입력하세요. (0이면 취소)");	// 시작자리를 start에 입력
		scanf("%d", &start);
	} while ( start < 0 || start > len );	// 문자길이보다 작아야 함
	if (start != 0){
		size = ( end = len ) - start + 1;	// 끝자리와 사이즈 지정
		temp = str_alloc(size);	// 자릿수만큼 문자열공간 할당(실제로는 \0포함 size+1)
		if (temp == NULL)	return NULL;

		printf("바꿀 최대 %d자리 문자열을 입력하세요. \n" , size );
		while (getchar() != '\n');
		scanf("%s", temp);
		if (strlen(temp) < size){ // 입력값이 size보다 작으면 size를 입력길이로 수정
			size = (end=start + strlen(temp) - 1) - start + 1;	// 시작, 끝 크기 변경
		}
		for (i=1; i<cnt; i++){	// 제목 다음행부터
			for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
				str[i][j] = temp[j-start+1];	// 지정 값 대치
			}
		}
	}
	free(temp);
	printf("rep_const_value()를 종료합니다\n");
	return str;
}
// 입력값에대한 시작주소, 종료주소, 길이 인덱스 계산 함수
int get_index(int len, int *size, int *end){
	int start;
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("get_index() 변경시작할 자리를 입력하세요. (0이면 취소)");	// 시작자리를 start에 입력
		scanf("%d", &start);
	} while ( start < 0 || start > len );	// 문자길이보다 작아야 함
	if (start != 0){
		do{
			printf("변경할 문자 길이를 입력하세요");	// 일련번호 자릿수를 size에 입력(인덱스+1)
			scanf("%d", size); 
		} while ( *size < 1 );	// 문자길이가 1이하면 반복
		*end = start + *size -1;
		if ( *end > len){	// 끝자리가 문자길이보다 크면
			*end = len; // 끝자리를 문자길이로 고정하고
			*size = *end - start + 1;	// 수정문자열 크기도 변경
		}
	}
	printf("start = %d, end = %d, size = %d, get_index() 종료\n", start, *end, *size);
	return start;	// 시작값 반환(size와 end는 주소로 접근하여 직접 수정
}
//////////////////////////////////////////

// LUHN check digit 생성 함수
char luhn(char* data){
    int sum = 0;
	int check_digit;
	int length = strlen(data);

    //char* iccid="898230092200700241";  // 18자리 + 널 문자
    //char* iccid="898240000001000018";  // 18자리 + 널 문자
    //char* iccid="898230071700590576";  // 18자리 + 널 문자
    //char* iccid="489016015297882";  // 18자리 + 널 문자
    //char* iccid="451842125809300";  // 18자리 + 널 문자
    //char* iccid="949019267903661";  // 18자리 + 널 문자
    //char* iccid="519892000135927";  // 18자리 + 널 문자
    //char* iccid="065002060017152";  // 18자리 + 널 문자
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

// IMSI 생성함수
	/* ITU-T Recommendation E.212
	3. Definition
		3.1. home network: The network responsible for the subscription identified by the elements within the IMSI
		3.2. international mobile subscription identity (IMSI): The IMSI is a string of decimal digits, up to a maximum length of 15 digits, which identifies a unique subscription. The IMSI consists of three fields: the mobile country code (MCC), the mobile network code (MNC), and the moblle subscription identification number (MSIN).
		3.3. mobile country code (MCC): The MCC is the first field of the IMSI, is three digits in length and idenfifies a country. The Director of TSB may assign more than one MCC to a country. MCCs in the 90x range are non-geographic MCCs (country-agnostic) and are administreted by the Director of TSB.
		3.4. mobile network code (MNC): The MNC is second field of the IMSI, is two or three digits in length and is administered by the respective national NPA. MNCs under MCC ranges 90x are administered by the Director of TSB. The MNC, in combination with the MCC, provides sufficient information to identify the home network.
		3.5. mobile subscription identification number (MSIN): The MSIN is the third field of the IMSI, is up to 10 digits in length and is administered by the relevant MNC assignee to identify individual subscriptions.
		* TSB Telecommunicaion Standardization Bureau
	6. IMSI structure, format and assignment procedures
		6.1 Structure and format of the IMSI
		6.2 IMSI assignment procedures
			6.2.3 MSINs are administred by the relevant MNC assignee in accordance with relevant national legal and regulatory environments or ITU-T Recommendation.
			6.2.4 In principle, only one IMSI shuld be assigned to each subscription, although multiple subscriptions may be associated with a SIM/USIM/UICC/embedded SIM card.
	Annex B
		Principles for the assignment of mobile network codes(MNCs) within geographical MCCs
		8) MSINs are to be assigned by the MNC assignee to their subscribed users. A user may have multiple IMSIs.
		MNC - SK05, KT04/08, LG06
		*/

// 2개 비교해서 표출하는 함수
void comp_arr(char** str1, char** str2, int len1 , int len2, int cnt){
	printf("comp_arr() \n");
	char index[50] = "1234567890123456789012345678901234567890";
	printf("\n       ");	// S/N 출력
	for (int j = 0; j < len1 ; j++){
		printf("%c", index[j]);
	}
	printf("\t");	// ICCID 출력
	for (int j = 0 ; j < len2 ; j++){
		printf("%c", index[j]);
	}
	printf("\n");
	for (int i = 0; i < cnt ; i++){
		printf("%5d: %s\t%s\n", i, str1[i], str2[i]);
	}

	printf("comp_arr()을 종료합니다. \n");
}

// ICCID (Integrated Circuit Card Identifier) 생성함수
	/* ITU-T Recommendation E.118
	INN(6) NN(2) AAAA(4) BBBBBB(6) C(1) P(1) 
	- INN : 89(Comm) 82(Country) NN - SK05, KT30 6NL
	4.1. Card numbering structure
		The maximum length of the visible card number (primary account number) shall be 19 characters and is composed of the following subparts :
		- Major Industry Identifier;
			(MII; "89" is assigned for telecommunication purpose(ISO/IEC 7812-1))
		- country code;
			(variable, 1 to 3 digits(ITU-T Rec. E.164))
		- issuer identifier;
			(variable, but fixed number of digits within a country or word zone where appropriate)
			=> Issuer Identification Number(digit variable, maxium 7)
		- individual account identification number;
			(variable, but fixed number of digits for each particular issuer identifier number)
		- parity check digit computed according to the Luhn formula.

	* 즉 INN에서 89 기존할당, 82(대한민국) 00(by company)
		 individual account identificaion number(12자리) 규칙 (기존 SIM 등)
			- NN : Codeshare No.(SA : 11)
			- AAAA : Company S/N
			- BBBBBB : Subscriber S/N
		  C : Check digit by Luhn Algorithm
		  P : padding('F', optional)
		ex. KT LTE : 898230 07 1700 590576 2F
     	 		?? : 898240 00 0001 000018 7F
			MVNO_KT: 898230 09 2200 700241 6F
	*/
// 에러없이 끝나는 자릿수 확인

int get_addr_legacy(int len, int *size, int *end){
	int start;
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	do{
		printf("get_addr_legacy() 변경값의 시작 위치를 입력하세요");	// 시작자리를 start에 입력
		scanf("%d", &start);
	} while ( start < 0 || start > len );
	if (start == 0){
		printf("%d가 입력되어 종료합니다", start);
	} else{
		do{
			printf("문자 길이를 입력하세요");	// 일련번호 자릿수를 size에 입력(인덱스+1)
			scanf("%d", size); 
		} while ( *size < 1 );
		*end = start + *size -1;
		if ( *end > len){	// 끝나는 자리가 length보다 크면
			*end = len; // 끝자리를 length로 고정
			*size = *end - start +1;	// 따라서 사이즈도 수정
		}
	}
	printf("get_addr_legacy() 종료\n");
	return start;
}

// 고정값으로 대치
char** chg_value(char**str, int len, int cnt){
	char* temp;
	int start, size, end;
	unsigned int no;
	int i, j;
	printf("chg_value() 특정 자리의 문자를 변경합니다\n");
	start = get_addr_legacy(len, &size, &end);	// start, end, size 확인
	if(start != 0){
		temp = str_alloc(size);	// 자릿수만큼 문자열공간 할당(실제로는 \0포함 size+1)
		if (temp == NULL)	return NULL;
		printf("선택한 자리를 특정 문자열로 대치합니다\n");
		printf("문자열을 입력하세요");
		while (getchar() != '\n');
		scanf("%s", temp);
		if (strlen(temp) < size){ // temp가 size보다 작으면
			for(i=0, j=strlen(temp); j < size ; i++, j++){
				temp[j] = temp[i];
			}
			temp[size] = '\0';
		}
		for (i=1; i<cnt; i++){	// 제목열 제거
			for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
				str[i][j] = temp[j-start+1];
			}
		}
	}
	while (getchar() != '\n');	// scanf 입력의 개행문자 제거
	printf("chg_value()를 종료합니다\n");
	return str;
}

// 일련번호 생성함수
char** fill_sn(char**str, int len, int cnt){
	char* temp;
	int start, size, end;
	unsigned int no;
	int i, j;
	
	while(getchar() != '\n');	// 하나의 문자를 반복 읽으면서 개행문자이면 버퍼를 비우고 종료
	printf("fill_sn() 일련번호를 생성합니다. 시작값을 입력하세요");
	scanf("%d", &no);	// 큰 수 입력 대비 unsigned로 입력
	start = get_addr_legacy(len, &size, &end);	// start, end, size 확인
	if(start != 0) {
		temp = str_alloc(size);	// 자릿수만큼 문자열공간 할당(실제로는 \0포함 size+1)
		printf("선택한 %d 자리부터 일련번호로 대치합니다. 시작값을 입력하세요", start);
		if (temp == NULL)	return NULL;

		for (i=1; i<cnt; i++){	// 제목열 제거
			snprintf(temp, size+1, "%0*d", size, no);
			for (int j=start-1 ; j < end ; j++){	// 인덱스 만들기 위해 -1
				str[i][j] = temp[j-start+1];
			}
			no++;
		}
		free(temp);
	} else {
		printf("%d이 입력되어 fill()을 종료합니다\n", start);
	}
	return str;
}
// 시리얼 번호 세팅하기
	/* 12자리 중 SJ YY NN BBBBBB
		SJ : 공통
		YY : 발행년도
		NN : 네트워크(수요처)
		BBBBBB : 일련번호
	*/


char** arr_gen(char **arr, char *name, int len, int cnt){
	arr = str_array_alloc(len, cnt);
	if (arr == NULL) return NULL;

	// 첫 행 초기화
	strcpy(arr[0], name);
	for(int j = (int)strlen(arr[0]) ; j < len; j++){
		arr[0][j] = '_';
	}
	arr[0][len] = '\0';
	// 나머지행 초기화
	arr = init_arr(arr, len, cnt);
	if (arr == NULL ) return NULL;

	return arr;
}
// 배열 초기화 함수
char** init_arr(char **str, int len, int cnt){

	for (int i=1; i < cnt; i++){	// 첫행은 제목임
		for (int j=0; j < len ; j++){
			str[i][j] = ' ';
		}
		str[i][len] = '\0';
	}
	return str;
}


//=====기존 함수========================


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
	char index[50] = "1234567890123456789012345678901234567890";
	printf("\n       ");
	for (int j = 0; j < length; j++){
		printf("%c", index[j]);
	}
	printf("\n");
	for (int i = 0; i < count ; i++){
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
/* 이 블럭은 엔터치면 에러남 (ts()는 \n도 입력)
			if (ts(temp, size+1, stdin) != NULL ){
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

	/* ts로 문자열 입력받기
		형식 ts(char *str, int size, FILE *stream);
		- stream은 입력을 받을 스트림으로, stdin 을 사용하면 키보드 입력을 처리함
		- size는 입력받을 최대 문자수로, \n이 포함되므로 실제 입력문자는 size -1임. 주로 sizeof(함수와 함께 사용)
		- str은 입력을 저장할 문자 배열임
		- 입력에 \n이 입력되면 그것도 문자열이며, 입력문자가 size-1이 되거나, \n을 만나면 입력을 종료함.
		- 즉, 엔터만 치면 \n\0 을 str에 저장하고, str(실패시 NULL)을 반환 
		har() 이용 입력 버퍼 처리하기
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
