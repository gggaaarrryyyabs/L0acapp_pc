#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define SWAP(a,b) {int temp = a;a = b;b = temp;}


void bubble_sort(int arr[], int len){
    int i,j,temp;
    for (int i=0;i<len-1;i++){
        for (int j=0;j<len-1-i;j++){
            if (arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void selection_sort(int arr[],int len){
    int i,j,temp;
    for (int i=0;i<len;i++)
    {
        // 默认第一个元素最小
        int min_index = i;
        for (int j=i+1;j<len;j++)
            if (arr[j] < arr[min_index])
                min_index = j;
        // 如果检测出最小值index发生改变，则
        if (min_index != i)
        {
            temp = arr[min_index];
            arr[min_index] = arr[i];
            arr[i] = temp;
        }
    }
}

void insertion_sort(int arr[],int len){
    int i,j,temp;
    for (int i = 1; i < len; i++)
    {
        temp = arr[i];
        j = i;
        while (j > 0 && arr[j-1] > temp)
        {
            arr[j] = arr[j-1];
            j--;
        }
        arr[j] = temp;
    }
}

void shell_sort(int arr[], int len){
    int i,j,gap,temp;
    for (int gap = len>>1; gap > 0; gap = gap>>1)
    {
        for (int i = gap; i < len; i++)
        {
            temp = arr[i];
            j = i-gap;
            while (j >= 0 && arr[j] > temp)
            {
                arr[j+gap] = arr[j];
                j -= gap;
            }
            arr[j+gap] = temp;
        }     
    }
}

void merge_sort(int arr[],int len){
    int reg[len];
    merge_sort_recursive(arr,reg,0,len-1);
}
void merge_sort_recursive(int arr[],int reg[],int start,int end){
    if (start >= end)
        return;
    int length = end - start;
    int mid = ( length >> 1) + start;
    int start1 = start, end1 = mid;
    int start2 = mid + 1, end2 = end;
    merge_sort_recursive(arr,reg,start1,end1);
    merge_sort_recursive(arr,reg,start2,end2);
    int k = start;
    while (start1 <= end1 && start2 <= end2)  
        reg[k++] = arr[start1] < arr[start2] ? arr[start1++] : arr[start2++];
    while (start1 <= end1)
        reg[k++] = arr[start1++];
    while (start2 <= end2)
        reg[k++] = arr[start2++];
    for (int k=start;k<=end;k++)
        arr[k] = reg[k];        
}

void quick_sort_recursive(int arr[],int start,int end){
    if (start >= end)
        return;
    // 最后一个元素为基准值
    int standard = arr[end];
    int left = start;
    int right = end - 1;
    // 比较与基准值的大小关系，小的在左边，大的在右边，相等的不管
    while (left < right)
    {
        while (arr[left] <= standard && left < right)
            left++;
        while (arr[right] >= standard && left < right)
            right--;
        SWAP(arr[left],arr[right]);
    }
    // 比较到最后，如果大于基准值，则交换
    if (arr[left] > arr[end]){
        SWAP(arr[left],arr[end]);
    }
    else{
        left++;
    }
    if (left > 0)    
        quick_sort_recursive(arr,start,left-1);
    quick_sort_recursive(arr,left+1,end);
}
void quick_sort(int arr[],int len){
    quick_sort_recursive(arr,0,len-1);
}

// library sort has not been completed
void library_sort(int arr[],int len){
    int j;
    // the length of arr after inserting zeros between elements
    int len_expended = (int) len*(1+1);
    int *ordered_arr = (int *) malloc(len_expended*sizeof(int));
    // label gaps
    int flag = 1<<31;
    for (int i=0;i<len_expended;i++)
        ordered_arr[i] = flag;
    
    int index = 1;
    // initialize the number of insert elements
    int numOfInsertElem = 1;
    // set the first element of expanded arr to be arr[0] to avoid all flag
    ordered_arr[0] = arr[0];
    // run the code 
    while (len > numOfInsertElem)
    {
        // for all insert elements 
        for (int j = 0; j < numOfInsertElem; j++)
        {
            int low = 0;
            int high = 2*numOfInsertElem - 1;
            while (low <= high)
            {
                int mid = low + (high - low)/2;
                int temp_mid = mid;
                // binary search 
                while (ordered_arr[mid] == flag)
                {
                    // run to right side to find no flag
                    mid++;
                    // if not, run to left side
                    if(mid == high)
                    {
                        mid = temp_mid - 1;
                        while (ordered_arr[mid] == flag)
                            mid--;
                        // if find, break
                        break;
                    }
                }
                // reduce the range of sort
                if (arr[index] > ordered_arr[mid])
                {
                    low = mid + 1;
                    while (ordered_arr[low] == flag)
                        low++;
                }
                else{
                    high = mid - 1;
                }
            }
            // insert arr[index] into ordered_arr[high+1]
            // if ordered_arr[high+1] is empty
            if (ordered_arr[high+1] == flag)
            {
                ordered_arr[high+1] = arr[index];
            }   
            // if ordered_arr[high+1] is not empty
            else
            {
                int temp = high + 1;
                // if not empty, first consider the elements before insert position
                while (ordered_arr[temp] != flag)
                {
                    temp--;
                    if (temp < 0)
                    {
                        temp = high + 1;
                        break;
                    }
                }
                // all elements before insert position are not empty, then consider elements after insert position
                while (ordered_arr[temp] != flag)
                    temp++;
                // 
                while (temp < high)
                {
                    ordered_arr[temp] = ordered_arr[temp + 1];
                    temp++;
                }
                while (temp > high+1)
                {
                    ordered_arr[temp] = ordered_arr[temp - 1];
                    temp--;
                }
                ordered_arr[temp] = arr[index];
            }
            index++;
            if (index == len)
                break;
        }
        numOfInsertElem *= 2;
        int generatedIndex;
        // rebalance
        for (int j=numOfInsertElem;j>0;j--)
        {
            if (ordered_arr[j] == flag)
                continue;
            generatedIndex = j*2;
            if (generatedIndex >= len_expended)
            {
                generatedIndex = len_expended - 1;
                if(ordered_arr[generatedIndex] != flag)
                    break;
            }
            ordered_arr[generatedIndex] = ordered_arr[j];
            ordered_arr[j] = flag;
        }
    }
    for (int i=0;i<len_expended;i++)
        printf("%d,",ordered_arr[i]); 
    // int k;
    // int count;
    // while(k < len_expended)
    // {
    //     if(ordered_arr[k] != flag)
    //     {
    //         arr[count] = ordered_arr[k];
    //         count++;
    //         if (count == len)
    //             break;
    //     }
    //     k++;
    // }
}


int main(){
    int arr[] = {1,2,44,3,5,11,7,24,4,13,9,10};
    int len = (int) sizeof(arr)/sizeof(arr[0]);
    library_sort(arr,len);
    // for (int i = 0; i < len; i++)
    //     printf("%d ", arr[i]);
    // printf(",%d,",1<<31);
}