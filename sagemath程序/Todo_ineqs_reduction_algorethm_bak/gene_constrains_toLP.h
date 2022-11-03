#ifndef   _GENECONSTRAINSTOLP_H
#define   _GENECONSTRAINSTOLP_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
typedef unsigned int word32;

int splitString(string str, string** pStr, int* subStrCount);
int s2i(const char* str, int* num);

bool is_point_satisfy_ineqVector(std::vector<int>& point_vec, std::vector<int>& set_vec);

#endif _GENECONSTRAINSTOLP_H
