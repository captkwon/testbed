#include <stdio.h>
#include <string.h>

// Luhn 알고리즘을 사용해 체크 디지트를 계산하는 함수
int calculate_check_digit(char* iccid) {
    int sum = 0;
    int length = strlen(iccid);
	printf("입력값은 %s이며, %d자리임\n", iccid, length);

    for (int i = length - 1; i >= 0; i--) {
        int num = iccid[i] - '0';

        // 오른쪽에서 두번째 자리마다 두 배로 처리
        if ((length - i) % 2 != 0) {
            num *= 2;
            if (num > 9) {
                num -= 9;
            }
        }

        sum += num;
    }

    // 체크 디지트 계산 (10 - (합계 % 10))
    int check_digit = (10 - (sum % 10));
    return check_digit;
}

int main() {
    // 사용자가 입력한 18자리 ICCID (체크 디지트 없이)
    //char* iccid="898230092200700241";  // 18자리 + 널 문자
    char* iccid="489016015297882";  // 18자리 + 널 문자
    //char* iccid="451842125809300";  // 18자리 + 널 문자
    //char* iccid="949019267903661";  // 18자리 + 널 문자
    //char* iccid="519892000135927";  // 18자리 + 널 문자
    //char* iccid="065002060017152";  // 18자리 + 널 문자
    //printf("18자리 ICCID를 입력하세요 (체크 디지트 없음): ");
    //scanf("%18s", iccid);

    // 체크 디지트를 계산
    int check_digit = calculate_check_digit(iccid);

    // 계산된 체크 디지트를 출력하고, 19자리 완성된 ICCID를 출력
    printf("계산된 체크 디지트: %d\n", check_digit);
    printf("완성된 19자리 ICCID: %s%d\n", iccid, check_digit);

    return 0;
}

