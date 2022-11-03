#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define N 8
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define BYTE_TO_8BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_8BINARY(byte)  \
    ((byte) & 1<<7 ? '1' : '0'), \
    ((byte) & 1<<6 ? '1' : '0'), \
    ((byte) & 1<<5 ? '1' : '0'), \
    ((byte) & 1<<4 ? '1' : '0'), \
    ((byte) & 1<<3 ? '1' : '0'), \
    ((byte) & 1<<2 ? '1' : '0'), \
    ((byte) & 1<<1 ? '1' : '0'), \
    ((byte) & 1<<0 ? '1' : '0')


int input[1<<(N-1)] = { 1, 2, 4, 8, 16, 32, 64, 3, 6, 12, 24, 48, 96, 67, 5, 10, 20, 40, 80, 35, 70, 15, 30, 60, 120, 115, 101, 73, 17, 34,
68, 11, 22, 44, 88, 51, 102, 79, 29, 58, 116, 107, 85, 41, 82, 39, 78, 31, 62, 124, 123, 117, 105, 81, 33, 66, 7, 14,
28, 56, 112, 99, 69, 9, 18, 36, 72, 19, 38, 76, 27, 54, 108, 91, 53, 106, 87, 45, 90, 55, 110, 95, 61, 122, 119, 109,
89, 49, 98, 71, 13, 26, 52, 104, 83, 37, 74, 23, 46, 92, 59, 118, 111, 93, 57, 114, 103, 77, 25, 50, 100, 75, 21, 42,
84, 43, 86, 47, 94, 63, 126, 127, 125, 121, 113, 97, 65, 0};

int output[1<<(N-1)] = {1, 8, 64, 12, 96, 10, 80, 15, 120, 73, 68, 44, 102, 58, 85, 39, 62, 117, 33, 14, 112, 9, 72, 76, 108, 106, 90, 95,
119, 49, 13, 104, 74, 92, 111, 114, 25, 75, 84, 47, 126, 121, 65, 4, 32, 6, 48, 5, 40, 70, 60, 101, 34, 22, 51, 29, 107,
82, 31, 123, 81, 7, 56, 69, 36, 38, 54, 53, 45, 110, 122, 89, 71, 52, 37, 46, 118, 57, 77, 100, 42, 86, 63, 125, 97, 2, 
16, 3, 24, 67, 20, 35, 30, 115, 17, 11, 88, 79, 116, 41, 78, 124, 105, 66, 28, 99, 18, 19, 27, 91, 87, 55, 61, 109, 98, 
26, 83, 23, 59, 93, 103, 50, 21, 43, 94, 127, 113, 0};

//int sbox_DDT[(1<<N)][(1<<N)]; //it stores the DDT of the S-box that we get
int binarylist[N];
int list1[N];
int list2[N];
int truthtable_N_1bit[N-1][(1<<(N-1))];
int truthtable_Nbit[N][(1<<(N))];

int newNbitsbox[1<<N];
time_t t;

int oldN_1bitsbox[1<<(N-1)] = {0,3,3,3,53,99,95,10,85,42,99,31,21,63,74,99,55,91,66,45,95,102,67,121,44,60,108,127,49,116,24,94,87,77,59,34,81,30,84,24,31,121,70,35,108,95,92,108,23,98,13,123,76,108,63,28,17,24,62,52,63,99,121,38,72,90,52,37,112,55,101,33,71,41,14,99,10,49,42,18,111,18,101,27,10,34,105,66,46,47,17,19,62,106,104,63,90,81,73,65,81,15,43,118,72,63,110,26,54,20,121,88,10,110,111,8,92,109,80,98,86,78,6,29,117,56,76,2};


void decimalTobinary(int n,int list[N]){
    int i = 0;
    while (n!=0)
    {
        list[i] = n&1;
        n = n>>1;
        i++;
    }
    for (i;i<N;i++) list[i] = 0;
}


// it returns 0 if the inputsbox is not APN, otherwise returns 1 
int DDTInformation(int inputsbox[1<<N]) {
    int diffuniform = 0;
    // for all input differences a
    for (int a=0;a<(1<<N);a++)
    {
        int sbox_DDT_temp[1<<N] = {0};
        for (int i=0;i<(1<<N);i++) 
        {
            sbox_DDT_temp[inputsbox[i]^inputsbox[i^a]]+=1;
            // if set above 2, return that it cannot be APN anymore
            if (sbox_DDT_temp[inputsbox[i]^inputsbox[i^a]]>diffuniform && a!=0) 
            {
                diffuniform = sbox_DDT_temp[inputsbox[i]^inputsbox[i^a]];
                if (diffuniform>2)
                    return 0;
            }
        }
    }
    return 1;
}

// it returns the first N-1 components Boolean function truth table 
void Initial_N_1bitsbox(int sequence[N-1]){
    // it returns the first N-1 based on output sequence 
    // for (int i=0;i<(1<<(N-1));i++)
    //     oldN_1bitsbox[input[i]] = output[i];
    for (int i=0;i<N-1;i++){
        for (int j=0;j<(1<<(N-1));j++){
            // it returns the N-1 bits truthtable of output sequence 
            truthtable_N_1bit[i][j] = (oldN_1bitsbox[j]>>i)&1;
            // it returns the left half of N bits tt based on ouput sequence
            truthtable_Nbit[i][j] = truthtable_N_1bit[i][j];
        }
    }
    // it returns the fist N-1 right tt of N bit S-box based on seq
    // seq[i] == N-1 means no quadratic is added, seq[i]=m means the i-th component funtion is added to x_m*x_N
    for (int i=0;i<N-1;i++){
        for (int j=(1<<(N-1));j<(1<<(N));j++){
            if (sequence[i]==N-1)
                truthtable_Nbit[i][j] = truthtable_N_1bit[i][j-(1<<(N-1))];
            else
                truthtable_Nbit[i][j] = truthtable_N_1bit[i][j-(1<<(N-1))]^((j>>sequence[i])&1);
        }
    }
}

void Initial_Nbitsbox(int sequence[N-1],int counter){
    int list[N-1];
    for (int i=0;i<N-1;i++) 
        list[i] = i;
    for (int i=0;i<N-1;i++) 
        if (sequence[i]!=N-1)
            list[sequence[i]] = -1;
    // set the last component function f_N + quadratic function (sum_{i not in seq}x_i*x_N) +other quadratic functions
    // we set f_N=0 to test 
    for (int j=0;j<(1<<N);j++)
        truthtable_Nbit[N-1][j] = 0;
    // set quadratic function (sum_{i not in seq}x_i*x_N)
    for (int i=0;i<(1<<(N-1));i++) 
        for (int j=0;j<N-1;j++) 
            if ((list[j] > -1)&&(list[j] != N-1))
                truthtable_Nbit[N-1][(1<<(N-1))+i] ^= ((i>>list[j])&1);
}

int main(){

    int sbox[1<<(N-1)] = {0,3,3,3,53,99,95,10,85,42,99,31,21,63,74,99,55,91,66,45,95,102,67,121,44,60,108,127,49,116,24,94,87,77,59,34,81,30,84,24,31,121,70,35,108,95,92,108,23,98,13,123,76,108,63,28,17,24,62,52,63,99,121,38,72,90,52,37,112,55,101,33,71,41,14,99,10,49,42,18,111,18,101,27,10,34,105,66,46,47,17,19,62,106,104,63,90,81,73,65,81,15,43,118,72,63,110,26,54,20,121,88,10,110,111,8,92,109,80,98,86,78,6,29,117,56,76,2};

    for (int i=1;i<1<<(N-1);i++){
        // printf("%d,", test[i]^test[i^2]);
        int terminate = 0;
        int threadnumber = 0;
        FILE *fp = NULL;
        do
        {
        // sbox = {0,0,0,5,0,4,3,2};
            //gets the random integer sequences without repeatation
            int seq[N-1];
            int flag = 1;
            srand((unsigned) time(&t));
            seq[0] = rand() % (N);
            while (flag!=N-1)
            {
                int temp = rand() % N;
                if (temp!= N-1)
                {
                    int norepeat = 0;
                    for (int i=0;i<flag;i++)
                        if (temp!=seq[i])
                            norepeat++;
                    if (norepeat==flag) 
                        seq[flag] = temp;
                    else flag--;
                }
                else 
                    seq[flag] = temp;
                flag++;
            }
            
            int counter = 0;
            
            //it returns the N-1 tt of sbox with first N-1 component functions
            Initial_N_1bitsbox(seq);
            //it returns the sbox with the last component function
            // note that the 
            Initial_Nbitsbox(seq,counter);

            int nullcounter = 0;
            for (int arg=0;arg<(1<<(N-1));arg++)
            {
                for (int j=0;j<(1<<(N-1));j++)
                {
                    for (int loop=0;loop<N-1;loop++)
                    {
                        if ((arg>>loop)&1)
                            truthtable_Nbit[N-1][(1<<(N-1))+j] ^= ((j>>loop)&1);
                    }
                }

                // gets the sbox from the truthtable_Nbits
                for (int i=0;i<(1<<N);i++)
                {
                    int sum = 0;
                    for (int k=0;k<N;k++)
                        sum += (1<<(k))*truthtable_Nbit[k][i];
                    newNbitsbox[i] = sum;
                }
                //test APNness of sbox 
                if (DDTInformation(newNbitsbox))
                {
                    printf("when arg = %4d, we have the APN function\n",1);
                    
                    fp = fopen("test10bitAPN.txt", "w+");
                    for (int i=0;i<(1<<N);i++)
                        fprintf(fp,"%d,",newNbitsbox[i]);
                    fprintf(fp, "\n This is end for APN...\n");
                    fclose(fp);
                    terminate = 1;
                    break;
                }
                else nullcounter++;
            }
            threadnumber++;
            printf("\e[1;1H\e[2J");
            printf("The threadnumber = %d\n",threadnumber);
        }while(terminate==0);
        for (int i=0;i<(1<<N);i++)
            printf("%d,",newNbitsbox[i]);
        printf("\n");
    }
}


 