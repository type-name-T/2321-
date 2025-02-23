
#include"heap_aux.h"
void overload(push_heap)(T* first, T* last, Comp bfun_op);
void overload(pop_heap)(T* it_first, T* it_last, Comp bfun_op);
void overload(sort_heap)(T* it_first, T* it_last, Comp bfun_op);
void overload(make_heap)(T* it_first, T* it_last, Comp bfun_op);
bool overload(is_heap)(T* it_first, T* it_last, Comp bfun_op);
/**
 * Adds an element that is at the end of a range to an existing heap consisting of the prior elements in the range.
 */
void overload(push_heap)(T* first, T* last, Comp bfun_op){
    size_t t_pos = 0;
    T* parent;
    T* current;
 
    if(bfun_op == NULL)
    {
        bfun_op =  std_less;
    }
 
    /* not empty range */
    if (first != last) {
        /* get position */
        t_pos = (last - first) - 1;
        t_pos = t_pos == 0 ? t_pos : (t_pos - 1) / 2;
        current = last - 1;
        parent = first + t_pos;
        while (current != first) {
            if (bfun_op(current, parent)) { /* current < parent */
                break;
            }
 
            swap(parent, current);
            t_pos = t_pos == 0 ? t_pos : (t_pos - 1) / 2;
            current = parent;
            parent = first + t_pos;
        }
    }
}

/**
 * Removes the largest element from the front of a heap to the next-to-last position in the range and then forms a new heap from the remaining elements.
 */
void overload(pop_heap)(T* it_first, T* it_last, Comp bfun_op) {
    if (bfun_op == NULL) {
        bfun_op = std_less;
    }

    if (it_first != it_last) {
        /* swap the first and prev */
        it_last = it_last - 1;
        swap(it_first, it_last);

        overload(adjust_heap)(it_first, it_last, it_first, bfun_op);
    }
}


/**
 * Removes the largest element from the front of a heap to the next-to-last position in the range and then forms a new heap from the remaining elements.
 */
void overload(sort_heap)(T* it_first, T* it_last, Comp bfun_op)
{
    if (bfun_op == NULL) {
        bfun_op = std_less;
    }

    for (; it_first != it_last; it_last = it_last - 1) {
        overload(pop_heap)(it_first, it_last, bfun_op);
    }
}

/**
 * Converts elements from a specified range into a heap in which the first element is for which a sorting criterion may be specified with a binary predicate.
 */
void overload(make_heap)(T* it_first, T* it_last, Comp bfun_op)
{
    size_t t_pos = 0;
    size_t t_len = 0;
    T* it_parent;

    if (bfun_op == NULL) {
        bfun_op = std_less;
    }

    t_len = it_last - it_first;
    if (t_len >= 2) {
        t_pos = (t_len - 2) / 2;

        for (;;) {
            it_parent = it_first + t_pos;
            overload(adjust_heap)(it_first, it_last, it_parent, bfun_op);

            if (t_pos == 0) {
                break;
            }

            t_pos--;
        }
    }
}

/**
 * Check the specified range is a heap with user-defined predicate function.
 */
bool overload(is_heap)(T* it_first, T* it_last, Comp bfun_op)
{
    size_t t_len = 0;
    size_t t_ppos = 0;
    size_t t_lpos = 0;
    size_t t_rpos = 0;
    T* it_parent;
    T* it_left;
    T* it_right;

    if (bfun_op == NULL) {
        bfun_op =  std_less;
    }

    t_len = it_last - it_first;
    for (t_ppos = 0; t_ppos < t_len; ++t_ppos) {
        t_lpos = t_ppos * 2 + 1;
        t_rpos = t_ppos * 2 + 2;
        it_parent = it_first + t_ppos;

        if (t_lpos < t_len) {
            it_left = it_first + t_lpos;
            ;
            if (bfun_op(it_parent, it_left)) { 
                return false;
            }
        }

        if (t_rpos < t_len) {
            it_right = it_first + t_rpos;
            if (bfun_op(it_parent, it_right)) { 
                return false;
            }
        }
    }

    return true;
}
