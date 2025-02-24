#include"avl_iterator.h"
typedef struct avl_insert_result {
    _template(avl_node)* adjust;   /* the adjusted root _template(avl_node) pointer */
    _template(avl_node)* new_node;      /* the new inserting _template(avl_node)* pointer */
}avl_insert_result;
def_construct(_template(avl_node)){

}
/**
 * Get avl node height.
 */
int _static(_template(avl_tree),get_height)(const _template(avl_node)* root){
    return root == NULL ? -1 : (int)root->height;
}
/**
 * Destroy the subtree with postorder traverse.
 */
void _static(_template(avl_tree), destroy_subtree)(_template(avl_node)* root) {
    if (root != NULL) {
        /* recursively destroy left and right subtrees */
        _static(_template(avl_tree), destroy_subtree)(root->left);
        _static(_template(avl_tree), destroy_subtree)(root->right);
        /* free the node itself */
        free(root);  // Use free instead of custom allocator
    }
}
/**
 * Travel subtree for find the value in preorder.
 */
_template(avl_node)* def_memfn(_template(avl_tree), find_value, _template(avl_node)* root, const T* key) {
    if (root == NULL) {
        return NULL;
    }
    if (_this->comp(key, &root->data)) {
        return memfn(_template(avl_tree), find_value)(_this, root->left, key);
    }
    if (_this->comp(&root->data, key)) {
        return memfn(_template(avl_tree), find_value)(_this, root->right, key);
    }
    else {
        return root;
    }
}
/**
 * Get minimum _template(avl_node).
 */
_template(avl_node)* _static(_template(avl_tree), get_min_avl_node)(_template(avl_node)* root) {
    _template(avl_node)* min = root;

    while (min->left != NULL) {
        min = min->left;
    }

    return min;
}
/**
 * Get maximum _template(avl_node).
 */
_template(avl_node)* _static(_template(avl_tree), get_max_avl_node)(_template(avl_node)* root) {
    _template(avl_node)* max = root;
    while (max->right != NULL) {
        max = max->right;
    }
    return max;
}
/**
 * ll Rotate.
 */
_template(avl_node)* _static(_template(avl_tree),left_single_rotate)(_template(avl_node)* root)
{
    /*
     *     A         B
     *    /         / \
     *   B    =>   C   A
     *  / \           /
     * C   D         D
     */
    _template(avl_node)* pt_left = NULL;

    /* rotate */
    pt_left = root->left;
    root->left = pt_left->right;
    pt_left->right = root;
    /* change parent */
    root->parent = pt_left;
    if (root->left != NULL) {
        root->left->parent = root;
    }
    /* update height */
    root->height = (unsigned int)(
        (_static(_template(avl_tree),get_height)(root->left) > _static(_template(avl_tree),get_height)(root->right) ?
         _static(_template(avl_tree),get_height)(root->left) : _static(_template(avl_tree),get_height)(root->right)) + 1);
    pt_left->height = (unsigned int)(
        (_static(_template(avl_tree),get_height)(pt_left->left) > _static(_template(avl_tree),get_height)(pt_left->right) ?
         _static(_template(avl_tree),get_height)(pt_left->left) : _static(_template(avl_tree),get_height)(pt_left->right)) + 1);

    return pt_left;
}

/**
 * rr Rotate.
 */
_template(avl_node)* _static(_template(avl_tree),right_single_rotate)(_template(avl_node)* root)
{
    /*
     *  A              B
     *   \            / \
     *    B     =>   A   D
     *   / \          \
     *  C   D          C
     */
    _template(avl_node)* pt_right = NULL;

    /* rotate */
    pt_right = root->right;
    root->right = pt_right->left;
    pt_right->left = root;
    /* change parent */
    root->parent = pt_right;
    if (root->right != NULL) {
        root->right->parent = root;
    }
    /* update height */
    root->height = (unsigned int)(
        (_static(_template(avl_tree),get_height)(root->left) > _static(_template(avl_tree),get_height)(root->right) ?
         _static(_template(avl_tree),get_height)(root->left) : _static(_template(avl_tree),get_height)(root->right)) + 1);
    pt_right->height = (unsigned int)(
        (_static(_template(avl_tree),get_height)(pt_right->left) > _static(_template(avl_tree),get_height)(pt_right->right) ?
         _static(_template(avl_tree),get_height)(pt_right->left) : _static(_template(avl_tree),get_height)(pt_right->right)) + 1);

    return pt_right;
}

/**
 * lr Rotate.
 */
_template(avl_node)* _static(_template(avl_tree),left_double_rotate)(_template(avl_node)* root)
{
    /*
     *       A               A                 E
     *      / \             / \              /   \
     *     B   C           E   C            B     A
     *    / \       =>    / \        =>    / \   / \
     *   D   E           B   G            D   F G   C
     *      / \         / \
     *     F   G       D   F
     */        
    root->left = _static(_template(avl_tree),right_single_rotate)(root->left);
    root->left->parent = root;
    return _static(_template(avl_tree),left_single_rotate)(root);
}

/**
 * rl Rotate.
 */
_template(avl_node)* _static(_template(avl_tree),right_double_rotate)(_template(avl_node)* root)
{
    /*
     *       A               A                   D 
     *      / \             / \                /   \
     *     B   C           B   D              A     C
     *        / \    =>       / \       =>   / \   / \
     *       D   E           F   C          B   F G   E
     *      / \                 / \
     *     F   G               G   E
     */
    root->right = _static(_template(avl_tree),left_single_rotate)(root->right);
    root->right->parent = root;
    return _static(_template(avl_tree),right_single_rotate)(root);
}
/**
 * Rebalance the subtree and update the root height.
 */
_template(avl_node)* _static(_template(avl_tree),rebalance)(_template(avl_node)* root)
{
    if (root != NULL) {
        /* if left - right > 1 */
        if (_static(_template(avl_tree),get_height)(root->left) - _static(_template(avl_tree),get_height)(root->right) > 1) {
            /* ll */
            if (_static(_template(avl_tree),get_height)(root->left->left) >=
                _static(_template(avl_tree),get_height)(root->left->right)) {
                root = _static(_template(avl_tree),left_single_rotate)(root);
            } else {
            /* lr */
                root = _static(_template(avl_tree),left_double_rotate)(root);
            }
        } else if (_static(_template(avl_tree),get_height)(root->right) - _static(_template(avl_tree),get_height)(root->left) > 1) {
        /* else if right - left > 1 */
            /* rr */
            if (_static(_template(avl_tree),get_height)(root->right->right) >= 
                _static(_template(avl_tree),get_height)(root->right->left)) {
                root = _static(_template(avl_tree),right_single_rotate)(root);
            } else {
            /* rl */
                root = _static(_template(avl_tree),right_double_rotate)(root);
            }
        }

        root->height = (unsigned int)(
            (_static(_template(avl_tree),get_height)(root->left) > _static(_template(avl_tree),get_height)(root->right) ?
            _static(_template(avl_tree),get_height)(root->left) : _static(_template(avl_tree),get_height)(root->right)) + 1);
    }

    return root;
}
/**
 * Insert the value into subtree.
 */
avl_insert_result def_memfn(_template(avl_tree),insert_avl_node,_template(avl_node)* root, const T* key)
{
    avl_insert_result insert_result;

    /* if root is NULL then allocate memory */
    if (root == NULL) {
        _new(root,_template(avl_node));
        root->left = root->right = NULL;
        root->height = 0;
        insert_result.adjust = root;
        insert_result.new_node = root;
        root->data=*key;
        return insert_result;
    }
    /* compare the value and current node */
    /* if value < current node then insert into left subtree */
    if (_this->comp(key, &root->data)) {
        insert_result = memfn(_template(avl_tree),insert_avl_node)(_this, root->left, key);
        root->left = insert_result.adjust;
        root->left->parent = root;
        root = _static(_template(avl_tree),rebalance)(root);
        insert_result.adjust = root;
        return insert_result;
    } else {
        /* insert into right subtree */
        insert_result = memfn(_template(avl_tree),insert_avl_node)(_this, root->right, key);
        root->right = insert_result.adjust;
        root->right->parent = root;

        root = _static(_template(avl_tree),rebalance)(root);
        insert_result.adjust = root;

        return insert_result;
    }
}
/**
 * Inserts an element into a avl tree.
 */
_template(avl_iterator) def_memfn (_template(avl_tree),insert_equal, const T* key)
{
    avl_insert_result result;
    construct(_template(avl_iterator), iter);

    result = memfn(_template(avl_tree),insert_avl_node)(_this, _this->root.parent, key);

    _this->root.parent = result.adjust;
    _this->root.parent->parent = &_this->root;
    _this->root.left = _static(_template(avl_tree),get_min_avl_node)(_this->root.parent);
    _this->root.right = _static(_template(avl_tree),get_max_avl_node)(_this->root.parent);
    _this->size++;

    iter.tree = _this;
    iter.ref = result.new_node;

    return iter;
}