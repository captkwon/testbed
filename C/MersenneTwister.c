#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t mt[624];
static int index = 624 + 1;

void mt_init(uint32_t seed) {
    mt[0] = seed;
    for (int i = 1; i < 624; i++) {
        mt[i] = 1812433253UL * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
    }
}

int mt_rand() {
    if (index >= 624) {
        for (int i = 0; i < 624 - 397; i++) {
            uint32_t y = (mt[i] & 0x80000000) | (mt[i + 1] & 0x7fffffff);
            mt[i] = mt[i + 397] ^ (y >> 1) ^ (y % 2 ? 0x9908b0df : 0);
        }
        for (int i = 624 - 397; i < 624; i++) {
            uint32_t y = (mt[i] & 0x80000000) | (mt[i + 1] & 0x7fffffff);
            mt[i] = mt[(i + 397) % 624] ^ (y >> 1) ^ (y % 2 ? 0x9908b0df : 0);
        }
        index = 0;
    }

    uint32_t y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);
    
    return y & 0x7FFFFFFF;
}

int main() {
    mt_init(1); // 초기화
	srand(1);

    for (int i = 0; i < 30; i++) {
        int random_number = mt_rand(); // 난수 생성
        printf("생성된 mt 난수: %d : %02x ", random_number, random_number%16);
        int rand_number = rand(); // 난수 생성
        printf("  생성된 난수: %d : %02x \n", rand_number, rand_number%16);
    }
    return 0;
}

