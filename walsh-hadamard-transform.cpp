#include <immintrin.h> // avx+fma
#include <assert.h> // assert
#include <string.h> // memcpy
#include <stdio.h>
#include <stdlib.h>

#define NR (8) // Number of rows.
#define MR (75) // Number of columns.
/*
Walsh-Hadamard-Transform, H_8*x where x is a colomun vector [x_0,x_1,...,x_7]^T: 
H_8 = [+1 +1 +1 +1 +1 +1 +1 +1];
      [+1 -1 +1 -1 +1 -1 +1 -1];
      [+1 +1 -1 -1 +1 +1 -1 -1];
      [+1 -1 -1 +1 +1 -1 -1 +1];
      [+1 +1 +1 +1 -1 -1 -1 -1];
      [+1 -1 +1 -1 -1 +1 -1 +1];
      [+1 +1 -1 -1 -1 -1 +1 +1];
      [+1 -1 -1 +1 -1 +1 +1 -1];
H_8 = T_3 * T_2 * T_1, where 

T_1 = [+1  0  0  0 +1  0  0  0];
      [ 0 +1  0  0  0 +1  0  0];
      [ 0  0 +1  0  0  0 +1  0];
      [ 0  0  0 +1  0  0  0 +1];
      [+1  0  0  0 -1  0  0  0];
      [ 0 +1  0  0  0 -1  0  0];
      [ 0  0 +1  0  0  0 -1  0];
      [ 0  0  0 +1  0  0  0 -1];

T_2 = [+1  0 +1  0  0  0  0  0];
      [ 0 +1  0 +1  0  0  0  0];
      [+1  0 -1  0  0  0  0  0];
      [ 0 +1  0 -1  0  0  0  0];
      [ 0  0  0  0 +1  0 +1  0];
      [ 0  0  0  0  0 +1  0 +1];
      [ 0  0  0  0 +1  0 -1  0];
      [ 0  0  0  0  0 +1  0 -1];

T_3 = [+1 +1  0  0  0  0  0  0];
      [+1 -1  0  0  0  0  0  0];
      [ 0  0 +1 +1  0  0  0  0];
      [ 0  0 +1 -1  0  0  0  0];
      [ 0  0  0  0 +1 +1  0  0];
      [ 0  0  0  0 +1 -1  0  0];
      [ 0  0  0  0  0  0 +1 +1];
      [ 0  0  0  0  0  0 +1 -1];
*/

// static const inline __m256d transform1a(__m256d a, __m256d b)
// {
//     //Add two floating-point vectors
//     const __m256d ret = _mm256_add_pd(a, b);

//     return ret;
// }

// static const inline __m256d transform1b(__m256d a, __m256d b)
// {
//     //	Subtract two floating-point vectors
//     const __m256d ret = _mm256_sub_pd(a, b);

//     return ret;
// }

// static const inline __m256d transform2(const __m256d aamm, __m256d a)
// {
//     // [a2, a3, a0, a1]
//     const __m256d a2301 = _mm256_permute2f128_pd(a, a, 0b00000001);

//     // [a0 + a2, a1 + a3, a0 - a2, a1 - a3]
//     const __m256d ret = _mm256_fmadd_pd(a, aamm, a2301);

//     return ret;
// }

// static const inline __m256d transform3(const __m256d amam, __m256d a)
// {
//     // [a1, a0, a3, a2]
//     const __m256d a1032 = _mm256_permute_pd(a, 0b0101);

//     // [a0 + a1, a0 - a1, a2 + a3, a2 - a3]
//     const __m256d ret = _mm256_fmadd_pd(a, amam, a1032);

//     return ret;
// }

// static const inline __m256d wht4x4(const __m256d aamm, const __m256d amam, __m256d a)
// {
//     // [a1, a0, a3, a2]
//     __m256d a1032 = _mm256_permute_pd(a, 0b0101);

//     // [a0 + a1, a0 - a1, a2 + a3, a2 - a3]
//     __m256d a01012323 = _mm256_fmadd_pd(a, amam, a1032);

//     // [a2 + a3, a2 - a3, a0 + a1, a0 - a1]
//     __m256d a23230101 = _mm256_permute2f128_pd(a01012323, a01012323, 0b00000001);

//     // [a0 + a1 + a2 + a3, a0 - a1 + a2 - a3, a0 + a1 - a2 - a3, a0 - a1 - a2 + a3]
//     __m256d ret = _mm256_fmadd_pd(a01012323, aamm, a23230101);

//     return ret;
// }

// static void wht_composed_vectors(double* A, double* C)
// {
//     const __m256d aamm = _mm256_set_pd(-1.0, -1.0, 1.0, 1.0);
//     const __m256d amam = _mm256_set_pd(-1.0, 1.0, -1.0, 1.0);

//     double *a = A;
//     double *c = C;

//     for (int i = 0; i < MR; ++i) {
//         const __m256d al = _mm256_load_pd(a + 0);
//         const __m256d ah = _mm256_load_pd(a + 4);

//         const __m256d cl = wht4x4(aamm, amam, al);
//         const __m256d ch = wht4x4(aamm, amam, ah);

//         _mm256_store_pd(c + 0, _mm256_add_pd(cl, ch));
//         _mm256_store_pd(c + 4, _mm256_sub_pd(cl, ch));

//         a += NR;
//         c += NR;
//     }
// }

// static void wht_composed_novectors(double* A, double* C)
// {
//     double *a = A;
//     double *c = C;

//     // for (int i = 0; i < MR; ++i) {
//         c[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7];
//         c[1] = a[0] - a[1] + a[2] - a[3] + a[4] - a[5] + a[6] - a[7];
//         c[2] = a[0] + a[1] - a[2] - a[3] + a[4] + a[5] - a[6] - a[7];
//         c[3] = a[0] - a[1] - a[2] + a[3] + a[4] - a[5] - a[6] + a[7];
//         c[4] = a[0] + a[1] + a[2] + a[3] - a[4] - a[5] - a[6] - a[7];
//         c[5] = a[0] - a[1] + a[2] - a[3] - a[4] + a[5] - a[6] + a[7];
//         c[6] = a[0] + a[1] - a[2] - a[3] - a[4] - a[5] + a[6] + a[7];
//         c[7] = a[0] - a[1] - a[2] + a[3] - a[4] + a[5] + a[6] - a[7];
//     printf("%lf %lf %lf %lf %lf %lf %lf %lf\n",a[0],a[1],a[2],a[3],a[4]);
//         // a += NR;
//         // c += NR;
//     // }
// }
int main() {

    // double veca[4] = {1, 0, 1, 0};
    // double vecb[4] = {0,1,1,0};
    // int i;
    // // 这个地方实际上多申请的很多空间
    // double *aligned_doublea = (double *)aligned_alloc(32, 8 * 8 * sizeof(double));
    // double *aligned_doubleb = (double *)aligned_alloc(32, 8 * 8 * sizeof(double));
    // aligned_doublea[4] = {1,0,1,0};
    // aligned_doubleb[4] = {0,1,1,0};
    // double *A,*B;
    // A = aligned_doublea;
    // B = aligned_doubleb;
    // // double *B;B = &b[0];
    // wht_composed_vectors(A,B);


  /* Display the elements of the result vector */
//   double* res = (double*)&vecb;
for (int i =1;i<129;i++)
  printf("x%d,",i);
  
  return 0;
}

P<x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21,x22,x23,x24,x25,x26,x27,x28,x29,x30,x31,x32,x33,x34,x35,x36,x37,x38,x39,x40,x41,x42,x43,x44,x45,x46,x47,x48,x49,x50,x51,x52,x53,x54,x55,x56,x57,x58,x59,x60,x61,x62,x63,x64,x65,x66,x67,x68,x69,x70,x71,x72,x73,x74,x75,x76,x77,x78,x79,x80,x81,x82,x83,x84,x85,x86,x87,x88,x89,x90,x91,x92,x93,x94,x95,x96,x97,x98,x99,x100,x101,x102,x103,x104,x105,x106,x107,x108,x109,x110,x111,x112,x113,x114,x115,x116,x117,x118,x119,x120,x121,x122,x123,x124,x125,x126,x127,x128>:=PolynomialRing(GF(2),128);
X:=[x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21,x22,x23,x24,x25,x26,x27,x28,x29,x30,x31,x32,x33,x34,x35,x36,x37,x38,x39,x40,x41,x42,x43,x44,x45,x46,x47,x48,x49,x50,x51,x52,x53,x54,x55,x56,x57,x58,x59,x60,x61,x62,x63,x64,x65,x66,x67,x68,x69,x70,x71,x72,x73,x74,x75,x76,x77,x78,x79,x80,x81,x82,x83,x84,x85,x86,x87,x88,x89,x90,x91,x92,x93,x94,x95,x96,x97,x98,x99,x100,x101,x102,x103,x104,x105,x106,x107,x108,x109,x110,x111,x112,x113,x114,x115,x116,x117,x118,x119,x120,x121,x122,x123,x124,x125,x126,x127,x128];
seq:=StringToIntegerSequence(Read("tmp_state2.txt"));
matrix:=[Matrix(9,128,seq[128*9*i+1..128*9*(i+1)]):i in [0..4002]];

function func_g(x1,x2,x3,x4,x5,x6,x7, x8, x9)
    return x1*x3 + x1*x4 + x1*x9 + x1 + x2*x3 + x2*x4 + x2*x9 + x2 + x3*x4 + x3*x5 + x3*x8 + x3*x9 + x4*x5 + x4*x8 + x5*x9 + x5 + x8*x9 + x8;
end function;

equation_list:=[];
for i in [1..4003] do
    tmp_state := [];
    for j in [1..9] do
        each_bit := 0;
        for k in [1..128] do
            if matrix[i][j][k] eq 1 then
                each_bit +:= X[k];
            end if;
        end for;
        tmp_state[j]:=each_bit;
    end for;
    Include(~equation_list,func<x1,x2,x3,x4,x5,x6,x7, x8, x9|x1*x3 + x1*x4 + x1*x9 + x1 + x2*x3 + x2*x4 + x2*x9 + x2 + x3*x4 + x3*x5 + x3*x8 + x3*x9 + x4*x5 + x4*x8 + x5*x9 + x5 + x8*x9 + x8>(tmp_state[1],tmp_state[2],tmp_state[3],tmp_state[4],tmp_state[5],tmp_state[6],tmp_state[7],tmp_state[8],tmp_state[9]));
end for;
delete matrix;
delete seq;
ideal:=Ideal(equation_list);
delete equation_list;
Groebner(ideal);