#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <array>
#include <set>

using namespace std;

char file_walshspec[]  = "14.1.txt";
char file_solution[]   = "14.1sol.txt";
char file_solution1[]  = "14.1sol2.txt";

constexpr double THRESHOLD = 2;
constexpr int N     = 14;
constexpr int times = 0;
// constexpr int LEN = 363009;
// constexpr int LEN = 281177;
// constexpr int LEN = 119757;
// constexpr int LEN = 30756;
// constexpr int LEN = 31955;
// constexpr int LEN = 18713;
// constexpr int LEN = 17609;
// constexpr int LEN = 58991;
constexpr int LEN = 7986;
// constexpr int LEN = 4701;
// constexpr int LEN = 4405;


constexpr int N_CONSTRAINT = 4405;

constexpr int Nrow = 4405;
constexpr int Ncol = ((1<<13)+1);

array<int, LEN> position;
array<int, LEN> value;
array<int, 1 << N> tt, tt1;
array<int, 1 << N> wf, wf1;
int random_constraints[N_CONSTRAINT];
int walsh_mat_original[1<<N][N];

int EstimateTTByPro(array<int, 1 << N> &truthtable, int n_bit, array<int, 1 << N> &part_walshpoint, array<int, 1 << N> &part_walshspec, int len_part, double threshold);
int EstimateTTByPro(array<int, 1 << N> &truthtable, int n_bit, array<int, LEN> &part_walshpoint, array<int, LEN> &part_walshspec, int len_part, double threshold);
int InnerProduct(int u, int v, int n_bit);
int F2MatrixRank(vector<int> &matrix, int len, int n_bit);
int EstimateTT_nTimes(int threshold);
int TestSolution(array<int,1<<N> &truthtable, array<int,LEN> &part_walshpoints, array<int,LEN> &part_walshspectrum);
void FisherYates(int *, int);
void WalshTransform(array<int, 1 << N> &truthtable, array<int, 1 << N> &walshspectrum, int n_bit);
void WalshTransformPart(array<int, 1 << N> &truthtable, array<int, 1 << N> &walshspectrum, int n_bit);
void decimalTobinary(int num, int *list, int n_bits);
void FileRead_part_walshspec(char *fname, array<int, LEN> &part_walshpoint, array<int, LEN> &part_walshspect);
void FileRead_solution(char *fname, array<int, 1 << N> &truthtable);
void Count_TT_0_1(array<int, 1 << N> &truthtable, vector<int> &trutable_0, vector<int> &truthtable_1);
void FindSolBySubspace(vector<int> bit_reverse, array<int, 1 << N> &tt, array<int, 1 << N> &tt1, array<int, 1 << N> &wf);
void Combination_NchooseK(vector<int> &res, int N, int K);

void GenerateWalshOriginal(){
	for (int i = 0; i < 1 << N; i++)
	{
		int list[N];
		decimalTobinary(i,list,N);
		for (int j = 0; j < N; j++)
			walsh_mat_original[i][j] = list[j];
	}
}

int main(){
    FileRead_part_walshspec(file_walshspec,position,value);
    // int d = EstimateTTByPro(tt, N, position, value, LEN, THRESHOLD);
    // cout << "The truthtable has left " << d << " values indetermined" << endl;
    // vector<int> bit_reverse {};
    // FindSolBySubspace(bit_reverse,tt,tt1,wf);
    // vector<int> tt_indetermined;


    // int b = 0b10001011000110;

    // vector<int> sum_inner;
    // for (auto i:tt_indetermined)
    // {
    //     int sum = 0;
    //     for (auto j:position)
    //         if (InnerProduct(i,j,N) & 1)
    //             sum++;
    //     sum_inner.push_back(sum);
    // }
    // sort(sum_inner.begin(),sum_inner.end());
    // for (auto i:sum_inner)
    //     cout<< i<< " ";
    // cout << endl;


    // vector<int> nonzero_position{2923,3541, 6733, 13677, 14026, 15616 };
    // vector<int> zero_position;
    // set_difference(position.begin(),position.end(),nonzero_position.begin(),nonzero_position.end(),inserter(zero_position,zero_position.begin()));
    // cout << F2MatrixRank(zero_position,zero_position.size(),N) << endl;

    FileRead_solution(file_solution,tt);
    // vector<int> bit_reverse_4 {3394,3541,12051,12164,};
    // vector<int> bit_reverse_4_2 {6059,6407,13677,15297,};
    // vector<int> bit_reverse_8 {3394,3541,6059,6407,12051,12164,13677,15297,};
    // vector<int> wf_changed_4;
    // vector<int> wf_changed_4_2;
    // vector<int> wf_changed_8;

    // WalshTransform(tt1,wf1,N);

    // tt = tt1;
    // for (auto i:bit_reverse_4)
    //     tt[i] ^= 1;
    // WalshTransform(tt,wf,N);
    // for (auto i = 0;i < 1<<N; i++)
    //     if (wf[i] != wf1[i])
    //         wf_changed_4.push_back(i);
    
    
    // tt = tt1;
    // for (auto i:bit_reverse_4_2)
    //     tt[i] ^= 1;
    // WalshTransform(tt,wf,N);
    // for (auto i = 0;i < 1<<N; i++)
    //     if (wf[i] != wf1[i])
    //         wf_changed_4_2.push_back(i);
    
    // tt = tt1;
    // for (auto i:bit_reverse_8)
    //     tt[i] ^= 1;
    // WalshTransform(tt,wf,N);
    // for (auto i = 0;i < 1<<N; i++)
    //     if (wf[i] != wf1[i])
    //         wf_changed_8.push_back(i);


    // vector<int> wf_diff,wf_union;
    // set_intersection(wf_changed_8.begin(), wf_changed_8.end(), wf_changed_4_2.begin(), wf_changed_4_2.end(), inserter(wf_diff, wf_diff.begin()));
    // set_union(wf_changed_8.begin(), wf_changed_8.end(), wf_changed_4_2.begin(), wf_changed_4_2.end(), inserter(wf_union, wf_union.begin()));
    // cout << wf_diff.size() << endl;
    // cout << wf_union.size() << endl; 
     
    // int diff = wf_diff[0];
    // for (auto &i : wf_diff)
    //     i ^= diff;

    
    array<int,1<<N> sum_1 {};
    vector<int> tt_0 {}, tt_1 {};
    Count_TT_0_1(tt,tt_0,tt_1);

    int min_sum_1 = 0;
    for (auto i = 1; i < 1 << N; i++)
        for (auto j = 0; j < LEN; j++)
            if (InnerProduct(position[j], i, N) & 1)
                sum_1[i]++;
    auto index = min_element(sum_1.begin() + 1,sum_1.end());

    int x4 = distance(sum_1.begin(),index);
    cout << "the most orthogonal point is " << x4 << endl;

    vector<int> position_left {};
    vector<int> position_0 {};
    for (auto i = 0; i < LEN; i++)
    {
        if (InnerProduct(position[i], x4, N) & 1)
            position_left.push_back(position[i]);
        else
            position_0.push_back(position[i]);
    }
    cout << "position_left complete and those position are\n";
    for (auto i:position_left)
        cout << i << " ";
    cout << endl;
    vector<int> tt_1_xor_x4(tt_1);

    for (auto &i:tt_1_xor_x4)
        i ^= x4;
    sort(tt_1_xor_x4.begin(),tt_1_xor_x4.end());
    cout << "sort complete\n";
    // cout << tt_1_xor_x4.size();
    vector<int> intersect{};
    vector<int> combination{};
    // 1056,2029,3541,4197,6407,7128,8134,9200,12164,12272,13677,15784,
    set_intersection(tt_1_xor_x4.begin(), tt_1_xor_x4.end(), tt_0.begin(), tt_0.end(), inserter(intersect, intersect.begin()));
    cout << "intersection complete and the elements in intersection are\n";
    for (auto i : intersect)
        printf("%d,",i);
    cout << endl << "Right result are below:" << endl; 

    int K = 2;
    for (auto x1 = 1; x1 < 1 << N; ++x1)
        for (auto x5 = x1 + 1; x5 < 1 << N; ++x5)
            for (auto a : position_0)
            {
                if (InnerProduct(a, x1 ^ x5, N) & 1)
                    break;
                if (a == position_0[position_0.size() - 1])
                {
                    Combination_NchooseK(combination, position_left.size() - 1, K);
                    for (auto rth = 0; rth < (combination.size() / K); ++rth)
                        for (auto b : position_left)
                            if ((InnerProduct(b, intersect[combination[rth * K + 0]], N) & 1) + (InnerProduct(b, intersect[combination[rth * K + 1]], N) & 1))
                                if (InnerProduct(b, x1 ^ x5, N) & 1)
                                    for (auto i = 0; i < 1 << N; i++)
                                        if (!tt[i] && !tt[i^x1] && !tt[i^intersect[combination[rth * K + 0]]] && !tt[intersect[combination[rth * K + 1]]^i] && tt[x4 ^ i] && tt[x5 ^ i] && (i < i ^ x1))
                                        {
                                            vector<int> res {i,x1 ^ i,i ^ intersect[combination[rth * K + 0]],i ^ intersect[combination[rth * K + 1]],x4 ^ i,x5 ^ i,i ^ intersect[combination[rth * K + 0]] ^ x4,i ^ intersect[combination[rth * K + 1]] ^ x4};
                                            tt1 = tt;
                                            for (auto x:res)
                                                tt1[x] ^= 1;
                                            if (TestSolution(tt1,position,value))
                                                printf("%d,%d,%d,%d,%d,%d,%d,%d\n",i,x1 ^ i,i ^ intersect[combination[rth * K + 0]],i ^ intersect[combination[rth * K + 1]],x4 ^ i,x5 ^ i,i ^ intersect[combination[rth * K + 0]] ^ x4,i ^ intersect[combination[rth * K + 1]] ^ x4);
                                        }
                }
            }

    // int a[] = {12051,12272,16246,14520,3541,3382,7600,6782};
    // for (auto i:a)
    //     tt1[i] ^= 1;
    // cout << TestSolution(tt1,position,value) << endl;

    // cout << F2MatrixRank(wf_diff, wf_diff.size(), N) << endl;


 
    // FileRead_solution(file_solution1,tt1);

	// int wf_ne[1<<N - 2];
    
	// int count_wf_ne = 0;
	// for (int i = 0 ; i < 1<<N;i++)
	// 	if (wf[i] != wf1[i])
	// 	{
	// 		wf_ne[count_wf_ne] = i;
	// 		count_wf_ne++;
	// 	}



    // for (int i = 0 ;i < 9;i++)
    //     printf("%d,",a[i]);

    // for (auto i = 0 ; i < LEN; i++)
    //     cout << (InnerProduct(position[i],162,N) & 1) << (InnerProduct(position[i],162,N) & 1)<< ",";

    // for (int x1 = 0; x1 < 1 << N; x1++)
    //     if (!tt[x1])
    //     for (int x2 = x1 + 1; x2 < 1 << N ; x2++)
    //         if (!tt[x2])
    //         for (int x3 = 0; x3 < 1 << N ; x3++)
    //             if ( tt[x3])
    //             for (int x4 = x3 + 1; x4 < 1 << N ; x4++)
    //                 if(tt[x4])
    //                 for (auto i = 0; i < LEN; i++)
    //                 {
    //                     if ((InnerProduct(position[i], x1 ^ x2, N) & 1) != (InnerProduct(position[i], x3 ^ x4, N) & 1))
    //                         break;
    //                     if ((InnerProduct(position[i], x1, N) & 1 + InnerProduct(position[i], x2, N) & 1) != (InnerProduct(position[i], x3, N) & 1 + InnerProduct(position[i], x4, N) & 1))
    //                         break;
    //                     if(i == LEN - 1)
    //                         printf("%d,%d,%d,%d\n", x1, x2, x3, x4);
    //                 }
                 



    // struct set_inner_01
    // {
    //     int x_1;
    //     vector<int> x_0;
    // };
    // set_inner_01 x_v[1<<N];

    // auto count_len = 0;
	// for (int x1 = 0; x1 <1<<N; x1++)
    // {
    //     auto flag = 0;
	// 	for (int x0 = 0; x0 <1<<N && x0 != x1; x0++)
	// 		for (auto i = 0 ; i < LEN; i++)
	// 		{
	// 			if ((InnerProduct(position[i],x1,N) & 1) && (!(InnerProduct(position[i],x0,N) & 1)))
    //                 break;
	// 			if (i == (LEN - 1))
    //             {
    // 				x_v[count_len].x_0.push_back(x0);
    //                 flag++;
    //             }
	// 		}
    //     if (flag > 1)
    //     {
    //        count_len++;
    //        x_v[count_len].x_1 = x1;
    //     }
    //     else
    //         x_v[count_len].x_0.clear();
    // }

    /*no result for the space 2^3*/
    /*
    for (auto j =0; j < count_len;j++)
    {
        for (auto i =0; i < 1<<N;i++)
        {
            for (auto k = 0; k < x_v[j].x_0.size(); k++)
            for (auto k1 = k + 1; k1 < x_v[j].x_0.size(); k1++)
            {
                int cor[1<<3];
                for (int n = 0; n < 1<<3;n++)
                {
                    int list[3];
                    decimalTobinary(n,list,3);
                    if (list[0])
                        cor[n] ^= x_v[j].x_0[k];
                    if (list[1])
                        cor[n] ^= x_v[j].x_0[k1];
                    if (list[2])
                        cor[n] ^= x_v[j].x_1;
                    cor[n] ^= i;
                }

                if ((tt[cor[0]] + tt[cor[1]] + tt[cor[2]] + tt[cor[3]] +tt[cor[4]] + tt[cor[5]] + tt[cor[6]] + tt[cor[7]]== 4) && ((cor[2] < cor[3])) && (tt[cor[0]] == tt[cor[1]])&&(tt[cor[1]] == tt[cor[2]]) && (tt[cor[2]] == tt[cor[3]]))
                {
                    tt1 = tt;
                    for (auto n = 0; n < 8;n++)
                        tt1[cor[n]] ^= 1;
                    WalshTransform(tt1,wf,N);
                    for (auto m =0;m<LEN;m++)
                    {
                        if (wf[position[m]] != value[m])
                            break;
                        if (m == LEN - 1)
                        {
                            printf("the right positions are:");
                            for (auto o = 0;o < 8;o++)
                                printf("%d,",cor[o]);
                        }
                    }
                }
            }
        }
    }
    */

    // int x_v[count_x_v];
    // for (int i  = 0; i < count_x_v; i++)
    //     x_v[i] = x2_v[i];
    // qsort(x_v,count_x_v,sizeof(int),cmpfunc);

    // // the number of repeat elements x2
    // int count_repeat_elements = 0;
    // // the array of repeat elements x2
    // int repeat_elements[1<<N];
    // for (int i = 1 ; i < count_x_v - 1; i++)
    // {
    //     if ((x_v[i] == x_v[i-1]) && (x_v[i + 1] != x_v[i]))
    //     {
    //         repeat_elements[count_repeat_elements] = x_v[i];
    //         count_repeat_elements++;
    //     }
    // }
    // // array of x1, corresponding to x2
    // int repeat_x2_to_x1[count_repeat_elements][count_x_v];
    // // the length of x1, corresponding to x2
    // int len_repeat_elements_x1[count_repeat_elements];

    // for (int i = 0; i < count_repeat_elements; i++)
    // {
    //     int count_repeat_x2 = 0;
    //     for (int j = 0; j < count_x_v; j++)
    //     {
    //         if (x2_v[j] == repeat_elements[i])
    //         {
    //             repeat_x2_to_x1[i][count_repeat_x2] = x1_v[j];
    //             count_repeat_x2++;
    //         }
    //     }
    //     len_repeat_elements_x1[i] = count_repeat_x2;
    // }

    // for (int i = 0; i < count_repeat_elements; i++)
    // {
    //     printf("%d:",repeat_elements[i]);
    //     for (int j = 0 ; j < len_repeat_elements_x1[i]; j++)
    //         printf("%d,",repeat_x2_to_x1[i][j]);
    //     printf("\n");
    // }
	// printf("\nlength of x_vector is %d\n",count_x_v);



 
	
// printf("======================================================");
//     int index_notin_pos[(1<<N) - LEN];
//     int index = 0;
//     for (int i = 1; i < LEN; i++)
//         for (int j = position[i - 1] + 1; j < position[i]; j++)
//         {
//             index_notin_pos[index] = j;
//             index++;
//         }
//     printf("=========================================\n");

// 	// F2MatrixRank(base,rank,N);
// 	// for (int i = 0; i < 1<<rank; i++)
//     // {
//     //     int list[N];
//     //     decimalTobinary(base[i],list,N);
//     //     for (int j = 0 ;j < N;j ++)
//     //         printf("%d,",list[j]);
//     //     printf("\n");
//     // }

    

//     int index_subspace[1<<rank] = {0};
//     for (int i = 0; i < 1<<rank; i++)
//     {
//         int list[rank];
//         decimalTobinary(i,list,rank);
//         for (int j = 0 ;j < rank;j ++)
//             index_subspace[i] ^= list[j] * base[j];
//         index_subspace[i] ^= index_notin_pos[0];
//     }
 

// 	int Parsval_res = 0;
// 	for (int i = 0; i < 1<<rank; i++)
// 		Parsval_res += wf[index_subspace[i]] * wf[index_subspace[i]];


//  /*=======================================================================================
// 	milp model_gurobi

// */
   

// 	GRBenv   *env         = NULL;
// 	GRBenv   *menv        = NULL;
// 	GRBmodel *model       = NULL;
// 	int       error       = 0;
// 	int       vars        = 10;
// 	int       constrs     = 2; 
// 	int 	  nSolutions;
// 	double    LowerBound[vars], UpperBound[vars];

// 	char      vtype[vars];



//     GRBVar *x = 0;

//     for (int i = 0; i < vars; i++)
//     {
//         LowerBound[i] = 0;
//         UpperBound[i] = 1;
//         vtype[i] = GRB_BINARY;
//     }

//     try
//     {
//         // Create an environment
//         GRBEnv env = GRBEnv(true);
//         env.set("LogFile", "mip1.log");
//         env.start();

//         // Create an empty model
//         GRBModel model = GRBModel(env);

//         // Create variables
//         x = model.addVars(LowerBound,UpperBound,nullptr,vtype,nullptr,vars);

//         GRBLinExpr obj = 0;
//         model.setObjective(obj,GRB_MAXIMIZE);

//         // Add constraint
//         for (int i = 0; i < constrs; i++)
//         {
//             if ((i % 1000) == 0)
//                 printf("we have add %d constraints\n", i);
//             int list[N];
//             decimalTobinary(position[random_constraints[i]], list, N);
//             for (int j = 0; j < 1 << N; j++)
//             {
//                 int sum = 0;
//                 for (int index = 0; index < N; index++)
//                     sum += (walsh_mat_original[j][index] * list[index]);
//                 walsh_mat_part[j] = 1 - 2 * (sum & 1);
//             }
//             walsh_mat_part[1 << N] = value[random_constraints[i]];

//             int sum_1 = 0;
//             int sum_presolved_walsh = 0;
//             GRBLinExpr lhs = 0;
//             for (int j = 0; j < count_tt_left; j++)
//             {
//                 lhs += 2 * walsh_mat_part[tt_left[j]] * x[j];
//                 sum_1 += walsh_mat_part[tt_left[j]];
//             }
//             for (int j = 0; j < 1 << N; j++)
//                 if (tt[j] != -1)
//                     sum_presolved_walsh += (1 - 2 * tt[j]) * walsh_mat_part[j];
//             double right = sum_1 - walsh_mat_part[1 << N] + sum_presolved_walsh;
//             model.addConstr(lhs, GRB_EQUAL, right, nullptr);
//         }

//         // Optimize model
//         model.optimize();
//         // Status checking
//         auto status = model.get(GRB_IntAttr_Status);

//         if (status == GRB_INF_OR_UNBD ||
//             status == GRB_INFEASIBLE ||
//             status == GRB_UNBOUNDED)
//         {
//             cout << "The model cannot be solved "
//                     << "because it is infeasible or unbounded" << endl;
//             return 1;
//         }
//         if (status != GRB_OPTIMAL)
//         {
//             cout << "Optimization was stopped with status " << status << endl;
//             return 1;
//         }

//         cout << "Selected elements in best solution:" << endl << "\t";
//         for (int i = 0; i < count_tt_left;i++)
//         {
//             cout << x[i].get(GRB_DoubleAttr_X);
//         }
//     }
//     catch (GRBException e) {
//         cout << "Error code = " << e.getErrorCode() << endl;
//         cout << e.getMessage() << endl;
//     }
//     catch (...) {
//         cout << "Exception during optimization" << endl;
//   }
 
	return 0;
}





void FileRead_part_walshspec(char * fname, array<int,LEN> &part_walshpoint, array<int,LEN> &part_walshspectrum){
    FILE *fpRead;
    if ((fpRead = fopen(fname, "r")) == NULL){
        printf("\nERROR, Open File Fails\n");
        exit(1);
    }
    int data1,data2,inc;
    int count = 0;
    while (EOF != (inc = fscanf(fpRead, "%d,%d ", &data1, &data2)) && inc == 2){
        part_walshpoint.at(count) = data1;
        part_walshspectrum.at(count) = data2;
        count++;
    }
    fclose(fpRead); 
}

void FileRead_solution(char * fname, array<int,1<<N> &truthtable){
    FILE *fpRead;
    if ((fpRead = fopen(fname, "r")) == NULL){
        printf("\nERROR, Open File Fails\n");
        exit(1);
    }

    int data1,inc;
    int tt_count = 0;
    while (EOF != (inc = fscanf(fpRead, "%1d", &data1)) && inc == 1){
        truthtable[tt_count] = data1;
        tt_count++;
    }
    fclose(fpRead); 
}

int InnerProduct(int u,int v,int n_bit){
    int sum = 0;
    for (int i = 0;i < n_bit; i++)
        //sum += pow(-1,u[i]+v[i]);
        sum += ((u>>i) & 1) * ((v>>i) & 1);
    return sum;
}
// return the binary form of num in the array list 
void decimalTobinary(int num,int list[],int n_bits){
	int i = 0;
	while (num != 0){
		list[i] = num & 1;
		num = num >> 1;
		i++;
	}
	for (i; i < n_bits; i++) list[i] = 0;
}

int FindRowNozeroLeading(int matrix[Nrow][Ncol],int row){
    int i = row;
    int j = row;
    while (!matrix[i][j])
    {
        i++;
        if (i == Nrow){
            j++;
            i = row;
        }
    }
    return i;
}

void HadamardElimination(int matrix[Nrow][Ncol]){
    for (int i = 0; i < Nrow; i++)
    {
        int position_nonzero = 0;
        int j = FindRowNozeroLeading(matrix, i);
        for (int k = i; k < Ncol; k++)
            if (matrix[j][k]){
                position_nonzero = k;
                break;
            }
        // if the row with leading nonzero coef is not in the first, swap two rows
        if (i != j)
            for (int k = position_nonzero; k < Ncol; k++)
            {
                int temp = matrix[i][k];
                matrix[i][k] = matrix[j][k];
                matrix[j][k] = temp;
            }
        // subtract all the other rows by the row with the leading nonzero coef
        for (int k = i + 1; k < Nrow; k++)
            if (matrix[k][position_nonzero])
            {   
                if (matrix[k][position_nonzero] == -matrix[i][position_nonzero])
				{
					for (int l = position_nonzero + 1; l < Ncol; l++)
						matrix[k][l] += matrix[i][l];
					matrix[k][position_nonzero] = 0;
				}
				else if(matrix[k][position_nonzero] == matrix[i][position_nonzero])
				{
					for (int l = position_nonzero + 1; l < Ncol; l++)
						matrix[k][l] -= matrix[i][l];
					matrix[k][position_nonzero] = 0;
				}
            }
    }
	for (int i = 0; i < Nrow; i++)
		for (int j = 0; j < Ncol; j++)
			if (matrix[i][j])
			{
				int factor = matrix[i][j];
				for (int k = j; k < Ncol; k++)
					matrix[i][k] /= factor;
				break;
			}
}

void HadamrdReducedRowForm(int matrix[Nrow][Ncol]){
	for (int i = 1; i < Nrow; i++)
		for (int j = i; j < Ncol - 1; j++)
			if (matrix[i][j])
			{
				for (int k = i - 1; k > -1; k--)
				{
					if (matrix[k][j])
					{
						for (int col = j + 1; col < Ncol; col++)
							matrix[k][col] -= matrix[k][j] * matrix[i][j] * matrix[i][col];
						matrix[k][j] = 0;
					}
				}
				break;
			}
}

int EstimateTTByPro(array<int, 1 << N> &truthtable, int n_bit, array<int, LEN> &part_walshpoint, array<int, LEN> &part_walshspec, int len_part, double threshold){
    auto walshspec = new int[1 << n_bit]{};
	for ( int i = 0; i < len_part; i++)
		walshspec[part_walshpoint[i]] = part_walshspec[i];
    for (int i = 0; i < n_bit; i++)
    {
        int gap = 1 << (n_bit - i - 1);
        for (int j = 0; j < (2 << i); j = j + 2)
        {
            for (int l = 0; l < gap; l++)
            {
                walshspec[gap * j + l]       = walshspec[gap * j + l] +     walshspec[gap * (j + 1) + l];
                walshspec[gap * (j + 1) + l] = walshspec[gap * j + l] - 2 * walshspec[gap * (j + 1) + l];
            }
        }
    }
 	int count = 0;
    for (int beta = 0; beta < 1 << n_bit; beta++)
    {
        double res = (double) walshspec[beta] / len_part;
        if (res >= threshold)
			truthtable[beta] = 0;
		else if (res <= -1 * threshold)
			truthtable[beta] = 1;
		else 
		{
			truthtable[beta] = -1;
			count++;
		}
    }
    delete(walshspec);
	return count;
}


int EstimateTTByPro(array<int, 1 << N> &truthtable, int n_bit, array<int, 1<<N> &part_walshpoint, array<int, 1<<N> &part_walshspec, int len_part, double threshold){
    auto walshspec = new int[1 << n_bit]{};
	for ( int i = 0; i < len_part; i++)
		walshspec[part_walshpoint[i]] = part_walshspec[i];
    for (int i = 0; i < n_bit; i++)
    {
        int gap = 1 << (n_bit - i - 1);
        for (int j = 0; j < (2 << i); j = j + 2)
        {
            for (int l = 0; l < gap; l++)
            {
                walshspec[gap * j + l]       = walshspec[gap * j + l] +     walshspec[gap * (j + 1) + l];
                walshspec[gap * (j + 1) + l] = walshspec[gap * j + l] - 2 * walshspec[gap * (j + 1) + l];
            }
        }
    }
 	int count = 0;
    for (int beta = 0; beta < 1 << n_bit; beta++)
    {
        double res = (double) walshspec[beta] / len_part;
        if (res >= threshold)
			truthtable[beta] = 0;
		else if (res <= -1 * threshold)
			truthtable[beta] = 1;
		else 
		{
			truthtable[beta] = -1;
			count++;
		}
    }
    delete(walshspec);
	return count;
}


void WalshTransform(array<int, 1 << N> &truthtable, array<int, 1 << N> &walshspectrum, int n_bit){
    // make sure that the elements in truth table are in {-1,1} 
    for (int i = 0; i < 1<<n_bit; i++)
    {
        if (truthtable[i] == 0) 
        {
            for (int i = 0; i < 1<<n_bit; i++)
                walshspectrum[i] = 1 - 2 * truthtable[i];
            break;
        }
        else
            walshspectrum[i] = truthtable[i];
    }
    // just fft
    for (int i = 0; i < n_bit; i++)
    {
        int gap = 1<<(n_bit-i-1);
        for (int j = 0; j < (2<<i); j = j + 2)
        {
            for (int l = 0; l < gap; l++)
            {
                walshspectrum[gap * j + l]       = walshspectrum[gap * j + l] +     walshspectrum[gap * (j + 1) + l];
                walshspectrum[gap * (j + 1) + l] = walshspectrum[gap * j + l] - 2 * walshspectrum[gap * (j + 1) + l];
            }
        }
    }
}
//implementation of Fisher
void FisherYates(int *buf, int n) { 
	int i, j, tmp; // create local variables to hold values for shuffle
	srand(time(NULL));
	for (i = n - 1; i > 0; i--) { // for loop to shuffle
		j = rand() % (i + 1); //randomise j for shuffle with Fisher Yates
		tmp = buf[j];
		buf[j] = buf[i];
		buf[i] = tmp;
	}
}

void WalshTransformPart(array<int, 1 << N> &truthtable, array<int, 1 << N> &walshspectrum, int n_bit){
    for (int i = 0; i < 1 << n_bit; i++)
    {
		if (truthtable[i] == -1)
			walshspectrum[i] = 0;
		else
			walshspectrum[i] = 1 - 2 * truthtable[i];
	}	
	for (int i = 0; i < n_bit; i++)
    {
        int gap = 1 << (n_bit - i - 1);
        for (int j = 0; j < (2 << i); j = j + 2)
        {
            for (int l = 0; l < gap; l++)
            {
                walshspectrum[gap * j + l]       = walshspectrum[gap * j + l] +     walshspectrum[gap * (j + 1) + l];
                walshspectrum[gap * (j + 1) + l] = walshspectrum[gap * j + l] - 2 * walshspectrum[gap * (j + 1) + l];
            }
        }
    }
}

int EstimateTT_nTimes(int threshold){
	int number_tt_undetermined = 0;
	if (!times)
	{
		for (int i = 0; i < 1 << N; i++)
			tt[i] = -1;
		number_tt_undetermined = 1 << N;
	}

	number_tt_undetermined = EstimateTTByPro(tt, N, position, value, LEN, threshold);
    printf("%d,",number_tt_undetermined);
	if (times == 2)
	{
		WalshTransformPart(tt, wf, N);

		for (int i = 0; i < LEN; i++)
			wf[position[i]] = value[i];
		array<int,1<<N> pos;
		for (int i = 0; i < 1 << N; i++)
			pos[i] = i;

		number_tt_undetermined = EstimateTTByPro(tt, N, pos, wf, 1 << N, threshold);
	}
	return number_tt_undetermined;
}

int F2MatrixRank(vector<int> &matrix, int len, int n_bit){
    int Rank = n_bit;
    for (auto row = 0; row < Rank; row++)
    {   
        if (!((matrix[row]>>(n_bit - Rank + row)) & 1))
            for (auto i = row + 1; i < len; i ++)
            {
                if ((matrix[i]>>(n_bit - Rank + row)) & 1)
                {
                    swap(matrix[row], matrix[i]);
                    break;
                }
                if (i == len - 1)
                {
                    Rank--;
                    row--;
                }
            }
        for (auto i = row + 1; i < len; i++)
            if ((matrix[i]>>(n_bit - Rank + row)) & 1)
                matrix[i] ^= matrix[row];
    }
    // get the rref of matrix 
    for (auto row = 1; row < Rank; row++)
    {
        int offset = 0;
        for (auto i = row - 1; i > -1; i--)
        {
            int flag = 0;
            if ((matrix[i]>>(row + offset)) & 1)
            {
                matrix[i] ^= matrix[row];
                flag = 1;
            }
            if (flag && (i == 0))
                offset++;
        }
    }
    for (auto i = 0 ; i < Rank; i++)
    {
        int list[n_bit];
        decimalTobinary(matrix[i],list,n_bit);
        for (auto j = 0;  j < n_bit; j++)
            printf("%d ",list[j]);
        printf("\n");
    }
    return Rank;
}

int TestSolution(array<int,1<<N> &truthtable, array<int,LEN> &part_walshpoints, array<int,LEN> &part_walshspectrum){
    int flag = 0;
    array<int, 1 << N> walshspec;
    WalshTransform(truthtable, walshspec, N);
    for (auto i = 0; i < LEN; i++)
    {
        if (walshspec[part_walshpoints[i]] != part_walshspectrum[i])
        {
            flag = 0;
            break;
        }
        if (i == LEN - 1)
            flag = 1;
    }
    return flag;
}

void Count_TT_0_1(array<int,1<<N> &truthtable, vector<int> &truthtable_0,vector<int> &truthtable_1){
    for (auto i = 0; i < 1<<N; i++)
    {
        if (tt[i])
            truthtable_1.push_back(i);
        else
            truthtable_0.push_back(i);
    }
}

void FindSolBySubspace(vector<int> bit_reverse,array<int,1<<N> &tt,array<int,1<<N> &tt1,array<int,1<<N> &wf){
      array<int,1<<N> sum_1 {};
    vector<int> tt_0 {}, tt_1 {};
    Count_TT_0_1(tt,tt_0,tt_1);

    int min_sum_1 = 0;
    for (auto i = 1; i < 1 << N; i++)
        for (auto j = 0; j < LEN; j++)
            if (InnerProduct(position[j], i, N) & 1)
                sum_1[i]++;
    auto index = min_element(sum_1.begin() + 1,sum_1.end());

    auto x4 = distance(sum_1.begin(),index);
    cout << "the most orthogonal point is " << x4 << endl;

    vector<int> position_left {};
    for (auto i = 0; i < LEN; i++)
        if (InnerProduct(position[i], x4, N) & 1)
            position_left.push_back(position[i]);
    cout << "position_left complete and those position are\n";
    for (auto i:position_left)
        cout << i << " ";
    cout << endl;
    vector<int> tt_1_xor_x4(tt_1);

    for (auto &i:tt_1_xor_x4)
        i ^= x4;
    sort(tt_1_xor_x4.begin(),tt_1_xor_x4.end());
    cout << "sort complete\n";
    // cout << tt_1_xor_x4.size();
    vector<int> intersect{};
    vector<int> combination{};
    // 1056,2029,3541,4197,6407,7128,8134,9200,12164,12272,13677,15784,
    set_intersection(tt_1_xor_x4.begin(), tt_1_xor_x4.end(), tt_0.begin(), tt_0.end(), inserter(intersect, intersect.begin()));
    cout << "intersection complete and the elements in intersection are\n";
    for (auto i : intersect)
        printf("%d,",i);
    cout << endl << "Right result are below:" << endl;

    for (auto points = 2; points < intersect.size(); points += 2)
    {
        int K = points;
        Combination_NchooseK(combination, intersect.size(), K);
        for (auto rth = 0; rth < (combination.size() / K); rth++)
            for (auto i = 0; i < position_left.size(); i++)
            {
                int sum = 0;
                for (auto d = 0; d < K; d++)
                    sum += (InnerProduct(position_left[i], intersect[combination[rth * K + d]], N) & 1);
                if (sum != (K / 2))
                    break;
                if (i == position_left.size() - 1)
                {
                    vector<int> res {};
                    for (auto d = 0; d < K; d++)
                    { 
                        res.push_back(intersect[combination[rth * K + d]]);
                        res.push_back(intersect[combination[rth * K + d]] ^ x4);
                    }
                    sort(res.begin(), res.end());
                    vector<int> points_intersect {};
                    vector<int> points_union {};
                    vector<int> points_change {};
                    set_intersection(bit_reverse.begin(),bit_reverse.end(),res.begin(),res.end(),inserter(points_intersect, points_intersect.begin()));
                    set_union(bit_reverse.begin(),bit_reverse.end(),res.begin(),res.end(),inserter(points_union, points_union.begin()));
                    set_difference(points_union.begin(),points_union.end(),points_intersect.begin(),points_intersect.end(),inserter(points_change, points_change.begin()));
                    tt1 = tt;
                    for (auto d : points_change)
                        tt1[d] ^= 1;
                    WalshTransform(tt1, wf, N);
                    for (auto d = 0; d < LEN; d++)
                    {
                        if (wf[position[d]] != value[d])
                            break;
                        if (d == LEN - 1)
                        {
                            for (auto index : points_change)
                                cout << index << ",";
                            cout << endl;
                        }
                    }
                }
            }
    } 
}

void Combination_NchooseK(vector<int> &res, int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) res.push_back(i);
        }
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}