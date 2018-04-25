#ifndef KDTREE_NODE_H
#define KDTREE_NODE_H

#include <include.h>
#include <kdtree/block.h>
#include <objects/object.h>
#include <kdtree/kdtree_node.h>
#include <vector>

using namespace std;

class KDTreeNode {
friend class KDTree;
public:
    KDTreeNode(int l);
    void build(int axis);
    bool is_leaf();
    KDTreeNode* get_left();
    KDTreeNode* get_right();
    Block& get_block();
    vector<Object*>& get_objects();
private:
    Block* mBlock;
    KDTreeNode* mLeft;
    KDTreeNode* mRight;
    vector<Object*> object_list;
    int layer;
    bool isLeaf;
};

#endif