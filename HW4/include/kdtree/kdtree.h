#ifndef KDTREE_H
#define KDTREE_H

#include <include.h>
#include <kdtree/kdtree_node.h>
#include <objects/object.h>
#include <vector>

using namespace std;

class KDTree {
public:
    KDTree();
    void build_tree(vector<Object*>& opbject_list);
    KDTreeNode* get_root();
private:
    KDTreeNode* root_node;
};

#endif