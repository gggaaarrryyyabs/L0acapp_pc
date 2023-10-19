#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define N 8




// it returns 0 if the inputsbox is not APN, otherwise returns 1 
int DDTInformation(int inputsbox[1<<N]){
    int diffuniform = 0;
    // for all input differences a
    for (int a=0;a<(1<<N);a++)
    {
        int sbox_DDT_temp[1<<N] = {0};
        for (int i=0;i<(1<<N);i++) 
        {
            sbox_DDT_temp[inputsbox[i]^inputsbox[i^a]]++;
            // if set above 2, return that it cannot be APN anymore
            if (sbox_DDT_temp[inputsbox[i]^inputsbox[i^a]]>diffuniform && a!=0) 
            {
                diffuniform = sbox_DDT_temp[inputsbox[i]^inputsbox[i^a]];
            }
        }
    }
    return diffuniform;
}


int sbox[256] = {0, 32, 64, 128, 28, 221, 85, 116, 28, 212, 189, 149, 28, 53, 180, 125, 53, 242, 140, 171, 117, 83, 197, 3, 231, 200, 191, 112, 187, 117, 234, 196, 85, 107, 242, 44, 123, 164, 213, 234, 213, 3, 147, 165, 231, 208, 168, 127, 149, 76, 203, 242, 231, 223, 176, 104, 219, 234, 100, 181, 181, 101, 3, 51, 143, 9, 31, 121, 226, 133, 123, 252, 36, 74, 85, 219, 85, 218, 45, 66, 227, 130, 138, 11, 210, 82, 178, 210, 134, 15, 14, 103, 171, 195, 42, 162, 14, 150, 121, 1, 81, 40, 47, 182, 57, 73, 175, 63, 122, 235, 229, 148, 151, 232, 25, 134, 148, 10, 19, 109, 110, 249, 1, 118, 113, 7, 23, 129, 47, 217, 9, 31, 149, 130, 186, 77, 226, 252, 37, 219, 68, 187, 138, 149, 55, 38, 232, 25, 209, 33, 7, 23, 52, 205, 10, 19, 206, 214, 249, 1, 227, 11, 34, 42, 107, 98, 163, 74, 178, 178, 146, 114, 38, 199, 15, 14, 14, 1, 54, 217, 218, 52, 235, 229, 145, 118, 72, 79, 89, 95, 137, 111, 79, 31, 185, 9, 132, 53, 123, 42, 53, 141, 34, 122, 226, 187, 252, 69, 14, 185, 1, 86, 153, 207, 159, 41, 186, 229, 84, 235, 49, 143, 214, 136, 87, 25, 70, 232, 174, 1, 182, 249, 177, 23, 65, 7, 84, 19, 173, 10, 227, 74, 11, 66, 70, 14, 167, 15, 203, 138, 194, 99, 114, 210, 114, 50};


int main(){
    int sbox_one[256];
    // int G[128];
    // // dfs(1,0);
    // for(int i=0;i<256;i++) sbox_one[i] = sbox[i];
    // for(int i=0;i<256;i++) 
    //     if (sbox_one[i]>127)
    //         sbox_one[i] -= 128;

    // for (int i=0;i<128;i++)
    //     G[i] = sbox_one[i];

    // int H[128];
    // int H_binary[7][128];

    // for(int i=0;i<128;i++) H[i] = sbox_one[i+128]^sbox_one[i];

    // for (int i=0;i<128;i++)
    //     for (int j =0;j<7;j++)
    //         H_binary[j][i] = (H[i]>>j)&1;
    
    // for (int j=0;j<7;j++){
    //     for (int i=0;i<128;i++)
    //         printf("%d,",H_binary[j][i]);
    //     printf("\n");
    // }

    printf("%d\n",DDTInformation(sbox));
    // for (int i=0;i<128;i++)
    //     printf("%d,",sbox[i]);
}

    // int terminate = 0;
    // int threadnumber = 0;
    // FILE *fp = NULL;
    // for (int perm_index=0;perm_index<5040*8;perm_index++){
    // // for (int perm_index=0;perm_index<5040*8;perm_index++){
    //     // sbox = {0,0,0,5,0,4,3,2};
    //     // printf("%d\n",perm[perm_index]);
    //     int seq[N-1];
    //     seq[6] = perm[perm_index]/1%10-1;   
    //     seq[5] = perm[perm_index]/10%10-1;   
    //     seq[4] = perm[perm_index]/100%10-1;   
    //     seq[3] = perm[perm_index]/1000%10-1;   
    //     seq[2] = perm[perm_index]/10000%10-1;   
    //     seq[1] = perm[perm_index]/100000%10-1;   
    //     seq[0] = perm[perm_index]/1000000%10-1;   

    //     // seq[6] = perm[pperm_index]/100%10-1;   
    //     for (int i=0;i<7;i++) printf("%d,",seq[i]);

        
    //     // // seq[3] = perm[perm_index]/1%10-1;   
    //     // // seq[2] = perm[perm_index]/10%10-1;   
    //     // // seq[1] = perm[perm_index]/100%10-1;   
    //     // // seq[0] = perm[perm_index]/1000%10-1;   

    //     int counter = 0;
    //     // for (int i=0;i<7;i++) printf("%d,",seq[i]);
    //     // printf("\n");
    //     //it returns the N-1 tt of sbox with first N-1 component functions
    //     Initial_N_1bitsbox(seq);
    //     //it returns the sbox with the last component function
    //     // note that the 
    //     Initial_Nbitsbox(seq,counter);

    //     int nullcounter = 0;
    //     for (int arg=0;arg<(1<<(N-1));arg++)
    //     {
    //         for (int j=0;j<(1<<(N-1));j++)
    //         {
    //             for (int loop=0;loop<N-1;loop++)
    //             {
    //                 if ((arg>>loop)&1)
    //                     truthtable_Nbit[N-1][(1<<(N-1))+j] ^= ((j>>loop)&1);
    //             }
    //         }

    //         // gets the sbox from the truthtable_Nbits
    //         for (int i=0;i<(1<<N);i++)
    //         {
    //             int sum = 0;
    //             for (int k=0;k<N;k++)
    //                 sum += (1<<(k))*truthtable_Nbit[k][i];
    //             newNbitsbox[i] = sum;
    //         }
    //         //test APNness of sbox 
    //         if (DDTInformation(newNbitsbox))
    //         {
    //             printf("when arg = %4d, we have the APN function\n",1);
                
    //             fp = fopen("test10bitAPN.txt", "w+");
    //             for (int i=0;i<(1<<N);i++)
    //                 fprintf(fp,"%d,",newNbitsbox[i]);
    //             fprintf(fp, "\n This is end for APN...\n");
    //             fclose(fp);
    //             terminate = 1;
    //             goto tag;
    //         }
    //         else nullcounter++;
    //     }
    //     threadnumber++;
    //     printf("\e[1;1H\e[2J");
    //     printf("The threadnumber = %d!",threadnumber);
    // }
    // tag:
    // if (DDTInformation(newNbitsbox)){
    //     for (int i=0;i<(1<<N);i++)
    //         printf("%d,",newNbitsbox[i]);
    //     printf("\nand the diff_uniformity no greater than 2:yes or not %d",DDTInformation(newNbitsbox));
    // }


 