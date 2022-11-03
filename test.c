//
//  main.cpp
//  testsbox
//
//  Created by administrator on 2019/6/22.
//  Copyright © 2019 X. Feng. All rights reserved.
//

#include <immintrin.h> // avx+fma
#include <assert.h> // assert
#include <string.h> // memcpy
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

typedef unsigned int word32;

const word32 N = 1 << 18;

void DiffSpectrum(word32* s)
{
    word32 ds[N+1] = {0};
    word32 dt[N];
    word32 ecx = 0;

    time_t start, end;
    double usted;

    start = clock();

    word32 x, d;
    for (d = 1; d < N; d ++) {
        memset(dt, 0, sizeof(word32)*N);
        for (x = 0; x < N; x ++) {
            dt[s[x]^s[x^d]] ++;
        }

        for (x = 0; x < N; x ++) {
            ds[dt[x]] ++;
        }
    }

    end = clock();

    usted = double(end - start) / CLOCKS_PER_SEC;
    printf("used time = %lf\n", usted);

    for (d = 0; d <= N; d ++) {
        if (ds[d] > 0) {
            printf("%u, %u\n", d, ds[d]);
            ecx ++;
        }
    }
    printf("ecx = %u\n", ecx);
}

int ReadSbox(const char* sboxfile, word32* sbox)
{
    int ecx = 0;
    FILE* fp = fopen(sboxfile, "r");
    if (fp == NULL) {
        return 0;
    }

    while (fscanf(fp, "%d\n", &sbox[ecx]) != EOF) {
        ecx ++;
    }

    fclose(fp);

    return ecx;
}

int main(int argc, const char * argv[]) {
    // insert code here...

    word32 s[N];
//
//    word32 x;
//    for (x = 0; x < N; x ++) {
//        s[x] = x;
//    }
    // uint32_t s[1<<N];
    for (int i=0;i<N;i++)    s[i] = rand()%N;
    // int num = ReadSbox("sbox.txt", s);
    // printf("num = %d\n", num);

    DiffSpectrum(s);

    return 0;
}
