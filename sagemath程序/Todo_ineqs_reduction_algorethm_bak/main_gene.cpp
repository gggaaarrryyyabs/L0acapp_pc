#include "gurobi_c++.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
#include"gene_constrains_toLP.h"
#define PATTERNSSIZE 97
#define FIRST_NAME 'C'
int lable_at_empty_constrain = 1000;//维护一个全局变量，表示约束条件的下标和lastname

int main() {

	//先把文件的patterns读到vector_A中
	//对生成每个不可能差分patterns
	//再把ineq文件的不等式读到big_ineqs_set中
    ifstream in;
    ofstream out;
    //首先，从文件中读取所有的不等式，记为一个容器big_set，元素为一维数组长度为9
    out.open("not_patterns_file.txt", ios::trunc);
    out.close();

    in.open("bigineq_file.txt", ios::in);
    if (!in) {
        cout << "open file fail!";
    }
    else cout << "open file success!" << endl;

    //vector<vector<int>>big_set;
    //int n = 327;
    //int m = 9;
    std::vector<std::vector<int>> big_set;  //用来存所有的不等式

    int mid = 0;

    while (!in.eof())
    {

        string temp;
        getline(in, temp);//这个temp里面是一个string，一行元素，形如3,-1,0,1,-1,-1,0,1,-1   需要把他们按，分开，放到一个数组中，再放入到容器big_set中
        string* pStr;  //未释放
        int* numbs;
        int subStrCount;
        int k = 0;
        splitString(temp, &pStr, &subStrCount);
        if (subStrCount <= 0)
        {
            return -1;
        }
        numbs = new int[subStrCount];
        for (int i = 0; i < subStrCount; i++)
        {
            s2i(pStr[i].c_str(), (numbs + i));
        }
        std::vector<int> temp_vec1;
        for (int i = 0; i < subStrCount; i++)
        {
            //cout << *(numbs + i) << " ";
            //big_set[mid][i] = *(numbs + i);
            //numbs[k] = *(numbs + i);
            temp_vec1.push_back(*(numbs + i));

        }
        big_set.push_back(temp_vec1);
        mid++;

    }
    in.close();
    /*
    //检查big_set
    for (int i = 0; i < big_set.size(); i++)//输出二维动态数组
    {
        for (int j = 0; j < big_set[i].size(); j++)
        {
            cout << big_set[i][j] << " ";
        }
        cout << "\n";
    }
    */


    //从文件中读取所有的patterns，放在容器中vector_A中，元素为一维数组
    //ifstream in;

    in.open("patterns_file.txt", ios::in);
    if (!in) {
        cout << "open file fail!";
    }
    else cout << "open file success!" << endl;
    //vector<vector<int>>big_set;
    int num_of_patterns = 97;
    int num_of_size = 8;
    std::vector<std::vector<int>> vector_A;

    int mid_2 = 0;
    while (!in.eof())
    {

        string temp;
        getline(in, temp);//这个temp里面是一个string，一行元素，形如3,-1,0,1,-1,-1,0,1,-1   需要把他们按，分开，放到一个数组中，再放入到容器big_set中
        string* pStr = NULL;  //未释放
        int* numbs = NULL;
        int subStrCount;
        int k = 0;
        splitString(temp, &pStr, &subStrCount);
        if (subStrCount <= 0)
        {
            return -1;
        }
        numbs = new int[subStrCount];
        for (int i = 0; i < subStrCount; i++)
        {
            s2i(pStr[i].c_str(), (numbs + i));
        }
        std::vector<int> temp_vec2;
        for (int i = 0; i < subStrCount; i++)
        {
            //cout << *(numbs + i) << " ";
            //vector_A[mid_2][i] = *(numbs + i);
            //numbs[k] = *(numbs + i);
            temp_vec2.push_back(*(numbs + i));

        }
        vector_A.push_back(temp_vec2);
        mid_2++;

    }
    in.close();
    //对应生成所有的非patterns，放在容器vector_B中，元素为一组数组，并保存为文件
    int num_of_init = 256;
    std::vector<std::vector<int>> vector_Init_256;
    for (int i = 0; i < num_of_init; i++) {
        std::vector<int> temp_vec3;
        for (int j = 0; j < num_of_size; j++) {
            //把i转化为8位的二进制，分别对vector_Init_256[i][j]的每一位赋值，先化为二进制的string就可以了
            //vector_Init_256[i][j] = (i >> j) % 2;
            temp_vec3.push_back((i >> j) % 2);
        }
        vector_Init_256.push_back(temp_vec3);
    }
    //把A中的点全化为十进制，存在一个数组里面，遍历0-255，生成A的补集vector_B      所以问题出在B上面
    //int num_of_vB = num_of_init - num_of_patterns;
    std::vector<std::vector<int>> vector_B;

    int A_Int_array[PATTERNSSIZE];
    for (int i = 0; i < PATTERNSSIZE; i++) {
        int temp_int_num = 0;
        for (int j = 0; j < 8; j++) {
            int k = 7 - j;
            temp_int_num += vector_A[i][j] * pow(2, k);
        }
        A_Int_array[i] = temp_int_num;
    }

    for (int i = 0; i < num_of_init; i++) {
        bool find_label = false;
        for (int j = 0; j < PATTERNSSIZE; j++) {
            if (i == A_Int_array[j]) {
                find_label = true;
            }
        }
        //如果find_label 没找到，就是说这个下标在B中，把他转化为二进制加入到B_vector中即可
        if (find_label == false) {
            vector<int> temp_member_B;
            for (int k = num_of_size - 1; k >= 0; k--) {
                //temp_member_B[k] = (i >> k) % 2;
                temp_member_B.push_back((i >> k) % 2);
            }
            vector_B.push_back(temp_member_B);
        }
    }

	//对每个不可能差分pattern，遍历一遍不等式，把不满足的不等式标号记下来，新的constrain_vector二维链表
    std::vector<std::vector<int>> constrain_vector;
    vector<vector<int> >::iterator it_im_pats;
    for (it_im_pats = vector_B.begin(); it_im_pats != vector_B.end(); it_im_pats++) {
        vector<vector<int> >::iterator it_bigset;
        std::vector<int> temp_constrains_onepoint;
        int counter = 0;
        for (it_bigset = big_set.begin(); it_bigset < big_set.end(); it_bigset++) {
            if (false == is_point_satisfy_ineqVector(*it_im_pats, *it_bigset)) {
                temp_constrains_onepoint.push_back(counter);
            }
            counter += 1;
        }
        constrain_vector.push_back(temp_constrains_onepoint);
    }

	//根据constrain_vector生成LP文件
    out.open("constrain_vector_file.lp", ios::trunc);
    out.close();
    
    //以追加输出方式打开LP文件
    out.open("constrain_vector_file.lp", ios::app);
    if (!out) {
        cout << "creat file fail!";
    }
    else cout << "open file for write success!" << endl;
    out << "Minimize" << endl << "  ";
    for (int i = 0; i < 327; i++) {

        out << "Z" << i;
        if ((i + 1) % 8 == 0) {
            out << endl << "  ";
        }
        if (i != 326) {
            out << " + ";
        }
    }
    out << endl;
    out << "Subject To" << endl;
    
    //遍历constrain_vector二维链表   把每一维的内容假设为20 54 123  156 236  ==》 Z20 + Z54 + Z156 + Z236 >= 1
    vector<vector<int> >::iterator it_constrain_vector;
    
    for (it_constrain_vector = constrain_vector.begin(); it_constrain_vector != constrain_vector.end(); it_constrain_vector++) {
        vector<int>::iterator it_constrain_onepoint;
        vector<int> temp_vec;
        out << " " << FIRST_NAME << lable_at_empty_constrain << ": ";
        for (it_constrain_onepoint = (*it_constrain_vector).begin(); it_constrain_onepoint < (*it_constrain_vector).end(); it_constrain_onepoint++) {
            //cout << *it_constrain_onepoint << " ";
            if (it_constrain_onepoint != (*it_constrain_vector).end()-1) {//可能有错
                out << "Z" << *it_constrain_onepoint << " + ";
            }
            else {
                out << "Z" << *it_constrain_onepoint << " >= 1"<<endl;
            }
        }
        lable_at_empty_constrain++;
        //cout << endl;
    }
    //设定目标值
    /*
    out << " " << FIRST_NAME << lable_at_empty_constrain << ": ";
    for (int i = 0; i < 327; i++) {
        if (i  == 326) {
            out <<" Z326 = 21"<< endl;
        }
        else {
            out << " Z" << i << " +";
        }
        
    }

    */
    out << "Bounds" << endl;
    out << "Binaries";
    for (int i = 0; i < 327; i++) {
        if (i % 8 == 0) {
            out << endl;
        }
        out << " Z" << i;
    }
    out.close();
	return 0;
}