#include"def.h"
typedef bool (*Comp)(const T* lhs, const T* rhs);
void swap(T* lhs, T* rhs) {
    T temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}
bool std_less(const T* lhs, const T* rhs) {
    return *lhs < *rhs;
}
/**
 * Adjust specific position element into a heap.
 */
void overload(adjust_heap)
(T* it_first, T* it_last, T* it_parent, Comp bfun_op) {
    bool b_result = false;
    size_t t_len = 0;
    size_t t_ppos = 0;
    size_t t_lpos = 0;
    size_t t_rpos = 0;
    T* it_left;
    T* it_right;

    if (it_first != it_last) {
        /* get left child and right child */
        t_len = it_last - it_first;
        t_ppos = it_parent - it_first;
        t_lpos = t_ppos * 2 + 1;
        t_rpos = t_ppos * 2 + 2;

        while (t_lpos < t_len) {
            if (t_rpos < t_len) {
                it_left = it_first + t_lpos;
                it_right = it_first + t_rpos;

                if (bfun_op(it_right, it_left)) {/* it_right < it_left */
                    if (bfun_op(it_left, it_parent)) {/* it_left < it_parent */
                        break;
                    }
                    else {/* it_left >= it_parent */
                        swap(it_parent, it_left);
                        it_parent = it_left;
                        t_ppos = t_lpos;
                    }
                }
                else {/* it_right >= t_lef */
                    if (bfun_op(it_right, it_parent)) { /* it_right < it_parent */
                        break;
                    }
                    else {/* don't have right child */
                        swap(it_parent, it_right);
                        it_parent = it_right;
                        t_ppos = t_rpos;
                    }
                }
            }
            else {
                it_left = it_first + t_lpos;

                if (bfun_op(it_left, it_parent)) { /* it_left < it_parent */
                    break;
                }
                else { /* it_left >= it_parent */
                    swap(it_parent, it_left);
                    it_parent = it_left;
                    t_ppos = t_lpos;
                }
            }

            t_lpos = t_ppos * 2 + 1;
            t_rpos = t_ppos * 2 + 2;
        }
    }
}
