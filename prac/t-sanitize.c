// buggy.c
#include <stdlib.h>

int main() {
    // 越界访问
    int *arr = malloc(10 * sizeof(int));
    arr[10] = 42;  // 错误：访问 arr[10]，但只有 arr[0]-arr[9]
    free(arr);
    
    // 释放后使用
    int *ptr = malloc(sizeof(int));
    free(ptr);
    *ptr = 10;  // 错误：ptr 已被释放
    
    return 0;
}

