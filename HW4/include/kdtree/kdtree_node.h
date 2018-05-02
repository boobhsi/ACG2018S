#ifndef KDTREE_NODE_H
#define KDTREE_NODE_H

#include <include.h>
#include <kdtree/block.h>
#include <objects/object.h>
#include <kdtree/kdtree_node.h>
#include <vector>

using namespace std;

typedef enum{TRIANGLE_START, TRIANGLE_END} TrianglePointState;
typedef pair<float, TrianglePointState> SAHCandidate;

class KDTreeNode {
friend class KDTree;
public:
    KDTreeNode(int l);
    void build(int axis = 0);
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
    bool haveBlock;
    void set_block(Block* in);
    static bool compare_splitting_point(SAHCandidate i, SAHCandidate j);
};

#endif