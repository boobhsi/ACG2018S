#include <kdtree/kdtree_node.h>
#include <utils/median_factory.h>
#include <cassert>
#include <constants.h>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

KDTreeNode::KDTreeNode(int l) {
    mLeft = nullptr;
    mRight = nullptr;
    mBlock = nullptr;
    layer = l;
    isLeaf = false;
    haveBlock = false;
}

void KDTreeNode::build(int axis) {
    if(!haveBlock) {
        if(mBlock != nullptr) {
            delete mBlock;
        }
        mBlock = new Block(object_list[0]->get_box());
        for(size_t i = 1; i < object_list.size(); i++) {
            mBlock->extend(object_list[i]->get_box());
        }
        mBlock->init_box();
        mBlock->draw();
    }
    if(layer == LAYER_LIMIT) {
        finalize();
        return;
    }
    if((object_list.size() + overlap_list.size()) < BLOCK_MINIMUM) {
        finalize();
        return;
    }

#ifdef KDTREE_MEDIAN
    if(object_list.size() == 0) {
        finalize();
        return;
    }

    if(object_list.size() < overlap_list.size()) {
        finalize();
        return;
    }

    Object* median = nullptr;
    vector<Object*> left_list, both_overlap_list, right_list;
    float split_point = 0.0f;
    switch(axis) {
        case 0: {
            median = XMedianFactory<Object>::getMedian(object_list);
            split_point = (*median)[0];

#ifdef DEBUG
            vector<Object*> test_list = object_list;
            sort(test_list.begin(), test_list.end(), XMedianFactory<Object>::compare_by);
            float test_median = (*test_list[test_list.size() / 2])[0];
            cout << test_median << " : " << (*median)[0] <<endl;
            split_point = test_median;
#endif

            for(size_t i = 0; i < overlap_list.size(); i++) {
                object_list.push_back(overlap_list[i]);
            }
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_x < (*median)[0]) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_x > (*median)[0]) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    both_overlap_list.push_back(object_list[i]);
                }
            }
            break;
        }
        case 1: {
            median = YMedianFactory<Object>::getMedian(object_list);
            split_point = (*median)[1];

#ifdef DEBUG
            vector<Object*> test_list = object_list;
            sort(test_list.begin(), test_list.end(), YMedianFactory<Object>::compare_by);
            float test_median = (*test_list[test_list.size() / 2])[1];
            cout << test_median << " : " << (*median)[1] <<endl;
            split_point = test_median;
#endif

            for(size_t i = 0; i < overlap_list.size(); i++) {
                object_list.push_back(overlap_list[i]);
            }
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_y < (*median)[1]) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_y > (*median)[1]) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    both_overlap_list.push_back(object_list[i]);
                }
            }
            break;
        }
        case 2: {
            median = ZMedianFactory<Object>::getMedian(object_list);
            split_point = (*median)[2];

#ifdef DEBUG
            vector<Object*> test_list = object_list;
            sort(test_list.begin(), test_list.end(), ZMedianFactory<Object>::compare_by);
            float test_median = (*test_list[test_list.size() / 2])[2];
            cout << test_median << " : " << (*median)[2] <<endl;
            split_point = test_median;
#endif

            for(size_t i = 0; i < overlap_list.size(); i++) {
                object_list.push_back(overlap_list[i]);
            }
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_z < (*median)[2]) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_z > (*median)[2]) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    both_overlap_list.push_back(object_list[i]);
                }
            }
            break;
        }
        default: {
            assert(0);
            break;
        }
    }

#ifdef DEBUG
    cout << "Mid: flag=" << mFlag << ", layey=" << layer << ", size=" << object_list.size() << ", left=" << left_list.size() << ", right=" << right_list.size() << ", overlap=" << both_overlap_list.size() <<endl;
#endif

    int new_axis = (axis + 1) % 3;
    mLeft = new KDTreeNode(layer + 1);
    mLeft->mFlag = this->append('l');
    mLeft->object_list = left_list;
    mLeft->overlap_list = both_overlap_list;
    mRight = new KDTreeNode(layer + 1);
    mRight->mFlag = this->append('r');
    mRight->object_list = right_list;
    mRight->overlap_list = both_overlap_list;

    BlockSet lrb = mBlock->slash(axis, split_point);

    lrb.first->draw();
    lrb.second->draw();
    mLeft->set_block(lrb.first);
    mRight->set_block(lrb.second);
    mLeft->build(new_axis);
    mRight->build(new_axis);

#endif

#ifdef KDTREE_SAH
    for(size_t i = 0; i < overlap_list.size(); i++) {
        object_list.push_back(overlap_list[i]);
    }

    vector<Object*> left_list, left_overlap_list, right_list, both_overlap_list;
    vector<SAHCandidate> sah_list[3];
    int left_collided[3];
    int both_collided[3];
    for(int list_i = 0; list_i < 3; list_i++) {
        int lc = 0, bc = 0;
        for(size_t i = 0; i < object_list.size(); i++) {
            switch(list_i) {
                case 0: {
                    if(object_list[i]->get_box().s_x < mBlock->get_min_x()) {
                        if(object_list[i]->get_box().m_x > mBlock->get_max_x()) {
                            bc += 1;
                        }
                        else {
                            lc += 1;
                            SAHCandidate temp(object_list[i]->get_box().m_x, TRIANGLE_END);
                            sah_list[list_i].push_back(temp);
                        }
                    }
                    else {
                        if(mBlock->get_max_x() > object_list[i]->get_box().m_x && object_list[i]->get_box().m_x > mBlock->get_min_x()) {
                            SAHCandidate temp(object_list[i]->get_box().m_x, TRIANGLE_END);
                            sah_list[list_i].push_back(temp);
                        }
                        if(mBlock->get_max_x() > object_list[i]->get_box().s_x && object_list[i]->get_box().s_x > mBlock->get_min_x()) {
                            SAHCandidate temp(object_list[i]->get_box().s_x, TRIANGLE_START);
                            sah_list[list_i].push_back(temp);
                        }
                    }
                    break;
                }
                case 1: {
                    if(object_list[i]->get_box().s_y < mBlock->get_min_y()) {
                        if(object_list[i]->get_box().m_y > mBlock->get_max_y()) {
                            bc += 1;
                        }
                        else {
                            lc += 1;
                            SAHCandidate temp(object_list[i]->get_box().m_y, TRIANGLE_END);
                            sah_list[list_i].push_back(temp);
                        }
                    }
                    else {
                        if(mBlock->get_max_y() > object_list[i]->get_box().m_y && object_list[i]->get_box().m_y > mBlock->get_min_y()) {
                            SAHCandidate temp(object_list[i]->get_box().m_y, TRIANGLE_END);
                            sah_list[list_i].push_back(temp);
                        }
                        if(mBlock->get_max_y() > object_list[i]->get_box().s_y && object_list[i]->get_box().s_y > mBlock->get_min_y()) {
                            SAHCandidate temp(object_list[i]->get_box().s_y, TRIANGLE_START);
                            sah_list[list_i].push_back(temp);
                        }
                    }
                    break;
                }
                case 2: {
                    if(object_list[i]->get_box().s_z < mBlock->get_min_z()) {
                        if(object_list[i]->get_box().m_z > mBlock->get_max_z()) {
                            bc += 1;
                        }
                        else {
                            lc += 1;
                            SAHCandidate temp(object_list[i]->get_box().m_z, TRIANGLE_END);
                            sah_list[list_i].push_back(temp);
                        }
                    }
                    else {
                        if(mBlock->get_max_z() > object_list[i]->get_box().m_z && object_list[i]->get_box().m_z > mBlock->get_min_z()) {
                            SAHCandidate temp(object_list[i]->get_box().m_z, TRIANGLE_END);
                            sah_list[list_i].push_back(temp);
                        }
                        if(mBlock->get_max_z() > object_list[i]->get_box().s_z && object_list[i]->get_box().s_z > mBlock->get_min_z()) {
                            SAHCandidate temp(object_list[i]->get_box().s_z, TRIANGLE_START);
                            sah_list[list_i].push_back(temp);
                        }
                    }
                    break;
                }
            }
        }
        sort(sah_list[list_i].begin(), sah_list[list_i].end(), compare_splitting_point);
        left_collided[list_i] = lc;
        both_collided[list_i] = bc;
    }
    int max_axis = 0;
    float min_cost = 1000000000.0f;
    float split_point = 0.0f;
    float current_cost = INTERSECTION_COST * object_list.size();
    bool found_point = false;
    for(int list_i = 0; list_i < 3; list_i++) {
        int nl = left_collided[list_i];
        int nr = object_list.size() - both_collided[list_i];
        for(size_t i = 0; i < sah_list[list_i].size(); i++) {
            if(sah_list[list_i][i].second == TRIANGLE_START) {
                nl++;
            }else {
                nr--;
            }
            float temp_point = sah_list[list_i][i].first;
            float l_area = 0.0f, r_area = 0.0f;
            switch(list_i) {
                case 0: {
                    float xl = temp_point - mBlock->get_min_x();
                    float xr = mBlock->get_max_x() - temp_point;
                    float y = mBlock->get_max_y() - mBlock->get_min_y();
                    float z = mBlock->get_max_z() - mBlock->get_min_z();
                    l_area = 2 * (xl * y + xl * z + y * z);
                    r_area = 2 * (xr * y + xr * z + y * z);
                    break;
                }
                case 1: {
                    float yl = temp_point - mBlock->get_min_y();
                    float yr = mBlock->get_max_y() - temp_point;
                    float x = mBlock->get_max_x() - mBlock->get_min_x();
                    float z = mBlock->get_max_z() - mBlock->get_min_z();
                    l_area = 2 * (yl * x + yl * z + x * z);
                    r_area = 2 * (yr * x + yr * z + x * z);
                    break;
                }
                case 2: {
                    float zl = temp_point - mBlock->get_min_z();
                    float zr = mBlock->get_max_z() - temp_point;
                    float y = mBlock->get_max_y() - mBlock->get_min_y();
                    float x = mBlock->get_max_x() - mBlock->get_min_x();
                    l_area = 2 * (zl * y + zl * x + x * y);
                    r_area = 2 * (zr * y + zr * x + x * y);
                    break;
                }
            }

            float cost = TRAVERSE_COST + INTERSECTION_COST * ( nl * l_area + nr * r_area ) / mBlock->get_area() + OVERLAP_COST * (float)both_collided[list_i] / object_list.size();

            if(min_cost > cost) {
                max_axis = list_i;
                split_point = temp_point;
                min_cost = cost;
                found_point = true;
            }
        }
    }

    if(!found_point) {
        finalize();
    }


    if(min_cost > current_cost) {
        finalize();
    }

    assert(max_axis < 3 && max_axis > -1);

    switch(max_axis) {
        case 0: {
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_x < split_point) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_x > split_point) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    both_overlap_list.push_back(object_list[i]);
                }
            }
            break;
        }
        case 1: {
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_y < split_point) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_y > split_point) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    both_overlap_list.push_back(object_list[i]);
                }
            }
            break;
        }
        case 2: {
            for(size_t i = 0; i < object_list.size(); i++) {
                if(object_list[i]->get_box().m_z < split_point) {
                    left_list.push_back(object_list[i]);
                }
                else if(object_list[i]->get_box().s_z > split_point) {
                    right_list.push_back(object_list[i]);
                }
                else {
                    both_overlap_list.push_back(object_list[i]);
                }
            }
            break;
        }
    }

#ifdef DEBUG
    cout << "Mid: flag=" << mFlag << ", layey=" << layer << ", size=" << object_list.size() << ", left=" << left_list.size() << ", right=" << right_list.size() << ", overlap=" << both_overlap_list.size() <<endl;
#endif
    
    mLeft = new KDTreeNode(layer + 1);
    mLeft->object_list = left_list;
    mLeft->mFlag = this->append('l');
    mLeft->overlap_list = both_overlap_list;
    mRight = new KDTreeNode(layer + 1);
    mRight->object_list = right_list;
    mRight->mFlag = this->append('r');
    mRight->overlap_list = both_overlap_list;

    BlockSet lrb = mBlock->slash(max_axis, split_point);

    lrb.first->draw();
    lrb.second->draw();
    mLeft->set_block(lrb.first);
    mRight->set_block(lrb.second);
    mLeft->build();
    mRight->build();

#endif
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

void KDTreeNode::set_block(Block* in) {
    mBlock = in;
    haveBlock = true;
}

bool KDTreeNode::compare_splitting_point(SAHCandidate i, SAHCandidate j) {
    return (i.first < j.first);
}

string KDTreeNode::append(char i) {
    string si(&i);
    string answer = mFlag + si;
    return answer;
}

string KDTreeNode::getFlag() {
    return mFlag;
}

void KDTreeNode::finalize() {

#ifdef DEBUG
    cout << "Finalization: flag=" << mFlag << ", layey=" << layer << ", size=" << object_list.size() << ", overlap=" << overlap_list.size() << endl;
#endif

    isLeaf = true;

#ifdef MEDIAN
    for(size_t i = 0; i < overlap_list.size(); i++) {
        object_list.push_back(overlap_list[i]);
    }
#endif
}