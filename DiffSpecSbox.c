#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define N 8

void decimalTobinary(int num,int list[],int len){
    int i = 0;
    while (num!=0){
        list[i] = num&1;
        num = num>>1;
        i++;
    }
    for (i;i<len;i++) list[i] = 0;
}

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

int InnerProduct(int u,int v){
    int sum = 0;
    for (int i=0;i<N;i++)
        //sum += pow(-1,u[i]+v[i]);
        sum += ((u>>i)&1)*((v>>i)&1);
    return sum;
}



void WalshTransform(int truthtable[(1<<N)],int walshspectrum[(1<<N)]){
    // test the elements in truth table are in {0,1} or {-1,1} 
    for (int i=0;i<(1<<N);i++)
    {
        if (truthtable[i] == 0) 
        {
            for (int i=0;i<(1<<N);i++)
                walshspectrum[i] = 1-2*truthtable[i];
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
}

void WalshTransform_sbox(int sbox[1<<N]){
    int truthtable_vF[1<<N];
    for(int v=0;v<(1<<N);v++){
        int Wf[1<<N];
        for(int index=0;index<(1<<N);index++)
            truthtable_vF[index] = InnerProduct(sbox[index],v)&1;
        WalshTransform(truthtable_vF,Wf);
        // for (int i=0;i<(1<<N);i++)
        //     printf("%d,",Wf[i]);

        int WalshSpec[1<<(N-1)];
        int len_WS = 0;
        for (int i=0;i<(1<<N);i++)
        {
            int ne_flag = 0;
            for (int j=0;j<len_WS;j++)
                if (WalshSpec[j] != Wf[i] )
                    ne_flag++;
            if (ne_flag == len_WS)
            {
                WalshSpec[len_WS] = Wf[i];
                len_WS++;
            }
        }
        for (int i=0;i<len_WS;i++)
            if (WalshSpec[i] == 0){
                int list[N];
                decimalTobinary(v,list,N);
                for (int j=0;j<N;j++)
                    printf("%d,",list[j]);
                printf("\n");
            }
            // printf("%d,",WalshSpec[i]);
    }
}

// it return the ortho_derivative of the sbox, note that the sbox must be APN!
void OrthoDerivative(int sbox[1<<N],int od[1<<N]){
    od[0] = 0;
    for (int a=1;a<(1<<N);a++)
    {
        int list[1<<N];
        for (int x=0;x<1<<N;x++)
            list[x] = sbox[x]^sbox[x^a]^sbox[a]^sbox[0];
        for (int x=1;x<1<<N;x++)
        {
            int count = 0;
            for (int i=0;i<1<<N;i++)
            {
                if ((InnerProduct(x,list[i])&1) != 0)
                    break;
                else
                    count++;
            }
            if (count == 1<<N)
            {
                od[a] = x;
                break;
            }
        }
    }
}

void DiffSpec_od(int od[1<<N]){
    int freq_ddt_od[40] = {0};
    for (int a=1;a<1<<N;a++)
    {
        int diff_spec_temp[1<<N] = {0};
        for (int x=0;x<1<<N;x++)
            diff_spec_temp[od[x]^od[x^a]]++;
        for (int i=0;i<1<<N;i++)
            freq_ddt_od[diff_spec_temp[i]+1]++;
    }
    // FILE* myFile = fopen("apn_8bit_diffspec_od.txt", "a");

    // printf("The diff_spec of od of sbox = [");
    // fprintf(myFile,"[");
    for (int i=1;i<40;i=i+2)
        if (freq_ddt_od[i] != 0)
            printf("%d:%d, ",i-1,freq_ddt_od[i]);
    // fprintf(myFile,"]\n");
    // fclose(myFile);
}




/*
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
*/

int main(int argc, char* argv[]){
    // Note that in APN_8bit.txt, we have 4 class of Sbox with the same property, that is i = 12655-12658.
    // sbox_worst_nl is an 8-bit quadratic Sbox of the worst nonlinearity, its Walsh spectrum has the maximal absolute value 2^7!
    // when v=96, W_f(u,v) \in {0,\pm 128}

    int sbox_worst_nl[256] = {0, 152, 136, 227, 56, 67, 33, 169, 65, 32, 83, 193, 124, 254, 255, 142, 217, 13, 242, 213, 248, 207, 66, 134, 181, 152, 4, 218, 145, 95, 177, 140, 149, 9, 0, 111, 189, 194, 185, 53, 191, 218, 176, 38, 146, 20, 12, 121, 56, 232, 14, 45, 9, 58, 174, 110, 63, 22, 147, 73, 11, 193, 54, 15, 189, 7, 154, 211, 116, 45, 194, 104, 200, 139, 117, 197, 4, 164, 40, 123, 27, 237, 159, 154, 203, 222, 222, 56, 67, 76, 93, 161, 150, 122, 25, 6, 196, 122, 254, 179, 29, 64, 182, 24, 218, 157, 122, 206, 6, 162, 55, 96, 22, 228, 143, 142, 214, 199, 222, 60, 37, 46, 38, 222, 224, 8, 114, 105, 142, 116, 102, 111, 56, 33, 65, 171, 55, 52, 69, 181, 132, 100, 103, 116, 151, 33, 220, 153, 56, 109, 226, 68, 3, 76, 210, 110, 169, 5, 233, 182, 228, 26, 17, 28, 66, 95, 38, 200, 54, 49, 89, 173, 149, 113, 107, 124, 137, 59, 223, 158, 54, 103, 241, 83, 118, 61, 186, 2, 204, 100, 145, 202, 106, 178, 45, 6, 45, 22, 251, 51, 231, 198, 58, 232, 165, 103, 233, 216, 12, 152, 232, 143, 82, 37, 39, 163, 172, 193, 210, 76, 247, 121, 24, 101, 236, 48, 182, 153, 187, 132, 112, 188, 10, 47, 202, 28, 88, 158, 9, 60, 254, 110, 7, 100, 176, 195, 216, 88, 53, 92, 86, 204, 126, 244, 140, 245};

    // int sbox[1<<N];
    // for (int i=0;i<1<<N;i++)
    //     sbox[input[i]]=output[i];
    // printf("%d\n", DDTInformation(sbox));
    int tt1[1<<N];
    int tt2[1<<N];
    int tt[1<<N];
    for (int i=0;i<1<<(N-1);i++)
    {
        int binary[N-1];
        decimalTobinary(i,binary,N-1);
        tt1[i] = (binary[0]*binary[4])^(binary[1]*binary[5])^(binary[2]*binary[6]); 
        tt1[i + (1<<(N - 1))] = tt1[i] ^ binary[3] ;
        tt2[i] = (binary[0]*binary[4])^(binary[1]*binary[5])^(binary[2]*binary[6]) ^ binary[3]; 
        tt2[i + (1<<(N - 1))] = 0  ;
        // tt2[i] = (binary[0]*binary[6])^(binary[1]*(binary[4]^binary[6]))^(binary[2]*binary[5]);
        // tt2[i + (1<<(N - 1))] = tt2[i]^binary[1] ;
        tt[i] = tt1[i] ^ tt2[i];
        tt[i+ (1<<(N - 1))] = tt1[i+ (1<<(N - 1))] ^ tt2[i+ (1<<(N - 1))];
    }

    int wf[1<<N];
    WalshTransform(tt2 ,wf);
    for (int i=0;i<1<<N;i++)
        printf("%d,",wf[i]);
    // int sbox[1<<N];
    // for (int i=0;i<1<<N;i++)
    // {
    //     int sum = 0;
    //     for (int j=0;j<N;j++)
    //         sum+=tt[j][i]*(1<<j);
    //     sbox[i] = sum;
    // }
    // int wf[1<<N]; 
    // WalshTransform_sbox(sbox_worst_nl);
 

    // for (int i=0;i<1<<N;i++)
    //     printf("%d,",wf[i]);
    // int wf[1<<N];
    // WalshTransform(tt,wf);
    // int count = 0;
    // for (int i=0;i<1<<N;i++)
        // if (wf[i] != 0)
        // {
            // printf("%d,",wf[i]);
            // count++;
            // int list[N];
            // decimalTobinary(i,list);
            // for (int j=0;j<N;j++)
            //     printf("%d,",list[j]);
            // printf("\n");
        // }
    // printf("%d",count);
    // for (int i=0;i<1<<N;i++)
    //     printf("%d,",apn_od[i]);

    // for (int i=0;i<1<<N;i++)
    //     printf("%d,",sbox[i]);

    // int sbox_truthtable_bin[N][1<<N];
    // for (int i=0;i<N;i++)
    //     for(int j=0;j<1<<N;j++)
    //         sbox_truthtable_bin[i][j] = (sbox_worst_nl[j]>>i)&1;

    // int sbox_affine_componet[256];
    // // set the concatenate of two 7-bit affine functions at Sbox_7 by y_5=y_5+y_6 and exchanging y_5 and y_7
    // for (int i=0;i<256;i++)
    // {
    //     sbox_truthtable_bin[5][i] = sbox_truthtable_bin[5][i]^sbox_truthtable_bin[6][i];
    //     int temp;
    //     temp = sbox_truthtable_bin[7][i];
    //     sbox_truthtable_bin[7][i] = sbox_truthtable_bin[5][i];
    //     sbox_truthtable_bin[5][i] = temp;
    // }
    // // 
    // int Wf[1<<N];
    // WalshTransform(sbox_truthtable_bin[7],Wf);
    // int sbox_component[256];
    // for (int i=0;i<256;i++)
    //     sbox_component[i] = sbox_truthtable_bin[0][i];
    // int coef[1<<N];
    // ANF(sbox_component,coef);
    //     // if (Wf[i]==128||Wf[i]==-128)
    // for(int i=0;i<256;i++)
    //     printf("%d,",Wf[i]);
    //     if(coef[i]==1)
    //     {
    //         printf("%d , and the binary form is ",i);
    //         int binary[8];
    //         decimalTobinary(i,binary);
    //         for (int j=0;j<8;j++)
    //             if (binary[j]==1)
    //                 printf("%d,",j);
    //         printf("\n");
    //     }




    // open the sbox file apn_8bit.txt, this file contains `New_Instances_of_Quadratic_APN_Functions`'s results. 
    // FILE *fpRead=fopen("Sbox_DiffSpec_OD.txt","r");
    //     int sbox[1<<N];
    //     int apn_od[1<<N];
    //     for(int j=0;j<1<<N;j++)
    //         fscanf(fpRead,"%d, ",&sbox[j]);
    //     OrthoDerivative(sbox,apn_od);
    //     DiffSpec_od(apn_od);
    //     for (int i=0;i<1<<N;i++)
    //         printf("%d,",apn_od[i]);
    //     return 0;
    // fclose(fpRead);
    
}


  