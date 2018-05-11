#include <kdtree/kdtree.h>
#include <kdtree/block.h>
#include <kdtree/kdtree_node.h>

KDTree::KDTree() {
    
}

KDTreeNode* KDTree::get_root() {
    return root_node;
}

void KDTree::build_tree(vector<Object*>& object_list) {
    root_node = new KDTreeNode(0);
    root_node->mFlag = "n";
    root_node->object_list = object_list;
    root_node->build(0);
}