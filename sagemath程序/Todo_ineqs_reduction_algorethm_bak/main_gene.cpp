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
int lable_at_empty_constrain = 1000;//ά��һ��ȫ�ֱ�������ʾԼ���������±��lastname

int main() {

	//�Ȱ��ļ���patterns����vector_A��
	//������ÿ�������ܲ��patterns
	//�ٰ�ineq�ļ��Ĳ���ʽ����big_ineqs_set��
    ifstream in;
    ofstream out;
    //���ȣ����ļ��ж�ȡ���еĲ���ʽ����Ϊһ������big_set��Ԫ��Ϊһά���鳤��Ϊ9
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
    std::vector<std::vector<int>> big_set;  //���������еĲ���ʽ

    int mid = 0;

    while (!in.eof())
    {

        string temp;
        getline(in, temp);//���temp������һ��string��һ��Ԫ�أ�����3,-1,0,1,-1,-1,0,1,-1   ��Ҫ�����ǰ����ֿ����ŵ�һ�������У��ٷ��뵽����big_set��
        string* pStr;  //δ�ͷ�
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
    //���big_set
    for (int i = 0; i < big_set.size(); i++)//�����ά��̬����
    {
        for (int j = 0; j < big_set[i].size(); j++)
        {
            cout << big_set[i][j] << " ";
        }
        cout << "\n";
    }
    */


    //���ļ��ж�ȡ���е�patterns������������vector_A�У�Ԫ��Ϊһά����
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
        getline(in, temp);//���temp������һ��string��һ��Ԫ�أ�����3,-1,0,1,-1,-1,0,1,-1   ��Ҫ�����ǰ����ֿ����ŵ�һ�������У��ٷ��뵽����big_set��
        string* pStr = NULL;  //δ�ͷ�
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
    //��Ӧ�������еķ�patterns����������vector_B�У�Ԫ��Ϊһ�����飬������Ϊ�ļ�
    int num_of_init = 256;
    std::vector<std::vector<int>> vector_Init_256;
    for (int i = 0; i < num_of_init; i++) {
        std::vector<int> temp_vec3;
        for (int j = 0; j < num_of_size; j++) {
            //��iת��Ϊ8λ�Ķ����ƣ��ֱ��vector_Init_256[i][j]��ÿһλ��ֵ���Ȼ�Ϊ�����Ƶ�string�Ϳ�����
            //vector_Init_256[i][j] = (i >> j) % 2;
            temp_vec3.push_back((i >> j) % 2);
        }
        vector_Init_256.push_back(temp_vec3);
    }
    //��A�еĵ�ȫ��Ϊʮ���ƣ�����һ���������棬����0-255������A�Ĳ���vector_B      �����������B����
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
        //���find_label û�ҵ�������˵����±���B�У�����ת��Ϊ�����Ƽ��뵽B_vector�м���
        if (find_label == false) {
            vector<int> temp_member_B;
            for (int k = num_of_size - 1; k >= 0; k--) {
                //temp_member_B[k] = (i >> k) % 2;
                temp_member_B.push_back((i >> k) % 2);
            }
            vector_B.push_back(temp_member_B);
        }
    }

	//��ÿ�������ܲ��pattern������һ�鲻��ʽ���Ѳ�����Ĳ���ʽ��ż��������µ�constrain_vector��ά����
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

	//����constrain_vector����LP�ļ�
    out.open("constrain_vector_file.lp", ios::trunc);
    out.close();
    
    //��׷�������ʽ��LP�ļ�
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
    
    //����constrain_vector��ά����   ��ÿһά�����ݼ���Ϊ20 54 123  156 236  ==�� Z20 + Z54 + Z156 + Z236 >= 1
    vector<vector<int> >::iterator it_constrain_vector;
    
    for (it_constrain_vector = constrain_vector.begin(); it_constrain_vector != constrain_vector.end(); it_constrain_vector++) {
        vector<int>::iterator it_constrain_onepoint;
        vector<int> temp_vec;
        out << " " << FIRST_NAME << lable_at_empty_constrain << ": ";
        for (it_constrain_onepoint = (*it_constrain_vector).begin(); it_constrain_onepoint < (*it_constrain_vector).end(); it_constrain_onepoint++) {
            //cout << *it_constrain_onepoint << " ";
            if (it_constrain_onepoint != (*it_constrain_vector).end()-1) {//�����д�
                out << "Z" << *it_constrain_onepoint << " + ";
            }
            else {
                out << "Z" << *it_constrain_onepoint << " >= 1"<<endl;
            }
        }
        lable_at_empty_constrain++;
        //cout << endl;
    }
    //�趨Ŀ��ֵ
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