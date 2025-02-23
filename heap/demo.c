#include <stdio.h>
#define T int
#include "heap.h"
#undef T
void print_arr(int *begin,int *end){
    int* it=begin;
    while(it!=end){
        printf("%d ",*it);
        it++;
    }
    putchar('\n');
}
int main(){
    int arr[]={1,2,3,4,5,6,7,8,9,10};
    make_heap_int(arr,arr+10,NULL);
    print_arr(arr,arr+10);
    for(int i=0;i<10;i++){
        pop_heap_int(arr,arr+10-i,NULL);
        print_arr(arr,arr+10);
    }
}