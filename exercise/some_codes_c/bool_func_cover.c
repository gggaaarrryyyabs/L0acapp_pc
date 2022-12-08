#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#include<math.h>

#define n 5

typedef int (*p_array_d2)[2];

p_array_d2 list_nchoose2(int *res_nchoose2[][2])
{
    int Nrow = n*(n-1)/2;
    int i = 0;
    int value0 = 0;
    int value1 = 1;
    while(i != Nrow)
    {
        res_nchoose2[i][0] = &value0;
        res_nchoose2[i][1] = &value1;
        i++;
        value1++;
        if (value1 == n)
        {
            value0++;
            value1 = value0 + 1;
        }
    }
    for (int i=0;i<Nrow;i++)
    {
        for (int j=0;j<2;j++)
            printf("%d,",*(*(res_nchoose2+i)+j));
        printf("\n");
    }
    return res_nchoose2;

}

int main(){
    int nchoose2 = n*(n-1)/2;
    int res_nchoose2[][2] = {0};
    p_array_d2 p = list_nchoose2(res_nchoose2);
    // for (int i=0;i<nchoose2;i++)
    // {
    //     for (int j=0;j<2;j++)
    //         printf("%d,",*(*(p+i)+j));
    //     printf("\n");
    // }
}
