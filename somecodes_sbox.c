#include <immintrin.h> // avx+fma
#include <assert.h> // assert
#include <string.h> // memcpy
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define N 8
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

#define NR (8) // Number of rows.
#define MR (75) // Number of columns.



int aes_sbox[(1<<N)];  //it stores the S-box of AES

//Serpent S0
int sbox[1<<N];

// int sbox[(1<<N)] = {12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2};  //it stores the S-box that we are going to read
int autocor_table[1<<N][1<<N];
//it stores the S-box of the present;

int hammingweight_coef[(1<<10)]; // it stores the hamming weight of the integers from 0 to 2^10-1

int sbox_ANF[N][(1<<N)]; // it stores the Algebraic Normal Forms of the sbox
int sbox_DDT[(1<<N)][(1<<N)]; //it stores the DDT of the S-box that we get
int sbox_DLCT[(1<<N)][(1<<N)]; //it stores the DLCT of the S-box that we get
int sbox_LAT[(1<<N)][(1<<N)]; //it stores the LAT of the S-box that we get
int sbox_BCT[(1<<N)][(1<<N)]; //it stores the BCT of the S-box that we get

int temptruthtable[(1<<N)];
int tempcoeftable[(1<<N)];


//initialize sbox in AES from wiki
void initialize_aes_sbox(int sbox[256]) {
	uint8_t p = 1, q = 1;
	
	do {
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	sbox[0] = 0x63;
}

// calculate the hamming weight of the integer
int hw(int n){
    int sum = 0;
    while (n!=0){
        if (n&1) sum++;
        n = n>>1; 
    }
    return sum;
}

// it returns the binary string of the input number
void decimalTobinary(int n,int list[N]){
    int i = 0;
    while (n!=0){
        list[i] = n&1;
        n = n>>1;
        i++;
    }
    for (i;i<N;i++) list[i] = 0;
}

// it returns the inner product of two vectors u and v with length n
int InnerProduct_new(int u,int v,int n){
    int sum = 0;
    for (int i=0;i<n;i++)
        //sum += pow(-1,u[i]+v[i]);
        sum += ((u>>i)&1)*((v>>i)&1);
    return sum;
}

// it returns the autocorrelation of S-box
void Autocorrelation(int sbox[(1<<N)]){
    for (int a=0;a<1<<N;a++)
        for (int b=0;b<1<<N;b++)
            for (int x=0;x<1<<N;x++)
                autocor_table[a][b] += pow(-1,((InnerProduct_new(b,sbox[x],N))^(InnerProduct_new(b,sbox[x^a],N))));
        // printf("The autocorrelation table is\n");
    // for (int i=0;i<1<<N;i++){
    //     for (int j=0;j<1<<N;j++)
    //         printf("%4d" ,autocor_table[i][j]);
    //     printf("\n");
    // }
    // return autocor_table;
}


// checks if c is covered by x. Returns 1 if c is covered by x, else return 0.
int is_covered(int c, int x) {
  int mask = 1;
  for (int i=0; i<N; i++){
    if ((c&mask) > (x&mask)) return 0;
    mask = (mask << 1);
  }
  return 1;
}

// it returns the inner product of two vectors of length n
int InnerProduct(int u[],int v[],int n){
    int sum = 0;
    for (int i=0;i<n;i++)
        //sum += pow(-1,u[i]+v[i]);
        sum += u[i]*v[i];
    return sum;
}

// building the DDT for S-box. Returns differential uniformity of the S-box
int DDTInformation() {
    int diffuniform = 0;
    // for all input differences a
    for (int a=0;a<(1<<N);a++)
    {
        for (int i=0;i<(1<<N);i++) 
        {
            sbox_DDT[a][sbox[i]^sbox[i^a]]+=1;
            // if set above 2, return that it cannot be APN anymore
            if (sbox_DDT[a][sbox[i]^sbox[i^a]]>diffuniform && a!=0) 
                diffuniform = sbox_DDT[a][sbox[i]^sbox[i^a]];
        }
    }
    return diffuniform;
}


// yes
// it returns the DLCT (Differential-linear Connectivity Table)/ACT (Autocorrelation Table) of the S-box
void DLCTInformation(){
    for (int a=0;a<(1<<N);a++)
    {
        for (int b=0;b<(1<<N);b++)
        {
            int counter = 0;
            for (int x=0;x<1<<N;x++){
                if ((((InnerProduct_new(b,sbox[x],N))-(InnerProduct_new(b,sbox[x^a],N)))&1) == 0){
                    counter++;
                }
            }
            sbox_DLCT[a][b] = counter - (1<<(N-1));
        }
    }
}

// it returns the degree of S-box
int DegreeSbox(int sbox[(1<<N)]) {
    int coef_sum[(1<<N)];
    int temp_degree = 0;
    for (int u=0;u<(1<<N);u++)
    {
        for (int i=0;i<u+1;i++)
            if (is_covered(i,u)) coef_sum[u]^=sbox[i];
    }
    for (int j=0;j<(1<<N);j++)
    {
        if (coef_sum[j]!=0)
        {
            if (temp_degree < hammingweight_coef[j]) temp_degree = hammingweight_coef[j];
        }
    }
    return temp_degree;
}

// it returns the Walsh Spectrum of the truthtable of the boolean function 
void WalshTransform(int truthtable[(1<<N)],int walshspectrum[(1<<N)]){
    // test the elements in truth table are in {0,1} or {-1,1} 
    for (int i=0;i<(1<<N);i++)
    {
        if (truthtable[i] == -1) 
        {
            for (int i=0;i<(1<<N);i++)
                walshspectrum[i] = (1-truthtable[i])/2;
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
        // for (int j=0;j<1<<N;j++)
        //     printf("%d,",walshspectrum[j]);
        // printf("\n");
    }
}

//it return the Linear Approximation Table (LAT) of sbox, actually it's the Walsh coefficients of VBF.
void LATInformation(){
    int v_binaryarray[N];
    int F_binaryarray[N];
    for (int v=0;v<(1<<N);v++)
    {
        int vF_truthtable[(1<<N)];
        int WalshSpectrum[(1<<N)];
        decimalTobinary(v,v_binaryarray);
        for (int i=0;i<(1<<N);i++)
        {
            decimalTobinary(sbox[i],F_binaryarray);
            vF_truthtable[i] = InnerProduct(v_binaryarray,F_binaryarray,N);
        }
        WalshTransform(vF_truthtable,WalshSpectrum);
        for (int u=0;u<(1<<N);u++)
            sbox_LAT[u][v] = WalshSpectrum[u];
    }
}

// it returns the BCT (Boomerang Connectivity Table) of the S-box
void BCTInformation(){
    int sbox_inverse[1<<N];
    for (int i=0;i<1<<N;i++) sbox_inverse[sbox[i]] = i; 
    int counter;
    for (int a=0;a<(1<<N);a++){
        for (int b=0;b<(1<<N);b++){
            counter = 0;
            for (int x=0;x<(1<<N);x++)
                if ((sbox_inverse[sbox[x]^b]^sbox_inverse[sbox[x^a]^b]) == a)
                    counter++;
            sbox_BCT[a][b] = counter;
        }
    }
}

// it returns the ANF (Algebraic Normal Form) of the truthtable in {0,1}
void ANF(int truthtable[(1<<N)],int coeftable[(1<<N)]){
    // just NNF
    for (int i=0;i<(1<<N);i++)
        coeftable[i] = truthtable[i];
    for (int i=0;i<N;i++)
    {
        int gap = 1<<(N-i-1);
        for (int j=0;j<(2<<i);j=j+2)
        {
            for (int l=0;l<gap;l++)
            {
                coeftable[gap*j+l]     = coeftable[gap*j+l];
                coeftable[gap*(j+1)+l] = coeftable[gap*j+l] - coeftable[gap*(j+1)+l];
            }
        }
    }
    //ANF = NNF (mod 2)
    for (int i=0;i<(1<<N);i++)
        coeftable[i] &= 1;
}

// it returns the ANF of S-box
void ANFInformation(){
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<(1<<N);j++)
            temptruthtable[j] = (sbox[j]>>i)&1;
        ANF(temptruthtable,tempcoeftable);
        for (int loop=0;loop<(1<<N);loop++)
            sbox_ANF[i][loop] = tempcoeftable[loop];
    }    
}

// print Boolean function from the coefficient table
void printPolynomial(int coeftable[(1<<N)]){
    int index_monomial[] = {0};
    int counter = 0;
    int binaryarray[N] = {0};
    for (int i=0;i<(1<<N);i++)
    {
        if (coeftable[i])
        {
            index_monomial[counter] = i;
            counter++;
        }
    }
    for (int i=0;i<counter;i++)
    {
        decimalTobinary(index_monomial[i],binaryarray);
        for (int j=0;j<N;j++)
        {
            if (binaryarray[j])
                printf("x_%d",j+1);
        }
        printf("\n");
    }
}
//==============================================================================================================================================================================================================================================================================================================================================================================================================================================================================
// new change: gowers norm 
//Hamming Distance Caculation
int CalculateHammingDis(int u,int v){
    int dis = 0;
    while(u != 0 || v != 0){
        if((u&1) != (v&1)){
            dis ++;
        }
        u >>= 1;
        v >>= 1;
    }
    return dis;
}

//Hamming Weight Calculation
int CalculateHanmmingWeight(int u){
    int weight = 0;
    while(u != 0){
        if(u&1) weight ++;
        u >>= 1;
    }
    return weight;
}

int Differential_branch_number(int sbox[(1<<N)]){
    int number = (1<<30);
    for(int u=0;u<(1<<N);u++)
        for(int v=0;v<(1<<N);v++){
            if(u == v) continue;
            int tmp_num = CalculateHanmmingWeight(u^v) + CalculateHanmmingWeight(sbox[u]^sbox[v]);
            if(tmp_num < number) number = tmp_num;
        }
    return number;
}

// it returns the gowers norm of the boolean function
void GowersNorm(int truthtable[1<<N]){
    float Gowers_uniformity_norm[3];
    for (int i=0;i<(1<<N);i++)
        Gowers_uniformity_norm[0] += 1-2*truthtable[i];
    // 1th-dimension gowers uniformity norm (U_1 norm)
    Gowers_uniformity_norm[0] = Gowers_uniformity_norm[0]/(1<<N);

    for (int u_1=0;u_1<(1<<N);u_1++){
        int temp_u2 = 0;
        for (int x=0;x<(1<<N);x++){
            temp_u2 += (1-2*truthtable[x])*(1-2*truthtable[x^u_1]);
        }
        Gowers_uniformity_norm[1] += temp_u2*temp_u2/(1<<N);
    }
    // 2th-dimension gowers uniformity norm (U_2 norm)
    Gowers_uniformity_norm[1] = pow(Gowers_uniformity_norm[1]/(1<<N),0.25);
    if (N >= 8)
        Gowers_uniformity_norm[2] = 0;
    else{
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
        Gowers_uniformity_norm[2] = pow(Gowers_uniformity_norm[2]/(uint64_t)(1<<(4*N)),0.125);
    }
    // U_3 norm calculating is hard since 2^(4*N) is large for N no less than 8
        
    printf("\nThe Gowers  U1, U2, U3 norm are (we can calculate the U3 with N no greater than 7)\n");
    printf("%d,%d,%d\n",Gowers_uniformity_norm[0],Gowers_uniformity_norm[1],Gowers_uniformity_norm[2]);
}

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

int *WalshTransform_SIMD(int truthtable[(1<<N)],int walshspectrum[(1<<N)]){
    for (int i=0;i<(1<<N);i++)
    {
        if (truthtable[i] == -1) 
        {
            for (int i=0;i<(1<<N);i++)
                walshspectrum[i] = (1-truthtable[i])/2;
            break;
        }
        else
            walshspectrum[i] = truthtable[i];
    }

    int *tt_pointer = (int*) &walshspectrum[0];
    if (N > 3){
        __m256i tt_small[1<<(N-3)];
        __m256i tt_temp;
        int truthtable_small[1<<(N-3)][8];
        for (int i=0;i<(1<<(N-3));i++){
            tt_small[i] = _mm256_loadu_si256((__m256i  *) & tt_pointer[i*8]);
        }
        for (int i=0;i<(N-3);i++){
            int gap = 1<<(N-4-i);
            for(int j=0;j<1<<i;j++){

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

// double TransparentOrder(void){
//     double temp_beta;
//     double res = 0;
//     for (int beta=0;beta<1<<N;beta++){
//         double sum_a = 0;
//         for (int a=1;a<1<<N;a++){
//             int sum_j=0;
//             for (int j=1;j<N;j++){
//                 int abs_sum = 0;
//                 for (int i=0;i<N;i++){
//                     int index = (1<<i)^(1<<j);
//                     abs_sum += (1-2*(((beta>>i)&1)^((beta>>j)&1)))*autocor_table[index][a];
//                 }
//                 sum_j += abs(abs_sum);
//             }
//             sum_a += sum_j;
//         }
//         temp_beta = N-sum_a/(double)((1<<(2*N))-(1<<N));
//         res = res > temp_beta ? res : temp_beta;
//         break;
//     }
//     return res;
// }
// ========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================
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

    FormatToFile2(1<<N,sbox_DLCT,"sbox_DLCT.txt");
    FormatToFile2(1<<N,sbox_LAT,"sbox_LAT.txt");
    FormatToFile2(1<<N,sbox_BCT,"sbox_BCT.txt");
    FormatToFile2(1<<N,autocor_table,"sbox_ACT.txt");
    return;
}

int main()
{
    // clock_t start1,start2, finish2,finish1;
    // double duration1;
    // double duration2;
    // int tt[1<<N];
    initialize_aes_sbox(sbox);
    int res[1<<N];
    // for (int i=0;i<1<<N;i++) tt[i] = rand()%2;
    int tt[1<<N];
    for (int i=0;i<(1<<N);i++) 
        tt[i] = rand()%2;
    // for (int i=0;i<(1<<N);i++) 
    //     sbox[i] = rand()%(1<<N);
    // initialize_aes_sbox(aes_sbox);
    printf("The sbox is ");
    for (int i=0;i<1<<N;i++)
        printf("%d,",sbox[i]);
    printf("\nThe truthtable is\n");
    for (int i=0;i<1<<N;i++)
        printf("%d,",tt[i]);
    GowersNorm(tt);
    BCTInformation();
    ANFInformation();
    DLCTInformation();
    DDTInformation();
    LATInformation();
    Autocorrelation(sbox);
    printf("The differential branch number is %d,",Differential_branch_number(sbox));
    FormatOutput();
    printf("\nAll the other results are printed in files!");
}