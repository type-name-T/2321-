#include"avl_tree_private.h"
#include<stdlib.h>
def_pair(bool,_template(avl_iterator));
def_construct(_template(avl_tree), Comp comp) {
    /* initialize avl tree root node */
    _this->root.parent = NULL;
    _this->root.left = &_this->root;
    _this->root.right = &_this->root;
    _this->root.height = 0;
    /* initialize size and compare function */
    _this->size = 0;
    _this->comp = comp;
}

def_destruct(_template(avl_tree)) {
    _static(_template(avl_tree), destroy_subtree)(&_this->root);
    _this->root.parent = NULL;
    _this->root.left = &_this->root;
    _this->root.right = &_this->root;
    _this->root.height = 0;
    _this->size = 0;
}
/**
 * Test if a avl tree is empty.
 */
bool def_memfn(_template(avl_tree), empty) {
    return _this->size == 0;
}
/**
 * Get the number of elements int the avl tree.
 */
size_t def_memfn(_template(avl_tree), size) {
    return _this->size;
}
/**
 * Return an iterator that addresses the first element in the avl tree.
 */
_template(avl_iterator) def_memfn(_template(avl_tree), begin) {
    construct(_template(avl_iterator), it);
    it.tree = _this;
    it.ref = _this->root.left;
    return it;
}
/**
 * Return an iterator that addresses the location succeeding the last element in the avl tree.
 */
_template(avl_iterator) def_memfn(_template(avl_tree), end) {
    construct(_template(avl_iterator), it);
    it.tree = _this;
    it.ref = &_this->root;
    return it;
}

/**
 * Find specific element.
 */
_template(avl_iterator) def_memfn(_template(avl_tree), find, const T* key) {
    _template(avl_iterator) it;
    it.tree = _this;
    it.ref = memfn(_template(avl_tree), find_value)(
        _this, _this->root.parent, key);
    if (it.ref == NULL) {
        it.ref = &_this->root;
    }

    return it;
}
/**
 * Erases all the elements of a avl tree.
 */
void def_memfn(_template(avl_tree), clear) {
    /* destroy all elements */
    _static(_template(avl_tree), destroy_subtree)(_this->root.parent);
    _this->root.parent =
        _this->root.left = &_this->root;
    _this->root.right = &_this->root;
    _this->size = 0;
}

/**
 * Return an iterator to the first element that is equal to or greater than a specific element.
 */
_template(avl_iterator) def_memfn(_template(avl_tree), lower_bound, const T* key) {
    _template(avl_node)* current = NULL;
    _template(avl_node)* prev = NULL;
    bool b_less = false;
    bool b_greater = false;
    construct(_template(avl_iterator), iter);
    iter.tree = _this;

    if (_this->size == 0) return memfn(_template(avl_tree), end)(_this);
    prev = _this->root.parent;

    b_less = b_greater = true;
    b_less = _this->comp(key, &prev->data);
    b_greater = _this->comp(&prev->data, key);

    current = (b_less || !b_greater) ? prev->left : prev->right;
    while (current != NULL) {
        prev = current;
        b_less = b_greater = true;
        b_less = _this->comp(key, &prev->data);
        b_greater = _this->comp(&prev->data, key);

        current = (b_less || !b_greater) ? prev->left : prev->right;
    }

    if (b_less || !b_greater) {
        iter.ref = prev;
    }
    else {
        iter.ref = prev;
        iter = memfn(_template(avl_iterator), next)(&iter);
    }


    return iter;
}
/**
 * Return an iterator to the first element that is greater than a specific element.
 */
_template(avl_iterator) def_memfn(_template(avl_tree), upper_bound, const T* key) {
    if (_this->size == 0) return memfn(_template(avl_tree), end)(_this);
    _template(avl_node)* current = NULL;
    _template(avl_node)* prev = _this->root.parent;
    construct(_template(avl_iterator), iter);
    iter.tree = _this;
    bool b_result = _this->comp(key, &prev->data);

    current = b_result ? prev->left : prev->right;
    while (current != NULL) {
        prev = current;
        b_result = _this->comp(key, &prev->data);

        current = b_result ? prev->left : prev->right;
    }

    if (b_result) {
        iter.ref = prev;
    }
    else {
        iter.ref = prev;
        iter = memfn(_template(avl_iterator), next)(&iter);
    }
    return iter;
}
/**
 * Return an iterator range that is equal to a specific element.
 */
_template(avl_range) def_memfn(_template(avl_tree), equal_range, const T* key) {
    _template(avl_range) range;

    range.begin = memfn(_template(avl_tree), lower_bound)(_this, key);
    range.end = memfn(_template(avl_tree), upper_bound)(_this, key);

    return range;
}
/**
 * Return the number of specific elements in an avl tree
 */
size_t def_memfn(_template(avl_tree), count, const T* key) {
    _template(avl_range) range;
    range = memfn(_template(avl_tree), equal_range)(_this, key);
    return memfn(_template(avl_iterator), distance)(range.begin, range.end);
}
/**
 * Inserts an element into a avl tree.
 */
_template(avl_iterator) def_memfn(_template(avl_tree),insert, const T* key){

    /* if the avl tree is empty */
    if (_this->size==0) {
        return memfn(_template(avl_tree),insert_equal)(_this, key);
    } else {
        /* find value in avl tree */
        _template(avl_iterator) it_iter = memfn(_template(avl_tree),find)(_this, key);
        /* if the value is exist */
        if (!_static(_template(avl_iterator),equal)(it_iter, memfn(_template(avl_tree),end)(_this))) {
            return memfn(_template(avl_tree),end)(_this);
        } else {
            /* insert value into avl tree */
            return memfn(_template(avl_tree),insert_equal)(_this, key);
        }
    }
}
/*
 * Erase an element in an avl tree from specificed position.
 */
void avl_tree_erase_pos(_template(avl_tree)* _this, _template(avl_iterator) it_pos)
{
    _template(avl_node)* parent = NULL;
    _template(avl_node)* current = NULL;

    current = (_template(avl_node)*)it_pos.ref;
    parent = current->parent;

    /* delete node X express deleting */
    if (current->left == NULL && current->right == NULL) {
        if (parent == &_this->root) {
            parent->parent = NULL;
        } else if (current == parent->left) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
    } else if (current->left != NULL && current->right == NULL) {
        if (parent == &_this->root) {
            parent->parent = current->left;
            parent->parent->parent = parent;
        } else if (current == parent->left) {
            parent->left = current->left;
            parent->left->parent = parent;
        } else {
            parent->right = current->left;
            parent->right->parent = parent;
        }
    } else if (current->left == NULL && current->right != NULL) {
        if (parent == &_this->root) {
            parent->parent = current->right;
            parent->parent->parent = parent;
        } else if (current == parent->right) {
            parent->right = current->right;
            parent->right->parent = parent;
        } else {
            parent->left = current->right;
            parent->left->parent = parent;
        }
    } else {
        _template(avl_node)* parent_tmp = NULL;
        _template(avl_node)* cur_tmp = NULL;

        if (parent == &_this->root) {
            cur_tmp = _static(_template(avl_tree),get_min_avl_node)(current->right);
            if (current == cur_tmp->parent) {
                cur_tmp->left = current->left;
                cur_tmp->left->parent = cur_tmp;
                cur_tmp->parent = current->parent;
                cur_tmp->parent->parent = cur_tmp;

                parent = cur_tmp;
            } else {
                parent_tmp = cur_tmp->parent;
                parent_tmp->left = cur_tmp->right;
                if (parent_tmp->left != NULL) {
                    parent_tmp->left->parent = parent_tmp;
                }

                cur_tmp->left = current->left;
                cur_tmp->left->parent = cur_tmp;
                cur_tmp->right = current->right;
                cur_tmp->right->parent = cur_tmp;
                cur_tmp->parent = current->parent;
                cur_tmp->parent->parent = cur_tmp;

                parent = parent_tmp;
            }
        } else if (current == parent->left) {
            cur_tmp = _static(_template(avl_tree),get_min_avl_node)(current->right);
            if (current == cur_tmp->parent) {
                cur_tmp->left = current->left;
                cur_tmp->left->parent = cur_tmp;
                cur_tmp->parent = current->parent;
                cur_tmp->parent->left = cur_tmp;

                parent = cur_tmp;
            } else {
                parent_tmp = cur_tmp->parent;
                parent_tmp->left = cur_tmp->right;
                if (parent_tmp->left != NULL) {
                    parent_tmp->left->parent = parent_tmp;
                }

                cur_tmp->left = current->left;
                cur_tmp->left->parent = cur_tmp;
                cur_tmp->right = current->right;
                cur_tmp->right->parent = cur_tmp;
                cur_tmp->parent = current->parent;
                cur_tmp->parent->left = cur_tmp;

                parent = parent_tmp;
            }
        } else {
            cur_tmp = _static(_template(avl_tree),get_min_avl_node)(current->right);
            if (current == cur_tmp->parent) {
                cur_tmp->left = current->left;
                cur_tmp->left->parent = cur_tmp;
                cur_tmp->parent = current->parent;
                cur_tmp->parent->right = cur_tmp;

                parent = cur_tmp;
            } else {
                parent_tmp = cur_tmp->parent;
                parent_tmp->left = cur_tmp->right;
                if (parent_tmp->left != NULL) {
                    parent_tmp->left->parent = parent_tmp;
                }

                cur_tmp->left = current->left;
                cur_tmp->left->parent = cur_tmp;
                cur_tmp->right = current->right;
                cur_tmp->right->parent = cur_tmp;
                cur_tmp->parent = current->parent;
                cur_tmp->parent->right = cur_tmp;

                parent = parent_tmp;
            }
        }
    }

    /* rebalance until to root */
    if (parent != &_this->root) {
        _template(avl_node)* pt_newcur = parent;
        parent = pt_newcur->parent;
        while (parent != &_this->root) {
            if (pt_newcur == parent->left) {
                parent->left = _static(_template(avl_tree),rebalance)(parent->left);
                parent->left->parent = parent;
            } else {
                parent->right = _static(_template(avl_tree),rebalance)(parent->right);
                parent->right->parent = parent;
            }

            pt_newcur = parent;
            parent = pt_newcur->parent;
        }
    }

    /* rebalance root */
    if (parent->parent != NULL) {
        parent->parent = _static(_template(avl_tree),rebalance)(parent->parent);
        parent->parent->parent = parent;
    }

    /* destroy node */
    //_this->destroy(&current->data, &b_result);
    free(current);
    _this->size--;
    if (_this->size == 0) {
        _this->root.parent = NULL;
        _this->root.left = &_this->root;
        _this->root.right = &_this->root;
    } else {
        _this->root.left = _static(_template(avl_tree),get_min_avl_node)(_this->root.parent);
        _this->root.right = _static(_template(avl_tree),get_max_avl_node)(_this->root.parent);
    }
}