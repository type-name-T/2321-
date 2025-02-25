#include<stdio.h>
#define T int
#include"avl_tree.h"
#undef T
bool comp(const int* lhs,const int* rhs){
    return *lhs>*rhs;
}
int main(){
    construct(avl_tree_int,tree,comp);
    int arr[]={1,2,3,4,5,6,7,8,9,10};
    for(int i=0;i<10;i++){
        avl_tree_int_insert(&tree,&arr[i]);
    }
    for(avl_iterator_int it=avl_tree_int_begin(&tree);
    !avl_iterator_int_equal(it,avl_tree_int_end(&tree));
    it=avl_iterator_int_next(&it)){
        printf("%d ",avl_iterator_int_deref(&it));
    }
} 