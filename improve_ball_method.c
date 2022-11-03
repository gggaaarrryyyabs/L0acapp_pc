#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
// #include "gurobi_c.h"

#define N 8
#define n 16
#define r 2
#define M 1610
#define M_i 50
#define M_prime 800
#define SWAP(x, y){ \
    int tmp = x;\
    x = y;\
    y = tmp;}

#define ROTL8(x,shift) ((int) ((x) << (shift)) | ((x) >> (8 - (shift))))


int whether_x_in_arr(int x,int arr[],int cardinality);
int remove_dup_element_in_arr(int arr[],int size);
void initialize_aes_sbox(int sbox[256]);
void ddt_point(int sbox[256],int set_point[]);
void complement(int originalset[],int S_cardinality,int complement_set[]);
void construct_byr(int Bx0r2[],int cardinality_Bx0r2,int Scomplement[],int Byr_set[],int y);
void construct_pyr(int byr[],int byr_size,int byr_complement[],int pyr[]);


int main(int argc,char *argv[]){
    int aes_sbox[1<<N] = {99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22}; 
    int S[32386];
    int aes_ddt_point[] = {0};
    int S_complement[33150];
    int B_x0r2[] = {0};
    // construct ddt_point of S-box
    int S_cardinality = 32386;
    int S_complement_cardinality = 32386;
    ddt_point(aes_sbox,S);
    // construct S_complement 
    complement(S,S_cardinality,S_complement);
    // construct B_x0r2
    B_x0r2[0] = 0;
    int B_x0r2_index = 1;
    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){
            B_x0r2[B_x0r2_index] = (1<<i)+(1<<j);
            B_x0r2_index++;
        }
    }
    for (int i=0;i<n;i++){
        B_x0r2[B_x0r2_index] = (1<<i);
        B_x0r2_index++;
    }
    // printf("%d,",B_x0r2_index);
    // for(int i=0;i<B_x0r2_index;i++)
    //     printf("%d,",B_x0r2[i]);
    // printf("\n");
    // for 


    for (int i=0;i<S_complement_cardinality;i++){
        int Byr[137] = {0};
        int Pyr[1<<n] = {0};
        construct_byr(B_x0r2,B_x0r2_index,S_complement,Byr,S_complement[i]);
        int Byr_cardinality = 0;
        for (int j=0;j<137;j++){
            if (Byr[j] == 1){
                Byr[Byr_cardinality] = B_x0r2[j];
                Byr_cardinality++;
            }
        }

        int Byr_complement[(1<<n)-137] = {0};
        complement(Byr,Byr_cardinality,Byr_complement);
        construct_pyr(Byr,Byr_cardinality,Byr_complement,Pyr);
        int Pyr_cardinality = 0;
        for (int j=0;j<1<<n;j++){
            if (Pyr[j] == 1){
                Pyr[Pyr_cardinality] = j;
                Pyr_cardinality++;
            }
        }

        GRBenv *env = NULL;
        GRBmodel *model = NULL;
        int error = 0;
        double sol[3];
        int ind[3];
        double val[3];
        double obj[3];
        char vtype[3];
        int optimstatus;
        double objval;
        int zero = 0;

        error = GRBloadenv(&env, "mip1.log");
        if (error || env == NULL) {
        fprintf(stderr, "Error: could not create environment\n");
        exit(1);

        error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
        if (error) goto QUIT;

        /* Add variables */
        obj[0] = 1; obj[1] = 1; obj[2] = 2;
        vtype[0] = GRB_BINARY; vtype[1] = GRB_BINARY; vtype[2] = GRB_BINARY;
        error = GRBaddvars(model, 3, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype,
        NULL);
        if (error) goto QUIT;
        /* Change objective sense to maximization */
        error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
        if (error) goto QUIT;

        /* First constraint: x + 2 y + 3 z <= 4 */
        ind[0] = 0; ind[1] = 1; ind[2] = 2;
        val[0] = 1; val[1] = 2; val[2] = 3;
        error = GRBaddconstr(model, 3, ind, val, GRB_LESS_EQUAL, 4.0, NULL);
        if (error) goto QUIT;

        /* Second constraint: x + y >= 1 */
        ind[0] = 0; ind[1] = 1;
        val[0] = 1; val[1] = 1;
        error = GRBaddconstr(model, 2, ind, val, GRB_GREATER_EQUAL, 1.0, NULL);
        if (error) goto QUIT;

        /* Optimize model */
        error = GRBoptimize(model);
        if (error) goto QUIT;
        /* Write model to ’mip1.lp’ */
        error = GRBwrite(model, "mip1.lp");
        if (error) goto QUIT;
        /* Capture solution information */
        error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
        if (error) goto QUIT;
        error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
        if (error) goto QUIT;

        error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, 3, sol);
        if (error) goto QUIT;
        printf("\nOptimization complete\n");
        if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        printf("
        x=%.0f, y=%.0f, z=%.0f\n", sol[0], sol[1], sol[2]);
        } else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n");
        } else {
        printf("Optimization was stopped early\n");
        }

        QUIT:
            /* Error reporting */
        if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        exit(1);
        }
        /* Free model */
        GRBfreemodel(model);
        /* Free environment */
        GRBfreeenv(env);
        return 0;
    }
}

int whether_x_in_arr(int x,int arr[],int cardinality){
    for (int i=0;i<cardinality;i++)
    {
        if (x == arr[i]){
            return 1;
            break;
        }
        if (i == cardinality - 1) 
            return 0;
    }
}

int remove_dup_element_in_arr(int arr[],int size){
    // printf("%d",sizeof(arr));
    // printf("%d",sizeof(arr[0]));
    for (int i = 0; i < size; i++) {                   
        for (int j = i + 1; j < size; j++) {            
            // If any duplicate found 
            if (arr[i] == arr[j]) {
                // Delete the current duplicate element
                for (int k = j + 1; k < size; k++) {
                    SWAP(arr[k - 1],arr[k]);
                }
                size--; // Decrement size of array(n) after removing duplicate element
                j--; // Decrement the index to test a[j] again in the next iteration
            }
        }
    }
    return size;
}

void ddt_point(int sbox[256],int set_point[]){
    int counter = 1;
    set_point[0] = sbox[0]^sbox[0^0];
    for (int i=0;i<(1<<N);i++){
        for (int x=0;x<(1<<N);x++){
            int new_value = 256*x + sbox[i]^sbox[i^x];
            int flag = 0;
            for (int len=0;len<counter;len++){
                if (new_value == set_point[len])
                    break;
                flag++;
            }
            if (flag == counter){
                set_point[counter] = new_value;
                counter++;
            }
        }
    }
    // remove_dup_element_in_arr(set_point,(1<<N)*(1<<N));
} 

void complement(int originalset[],int S_cardinality,int complement_set[]){
    int complement_index = 0;
    int flag = 0;    
    for (int j=0;j<(1<<n);j++){
        if (whether_x_in_arr(j,originalset,S_cardinality) == 0){
            complement_set[flag] = j;
            flag++;
        } 
    }
}

void construct_byr(int Bx0r2[],int cardinality_Bx0r2,int Scomplement[],int Byr_set[],int y){
    int counter = 0;
    int cardinality_Scomplement = 33150;
    for (int i=0;i<cardinality_Bx0r2;i++){
        for (int j=0;j<cardinality_Scomplement;j++){
            if (Bx0r2[i] == (y^Scomplement[j])){
                Byr_set[i] = 1;
                break;
            }
        }
    }
    // printf("counter = %d,y=%d.",counter,y);
}

void construct_pyr(int byr[],int byr_size,int byr_complement[],int pyr[]){
    int index = 0;
    for (int i=0;i<byr_size;i++){
        int z = byr[i]; 
        for (int num=0;num<n;num++){
            if (whether_x_in_arr(z^(1<<num),byr_complement,(1<<n)-byr_size)){
                pyr[z^(1<<num)] = 1;
            }
        }
    }
    // remove_dup_element_in_arr(pyr,);
}
