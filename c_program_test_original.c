#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define N 7




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


int sbox[256] = {0, 3, 3, 3, 53, 227, 95, 138, 85, 42, 227, 159, 149, 63, 74, 227, 55, 219, 194, 45, 95, 102, 195, 249, 172, 60, 236, 127, 49, 116, 24, 94, 87, 77, 187, 162, 81, 158, 212, 24, 159, 249, 198, 163, 108, 223, 92, 236, 151, 98, 141, 123, 204, 236, 191, 156, 145, 24, 62, 180, 63, 99, 249, 166, 72, 90, 180, 165, 240, 55, 101, 161, 71, 41, 14, 99, 10, 177, 42, 146, 239, 18, 229, 27, 10, 34, 105, 66, 46, 175, 145, 19, 62, 106, 232, 191, 90, 81, 73, 65, 209, 15, 171, 118, 200, 191, 110, 26, 182, 20, 121, 216, 10, 238, 239, 8, 220, 237, 80, 98, 86, 206, 6, 157, 117, 56, 76, 2, 136, 101, 154, 116, 10, 50, 113, 74, 240, 97, 87, 197, 135, 195, 73, 14, 10, 8, 238, 239, 213, 2, 88, 140, 188, 194, 237, 144, 150, 61, 174, 6, 239, 27, 18, 229, 94, 127, 202, 232, 10, 130, 66, 201, 78, 19, 111, 49, 154, 129, 145, 137, 118, 184, 20, 217, 177, 214, 15, 107, 168, 26, 127, 206, 40, 212, 197, 58, 39, 14, 163, 137, 10, 138, 82, 209, 240, 165, 193, 151, 58, 41, 33, 49, 104, 174, 26, 223, 214, 185, 120, 20, 113, 203, 182, 15, 10, 239, 8, 238, 54, 6, 93, 110, 181, 44, 2, 152, 124, 48, 162, 237, 239, 229, 27, 18, 142, 81, 19, 207, 158, 232, 223, 170, 10, 169, 34, 130};


int main(){
    int sbox_one[256];
    int G[128];
    // dfs(1,0);
    for(int i=0;i<256;i++) sbox_one[i] = sbox[i];
    for(int i=0;i<256;i++) 
        if (sbox_one[i]>127)
            sbox_one[i] -= 128;

    for (int i=0;i<128;i++)
        G[i] = sbox_one[i];

    int H[128];
    int H_binary[7][128];

    for(int i=0;i<128;i++) H[i] = sbox_one[i+128]^sbox_one[i];

    for (int i=0;i<128;i++)
        for (int j =0;j<7;j++)
            H_binary[j][i] = (H[i]>>j)&1;
    
    for (int j=0;j<7;j++){
        for (int i=0;i<128;i++)
            printf("%d,",H_binary[j][i]);
        printf("\n");
    }

    printf("%d\n",DDTInformation(G));
    for (int i=0;i<128;i++)
        printf("%d,",G[i]);
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


 