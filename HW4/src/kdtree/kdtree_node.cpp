#include <kdtree/kdtree_node.h>
#include <utils/median_factory.h>
#include <cassert>

#define LAYER_LIMIT 1300
#define BLOCK_MINIMUM 12

KDTreeNode::KDTreeNode(int l) {
    mLeft = nullptr;
    mRight = nullptr;
    mBlock = nullptr;
    layer = l;
    isLeaf = false;
}

void KDTreeNode::build(int axis) {
    mBlock = new Block(object_list[0]->get_box());
    for(size_t i = 1; i < object_list.size(); i++) {
        mBlock->extend(object_list[i]->get_box());
    }
    mBlock->init_box();
    mBlock->draw();
    if(layer == LAYER_LIMIT) {
        isLeaf = true;
        return;
    }
    Object* median = nullptr;
    vector<Object*> left_list;
    vector<Object*> right_list;
    size_t repeat = 0;
    switch(axis) {
        case 0: {
            median = XMedianFactory<Object>::getMedian(object_list);
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_x < (*median)[0]) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_x > (*median)[0]) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    left_list.push_back(object_list[i]);
                    right_list.push_back(object_list[i]);
                    repeat += 1;
                }
            }
            break;
        }
        case 1: {
            median = YMedianFactory<Object>::getMedian(object_list);
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_y < (*median)[1]) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_y > (*median)[1]) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    left_list.push_back(object_list[i]);
                    right_list.push_back(object_list[i]);
                    repeat += 1;
                }
            }
            break;
        }
        case 2: {
            median = ZMedianFactory<Object>::getMedian(object_list);
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_z < (*median)[2]) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_z > (*median)[2]) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    left_list.push_back(object_list[i]);
                    right_list.push_back(object_list[i]);
                    repeat += 1;
                }
            }
            break;
        }
        default: {
            median = nullptr;
            break;
        }
    }
    assert(median != nullptr);
    if(left_list.size() < BLOCK_MINIMUM || right_list.size() < BLOCK_MINIMUM || repeat > (object_list.size() / 2)) {
        isLeaf = true;
        return;
    }
    else {
        int new_axis = (axis + 1) % 3;
        mLeft = new KDTreeNode(layer + 1);
        mLeft->object_list = left_list;
        mRight = new KDTreeNode(layer + 1);
        mRight->object_list = right_list;
        mLeft->build(new_axis);
        mRight->build(new_axis);
    }
}

bool KDTreeNode::is_leaf() {
    return isLeaf;
}

KDTreeNode* KDTreeNode::get_left() {
    return mLeft;
}

KDTreeNode* KDTreeNode::get_right() {
    return mRight;
}

Block& KDTreeNode::get_block() {
    return *mBlock;
}

vector<Object*>& KDTreeNode::get_objects() {
    return object_list;
}