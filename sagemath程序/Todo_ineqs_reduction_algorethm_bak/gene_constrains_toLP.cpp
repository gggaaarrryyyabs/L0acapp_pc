#include"gene_constrains_toLP.h"

int splitString(string str, string** pStr, int* subStrCount)
{
    int strLen = str.length();
    int pLen = (strLen + 4) / 4 * 4;
    char* pSource = new char[pLen];
    char* p;
    memset(pSource, 0, pLen);
    memcpy(pSource, str.c_str(), str.length());
    (*subStrCount) = 0;
    for (int i = 0; i < strLen; i++)
    {
        if (*(pSource + i) == ',')
        {
            (*subStrCount)++;
            *(pSource + i) = 0;
        }
    }
    //字符串不是以分隔符结束(最后一个分割符后还有字符串)
    if (*(pSource + strLen - 1) != ',')
    {
        (*subStrCount)++;
    }
    *pStr = new string[*(subStrCount)];
    p = pSource;
    for (int i = 0; i < *subStrCount; i++)
    {
        (*pStr)[i] = p;
        p += (*pStr)[i].length() + 1;
    }
    return 0;
}

int s2i(const char* str, int* num)
{
    int i = 0;
    *num = 0;
    if (strlen(str) == 0) {
        return -1;
    }
    if (*(str) != '-') {
        for (i = 0; i < strlen(str); i++) {
            if (*(str + i) < '0' || *(str + i) > '9') {
                return -1;
            }
            *num = (*num) * 10 + (*(str + i)) - '0';
        }
    }
    else {
        for (i = 1; i < strlen(str); i++)
        {

            if (*(str + i) < '0' || *(str + i) > '9')
            {
                return -1;
            }
            *num = (*num) * 10 + (*(str + i)) - '0';
        }
        *num = (*num) * -1;
    }

    return 0;
}

bool is_point_satisfy_ineqVector(std::vector<int>& point_vec, std::vector<int>& set_vec) {
    //验证某个点是否满足不等式。
    bool label_satisfy = true;
    int sum = 0;
    if (point_vec.size() == 8 && set_vec.size() == 9) {
        for (int i = 0; i < 8; i++) {
            int j = i + 1;
            sum += point_vec[i] * set_vec[j];
        }
        sum += set_vec[0];
        if (sum >= 0) {
            label_satisfy = true;
        }
        else {
            label_satisfy = false;
        }

    }
    else {
        cout << "allert, input fault!" << endl;
    }
    return label_satisfy;
}
