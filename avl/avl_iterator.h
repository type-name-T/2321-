#include"def.h"
/*
 * +-----------+
 * | parent    |
 * +-----------+
 * | left      |
 * +-----------+
 * | right     |
 * +-----------+
 * | height    |
 * +-----------+
 * | data      |
 * |   ...     |
 * +-----------+
 */
typedef struct _template(avl_node){
    struct _template(avl_node)* parent;
    struct _template(avl_node)* left;
    struct _template(avl_node)* right;
    unsigned int height;
    T data;
}_template(avl_node);
typedef struct _template(avl_tree){
    /* avl tree node */
    _template(avl_node) root;
    size_t size;
    /* compare function for the inserting order */
    Comp comp;
}_template(avl_tree);
typedef struct _template(avl_iterator){                    /* for avl tree or rb tree */
    _template(avl_node)* ref;
    _template(avl_tree)* tree;       /* point to the avl tree or rb tree*/
}_template(avl_iterator);
/* range type */
typedef struct _template(avl_range){
    _template(avl_iterator) begin;
    _template(avl_iterator) end;
}_template(avl_range);
/*return type for insert*/

/**
 * Create avl tree iterator.
 */
def_construct(_template(avl_iterator)){
    _this->ref=NULL;
    _this->tree=NULL;
}
/**
 * Get data value referenced by iterator.
 */
T def_memfn(_template(avl_iterator),deref){ 
    return _this->ref->data;
}
/**
 * Get data value pointer referenced by iterator.
 */
T* def_memfn(_template(avl_iterator),ptr){
    if (_this->ref == NULL) {
        return NULL; 
    }
    return &(_this->ref)->data;
}
/**
 * Return iterator reference next element.
 */
_template(avl_iterator) def_memfn(_template(avl_iterator),next){
    _template(avl_iterator) iter = *_this;
    _template(avl_node)* next = NULL;  /* next pos */
    _template(avl_node)* current = iter.ref;
    if (current->right != NULL) {
        /*
         *        A
         *       / \
         *      B   C
         *     / \   \
         *    D   E   F
         *       /
         *      G
         *  current : B
         *  next    : G
         */
        next = current->right;
        while (next->left != NULL) {
            next = next->left;
        }

        iter.ref = next;
    } else {
        /*
         *     avlroot
         *        |
         *        A
         *       / \
         *      B   C
         *     / \   \
         *    D   E   F
         *       /
         *      G
         */
        next = current->parent;
        while (current == next->right) {
            current = next;
            next = next->parent;
        }
        /* 
         * this is special condition, when the next pos is root's parent.
         * when the avl tree has only one node, this condition is came.
         */
        if (current->right != next) {
            /*
             * current : E or F
             * next    : A or avlroot
             */
            iter.ref = next;
        } else {
            /* here is special condition */
            /*
             *   avlroot
             *      |
             *      A
             */
            iter.ref = current;
        }
    }
    return iter;
}
/**
 * Return iterator reference previous element.
 */
_template(avl_iterator) def_memfn(_template(avl_iterator),prev){
    _template(avl_iterator) iter = *_this;
    _template(avl_node)* prev = NULL;
    _template(avl_node)* current = iter.ref;

    // If the iterator points to the end, move to the last node
    if (current == NULL) {
        // Assuming _avl_tree_end sets the iterator to the rightmost node
        iter.ref = iter.tree->root.right;
        return iter;
    }

    // If the current node has a left child
    if (current->left != NULL) {
        /*
         *        A
         *       / \
         *      B   C
         *     / \   \
         *    D   E   F
         *       /
         *      G
         *  current  : A
         *  previous : E
         */
        prev = current->left;
        while (prev->right != NULL) {
            prev = prev->right;
        }

        iter.ref = prev;
    } else {
        /*
         *        A
         *       / \
         *      B   C
         *     /   / \
         *    D   E   F
         *       /
         *      G
         * 
         *  current  : G
         *  previous : A
         */
        prev = current->parent;
        while (prev != NULL && current == prev->left) {
            current = prev;
            prev = prev->parent;
        }

        // If we reach the root, prev will be NULL
        if (prev == NULL) {
            // The iterator is already at the beginning, no previous node
            iter.ref = NULL;
        } else {
            iter.ref = prev;
        }
    }

    return iter;
}
/**
 * Test the two avl tree iterator are equal.
 */
bool _static(_template(avl_iterator),equal)(_template(avl_iterator) lhs, _template(avl_iterator) rhs)
{
    // Check if the iterators belong to the same tree and have the same core position
    return (lhs.tree == rhs.tree) && 
           (lhs.ref == rhs.ref);
}
/**
 * Calculate distance between two iterators.
 */
unsigned _static(_template(avl_iterator),distance)(_template(avl_iterator) first,_template(avl_iterator) last){
    unsigned distance = 0;
    for (_template(avl_iterator) it=first;!_static(_template(avl_iterator),equal)(first,last);it=memfn(_template(avl_iterator),prev)(&first)) {
        ++distance;
    }
    return distance;
}