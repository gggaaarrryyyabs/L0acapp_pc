#include <immintrin.h> // avx+fma
#include <assert.h> // assert
#include <string.h> // memcpy
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define N           5
// #define SEED        0x1234

// 仅仅统计ddt表的一列, 得到一列的最大差分均匀度, 最后比较每列最大的差分均匀度, 来计算该S-box的差分均匀度
uint32_t sbox_DDT[1<<N];
int autocor_table[1<<N][1<<N];
// 统计频率
uint32_t sbox_DDT_freq[1<<N];
uint32_t SIMD_DDT_freq[1<<N];
// 存储S-box
uint32_t sbox[1<<N] ;
int truthtable_small[1<<(N-3)][8];

typedef long clock_t;

//AES的S-box, 差分均匀度是4, 
// uint32_t sbox[] = {99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};
// int sbox[] = {7,6,0,4,2,5,1,3};

/*  新增:
    DDT_SIMD 
    WalshTransform_SIMD
    Gowers Norm */


// 使用最基本的公式计算ddt, N=15, 大概需要 4-5s, N=16大概要16-20s
// 给出ddt所有数值的频率, 最大的数值为 差分均匀度 
void DDTInformation() {
    int diffuniform = 0;
    // for all input differences a
    for (int a=0;a<(1<<N);a++){
        for (int i=0;i<(1<<N);i++){
            sbox_DDT[sbox[i]^sbox[i^a]]+=1;
            // if set above 2, return that it cannot be APN anymore
        }
        for (int x=0;x<1<<N;x++){
            sbox_DDT_freq[sbox_DDT[x]]++;
            sbox_DDT[x] = 0;
        }
    }
    for (int i=0; i<1<<N; i++){
        if (sbox_DDT_freq[i] > 0){
            printf("the element %u occurs %u times\n",i,sbox_DDT_freq[i]);
        }
    }
}

// 使用SIMD计算ddt, 但是效果比不用要差, 这里认为是 SIMD 的装载, 异或, 构造S-box(x+a)造成的, 影响最大的应该是构造S-box(x+a)
void DDT_SIMD(){
    __m256i sbox0_d_new[(1<<N)/8];

    __m256i sbox0[(1<<N)/8];
    __m256i xor_res[(1<<N)/8];
    // the sbox(x+d) where d is the different.
    uint32_t sbox_d_new[1<<N];
    // the point of sbox
    int *sbox_pointers = (int*) &sbox;
    // load sbox into small vectors sbox0 consisting of eight 32 bit ints, repeat 10000times, spend 0.09s
    for (int i=0;i<(1<<N)/8;i++)
        sbox0[i]  = _mm256_loadu_si256((__m256i  *) & sbox_pointers[i*8]);
    
    // x is the differential of input
    uint32_t differential_uniformity = 0;
    for (int x=0;x<1<<N;x++){
        // construct the sbox(x+d)
        // repeat 10000 times, spend 0.6s
        for (int i=0;i<1<<N;i++)
            sbox_d_new[i] = sbox[i^x];

        // construct the pointers of sbox(x+d), repeat 10000 times, spend 0.09s
        int *sbox_d_pointers = (int*) &sbox_d_new;
        for (int j=0;j<(1<<N)/8;j++)
            sbox0_d_new[j]  = _mm256_loadu_si256((__m256i  *) & sbox_d_pointers[j*8 + 0]);
        
        //  return the xor between sbox(x+d) and sbox(x), repeat 10000 times, spend 0.11s
        for (int j=0;j<(1<<N)/8;j++)
            xor_res[j] = _mm256_xor_si256(sbox0_d_new[j],sbox0[j]);
        // for(int s=0;s<10000;s++){
        // construct the pointers of the xor between sbox(x+d) and sbox(x), repeat 10000 times, spend 0.07s
        int *xor_res_point = (int*) &xor_res;
    
        for (int l=0;l<1<<N;l++){
            sbox_DDT[xor_res_point[l]]++;
        } 
        for (int x=0;x<1<<N;x++){
            SIMD_DDT_freq[sbox_DDT[x]]++;
            sbox_DDT[x] = 0;
        }
    }
    // for (int i=0; i<1<<N; i++){
    //     if (SIMD_DDT_freq[i] > 0){
    //         printf("the element %u occurs %u times\n",i,SIMD_DDT_freq[i]);
    //     }
    // }
}

// it returns the gowers norm of the boolean function
void GowersNorm(int truthtable[1<<N]){
    double Gowers_uniformity_norm[3];
    // values of truthtable should be in {-1,1}
    for (int i=0;i<(1<<N);i++)
        Gowers_uniformity_norm[0] += 1-2*truthtable[i];
    // 1th-dimension gowers uniformity norm (U_1 norm)
    Gowers_uniformity_norm[0] = fabs(Gowers_uniformity_norm[0])/(1<<N);

    for (int u_1=0;u_1<(1<<N);u_1++){
        int temp_u2 = 0;
        for (int x=0;x<(1<<N);x++){
            temp_u2 += (1-2*truthtable[x])*(1-2*truthtable[x^u_1]);
        }
        Gowers_uniformity_norm[1] += temp_u2*temp_u2/(1<<N);
    }
    // 2th-dimension gowers uniformity norm (U_2 norm)
    Gowers_uniformity_norm[1] = pow(Gowers_uniformity_norm[1]/(1<<N),0.25);

    for (int x=0;x<(1<<N);x++){
        for (int u1=0;u1<(1<<N);u1++){
            for (int u2=0;u2<(1<<N);u2++){
                for (int u3=0;u3<(1<<N);u3++){
                    Gowers_uniformity_norm[2] += (1-2*truthtable[x])*(1-2*truthtable[x^u1])*(1-2*truthtable[x^u2])*(1-2*truthtable[x^u3])*(1-2*truthtable[x^u1^u2])*(1-2*truthtable[x^u1^u3])*(1-2*truthtable[x^u2^u3])*(1-2*truthtable[x^u1^u2^u3]);
                }
            }
        }
    }
    // 3th-dimension gowers uniformity norm (U_3 norm)
    Gowers_uniformity_norm[2] = pow(Gowers_uniformity_norm[2]/(1<<(4*N)),0.125);
    printf("We give the Gowers uniformity norm, U1=%f,U2=%f,U3=%f",Gowers_uniformity_norm[0],Gowers_uniformity_norm[1],Gowers_uniformity_norm[2]);
}

// it returns the walsh spectrum of the 3-variable Boolean function
void wht3bit_vectors(int *A)
{
    __m256i new_colomun;
    __m256i hadamard_matrix[8];
    hadamard_matrix[0] = _mm256_setr_epi32(1,1,1,1,1,1,1,1);
    hadamard_matrix[1] = _mm256_setr_epi32(1,-1,1,-1,1,-1,1,-1);
    hadamard_matrix[2] = _mm256_setr_epi32(1,1,-1,-1,1,1,-1,-1);
    hadamard_matrix[3] = _mm256_setr_epi32(1,-1,-1,1,1,-1,-1,1);
    hadamard_matrix[4] = _mm256_setr_epi32(1,1,1,1,-1,-1,-1,-1);
    hadamard_matrix[5] = _mm256_setr_epi32(1,-1,1,-1,-1,1,-1,1);
    hadamard_matrix[6] = _mm256_setr_epi32(1,1,-1,-1,-1,-1,1,1);
    hadamard_matrix[7] = _mm256_setr_epi32(1,-1,-1,1,-1,1,1,-1);
    __m256i res = _mm256_setzero_si256();

    for (int i=0;i<8;i++){
        if (A[i] != 0){
            int n = A[i];
            new_colomun = _mm256_mullo_epi32(_mm256_setr_epi32(n,n,n,n,n,n,n,n),hadamard_matrix[i]);
            res = _mm256_add_epi32(res,new_colomun);
        }
    }
    int *p = (int *) &res;
    for (int i=0;i<8;i++)
        A[i] = p[i];
    // return res;
}

// it returns the walsh spectrum of the n-variable Boolean function, where n>=3
// 当 n > 10的时候SIMD的速度要优于非SIMD
// ======================================================================================================
// 注意!!!! Walsh Transform 输入的真值表有两种, 一种是 {0,1} 的, 一种是 {-1,1}的, 前者实际上是对f(x)进行变换, 后者是对(-1)^f(x)
// 进行变换, 哪种都可以(walsh spectrum有对应关系的), 这个代码中的是对 {0,1} 进行变换的
// ======================================================================================================
int *WalshTransform_SIMD(int truthtable[(1<<N)],int walshspectrum[(1<<N)]){
    for (int i=0;i<(1<<N);i++)
    {
        if (truthtable[i] == 0) 
        {
            for (int i=0;i<(1<<N);i++)
                walshspectrum[i] = pow(-1,truthtable[i]);
            break;
        }
        else
            walshspectrum[i] = truthtable[i];
    }

    int *tt_pointer = (int*) &walshspectrum[0];
    if (N > 3){
        __m256i tt_small[1<<(N-3)];
        __m256i tt_temp;
        // int truthtable_small[1<<(N-3)][8];
        for (int i=0;i<(1<<(N-3));i++){
            tt_small[i] = _mm256_loadu_si256((__m256i  *) & tt_pointer[i*8]);
        }
        for (int i=0;i<(N-3);i++){
            int gap = 1<<(N-4-i);
            for(int j=0;j<gap;j++){

               tt_temp = _mm256_add_epi32(tt_small[j],tt_small[j+gap]);
                
               tt_small[j+gap] = _mm256_sub_epi32(tt_small[j],tt_small[j+gap]);  
               
               tt_small[j] = tt_temp;  
            }
        }
        int *tt_small_pointer[1<<(N-3)];

        for (int j=0;j<1<<(N-3);j++){
            tt_small_pointer[j] = (int*) &tt_small[j];
            wht3bit_vectors(tt_small_pointer[j]);
            for (int i=0;i<8;i++){
                walshspectrum[8*j + i] = tt_small_pointer[j][i];
                // printf("%d,",walshspectrum[8*j +i]);
            }
        }
        return walshspectrum;// printf("\n");
    }
    if (N == 3){
        __m256i tt_small = _mm256_loadu_si256((__m256i  *) & tt_pointer[0]);
        int *tt_small_pointer = (int *) &tt_small;
        wht3bit_vectors(tt_small_pointer);
        for (int i=0;i<8;i++){
            walshspectrum[i] = tt_small_pointer[i];
            // printf("%d,",walshspectrum[i]);
        }
        return walshspectrum;
    }
}

int *WalshTransform(int truthtable[(1<<N)],int walshspectrum[(1<<N)]){
    // preserve the elements in truth table are in {-1,1}, 
    for (int i=0;i<(1<<N);i++)
    {
        if (truthtable[i] == 0) 
        {
            for (int i=0;i<(1<<N);i++)
                walshspectrum[i] = pow(-1,truthtable[i]);
            break;
        }
        else
            walshspectrum[i] = truthtable[i];
    }
    // just fft
    for (int i=0;i<N;i++)
    {
        int gap = 1<<(N-i-1);
        for (int j=0;j<(2<<i);j=j+2)
        {
            for (int l=0;l<gap;l++)
            {
                walshspectrum[gap*j+l]     = walshspectrum[gap*j+l] + walshspectrum[gap*(j+1)+l];
                walshspectrum[gap*(j+1)+l] = walshspectrum[gap*j+l] - 2*walshspectrum[gap*(j+1)+l];
            }
        }
    }
    return walshspectrum;
}

void FormatToFile1(int row,int *form,const char *name){
    FILE* pf = fopen(name,"w");
    if(pf == NULL){
        printf("%s\n",strerror(errno));
        return;
    }
    else{
        for(int i=0;i<row;++i)
            if(i != (row - 1)){
                fprintf(pf,"%d,",form[i]);
            }
            else{
                fprintf(pf,"%d\n",form[i]);
            }  
    }
    return;
}


void FormatToFile2(int row,int (*form)[(1<<N)],const char *name){
    FILE* pf = fopen(name,"w");
    if(pf == NULL){
        printf("%s\n",strerror(errno));
        return;
    }
    else{
        for(int i=0;i<row;++i)
            for(int j=0;j<(1<<N);++j){
                if(j != ((1<<N) - 1)){
                    fprintf(pf,"%d,",form[i][j]);
                }
                else{
                    fprintf(pf,"%d\n",form[i][j]);
                }
            }
    }
    return;
}

void FormatOutput(){
    FormatToFile1(1<<N,sbox,"sbox.txt");

    FormatToFile2(1<<N,sbox_DDT_freq,"sbox_ACT.txt");
    return;
}

// int main(){
//     clock_t start1,start2, finish2,finish1;
//     double duration1;
//     double duration2;
//     srand((unsigned)time(NULL));

//     // it returns a random S-box with N bits input and N bits output
//     for (int i=0;i<1<<N;i++)    sbox[i] = rand()%(uint32_t)(1<<N);

//     // DDTInformation();
//     start2 = clock();

//     DDTInformation();
//     finish2 = clock();
//     duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
//     printf( "Calculating DDT of %d bit S-box needs %f seconds\n", N,duration2 );
//     int *a_p,*SIMD_p;
//     // the truthtable of boolean function, {0,1}
//     int tt[1<<N];
//     // the walsh spectrum of Boolean functions, 
//     int res[1<<N];
//     // for (int i=0;i<1<<N;i++) 
//     //     for (int j=0;j<1<<N;j++)
//     //         res[i][j] =0;
//     // walsh spectrum, as known as  Walsh Hadamard Transform, 
//     // https://en.wikipedia.org/wiki/Hadamard_transform


//     // int a[1<<N] = {1,1,-1,-1,1,1,-1,1,1,-1,-1,1,1,-1,-1,-1};
//     // int a[1<<N] = {0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1};
//     for (int i=0;i<1<<N;i++) tt[i] = a[i];
//     // SIMD的walsh变换返回的是数组指针
//     // Autocorrelation(sbox);
//     // printf("The truthtable is\n");

//     // // for(int j=0;j<1<<N;j++) printf("%d,",tt[j]);
//     // printf("\n");
//     // printf("The Walsh spectrum is\n");
//     //     // for(int j=0;j<1<<(2*N);j++) printf("%d,",*(b_p+j));
//     // printf("\n");

//     start2 = clock();
//     // walsh变换
//     for (int i=0;i<1000;i++)
//         a_p = WalshTransform(tt,res);
//     // printf("\n");
//     finish2 = clock();
//     duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
//     printf( "\nCalculating Walsh Spectrum of a %d variable Boolean function without SIMD needs %f seconds\n", N,duration2);
//     printf("Walsh Transform result without SIMD is\n ");
//     for (int i=0;i<1<<N;i++)  printf("%d,",*(a_p+i));

//     start1 = clock();
//     for (int i=0;i<1000;i++)
//         SIMD_p = WalshTransform_SIMD(tt,res);
//     // printf("\n");
//     finish1 = clock();
//     duration2 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
//     printf( "\nCalculating Walsh Spectrum of a %d variable Boolean function with SIMD needs %f seconds\n", N,duration2);

//     printf("Walsh Transform result with SIMD is\n ");
//     for (int i=0;i<1<<N;i++)  printf("%d,",*(SIMD_p+i));
// }
    
int main(){
    clock_t start1,start2, finish2,finish1;
    double duration1;
    double duration2;
    srand((unsigned)time(NULL));

    // it returns a random S-box with N bits input and N bits output
for (int i=0;i<1<<N;i++)    sbox[i] = rand()%(uint32_t)(1<<N);

    start2 = clock();

    DDTInformation();
    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    printf( "Calculating DDT of %d bit S-box needs %f seconds\n", N,duration2 );
    int *a_p,*SIMD_p;
    // the truthtable of boolean function, {0,1}
    int tt[1<<N];
    // the walsh spectrum of Boolean functions, 
    int res[1<<N];
    // walsh spectrum, as known as  Walsh Hadamard Transform, 
    // https://en.wikipedia.org/wiki/Hadamard_transform
    for (int i=0;i<1<<N;i++) tt[i] = rand()%2;

    // SIMD的walsh变换返回的是数组指针

    // printf("The truthtable is\n");

    start2 = clock();
    // walsh变换
    for (int i=0;i<1000;i++)
        a_p = WalshTransform(tt,res);
    // printf("\n");
    finish2 = clock();
    duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    printf( "\nCalculating Walsh Spectrum of a %d variable Boolean function without SIMD needs %f seconds\n", N,duration2);
    printf("Walsh Transform result without SIMD is\n ");
    for (int i=0;i<1<<N;i++)  printf("%d,",*(a_p+i));

    start1 = clock();
    for (int i=0;i<1000;i++)
        SIMD_p = WalshTransform_SIMD(tt,res);
    // printf("\n");
    finish1 = clock();
    duration2 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    printf( "\nCalculating Walsh Spectrum of a %d variable Boolean function with SIMD needs %f seconds\n", N,duration2);

    printf("Walsh Transform result with SIMD is\n ");
    for (int i=0;i<1<<N;i++)  printf("%d,",*(SIMD_p+i));
}
