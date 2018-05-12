#include <kdtree/block.h>

Block::Block(vec4 p1, vec4 p2, vec4 p3, vec4 p4, vec4 p5, vec4 p6, vec4 p7, vec4 p8) {
    init(p1, p2, p3, p4, p5, p6, p7, p8);
}

void Block::init(vec4 p1, vec4 p2, vec4 p3, vec4 p4, vec4 p5, vec4 p6, vec4 p7, vec4 p8) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
    points[4] = p5;
    points[5] = p6;
    points[6] = p7;
    points[7] = p8;
}

void Block::draw() {
    boundary = new Triangle*[BLOCK_TRIANGLE_NUM];
    for(int i = 0; i < BLOCK_TRIANGLE_NUM; i++) {
        boundary[i] = new Triangle();
    }

    boundary[0]->set_points(points[2], points[0], points[1]);
    boundary[1]->set_points(points[2], points[1], points[3]);
    boundary[2]->set_points(points[6], points[4], points[0]);
    boundary[3]->set_points(points[6], points[0], points[2]);
    boundary[4]->set_points(points[4], points[5], points[1]);
    boundary[5]->set_points(points[4], points[1], points[0]);
    boundary[6]->set_points(points[6], points[7], points[3]);
    boundary[7]->set_points(points[6], points[3], points[2]);
    boundary[8]->set_points(points[6], points[4], points[5]);
    boundary[9]->set_points(points[6], points[5], points[7]);
    boundary[10]->set_points(points[7], points[5], points[1]);
    boundary[11]->set_points(points[7], points[1], points[3]);
}

Intersection_info Block::checkIntersection(Ray& nray) {
    //find nearest object, return nearest surface
    Intersection_info n_i;
    /* 
    for(int i = 0; i < BLOCK_TRIANGLE_NUM; i++) {
        Intersection_info temp_intersect = boundary[i]->checkIntersection(nray);
        if(temp_intersect.intersected) {
            if(n_i.intersected) {
                if(n_i.t > temp_intersect.t) {
                    n_i = temp_intersect;
                }
            }
            else {
                n_i = temp_intersect;
            }
        }
    }
    */
    float tmin = -10000000000.0f, tmax = 10000000000.0f;
    bool intersected = false;
    //vec4 direction_inv(1 / nray.getVector()[0], 1 / nray.getVector()[1], 1 / nray.getVector()[2], 0.0f);
    if(nray.getVector()[0] > 0.0f || nray.getVector()[0] < 0.0f) {
        float x_inv = 1 / nray.getVector()[0];
        float tx1 = (points[6][0] - nray.getOrigin()[0]) * x_inv;
        float tx2 = (points[1][0] - nray.getOrigin()[0]) * x_inv;

        tmin = std::min(tx1, tx2);
        tmax = std::max(tx1, tx2);
        intersected = true;
    }
    if(nray.getVector()[1] > 0.0f || nray.getVector()[1] < 0.0f) {
        float y_inv = 1 / nray.getVector()[1];
        float ty1 = (points[6][1] - nray.getOrigin()[1]) * y_inv;
        float ty2 = (points[1][1] - nray.getOrigin()[1]) * y_inv;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
        intersected = true;
    }

    if(nray.getVector()[2] > 0.0f || nray.getVector()[2] < 0.0f) {
        float z_inv = 1 / nray.getVector()[2];
        float tz1 = (points[6][2] - nray.getOrigin()[2]) * z_inv;
        float tz2 = (points[1][2] - nray.getOrigin()[2]) * z_inv;

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
        intersected = true;
    }

    n_i.intersected = (tmax >= tmin) && intersected;
    n_i.t = tmin;
    
    return n_i;
}

BlockSet Block::slash(int axis, float pos) {
    BlockSet answer;
    switch(axis) {
        case 0: {
            vec4 np1(pos, points[0][1], points[0][2], 1.0f);
            vec4 np2(pos, points[1][1], points[1][2], 1.0f);
            vec4 np3(pos, points[2][1], points[2][2], 1.0f);
            vec4 np4(pos, points[3][1], points[3][2], 1.0f);
            Block* l_block = new Block(np1, np2, np3, np4, points[4], points[5], points[6], points[7]);
            Block* r_block = new Block(points[0], points[1], points[2], points[3], np1, np2, np3, np4);
            answer.first = l_block;
            answer.second = r_block;
            break;
        }
        case 1: {
            vec4 np1(points[0][0], pos, points[0][2], 1.0f);
            vec4 np2(points[1][0], pos, points[1][2], 1.0f);
            vec4 np5(points[4][0], pos, points[4][2], 1.0f);
            vec4 np6(points[5][0], pos, points[5][2], 1.0f);
            Block* u_block = new Block(points[0], points[1], np1, np2, points[4], points[5], np5, np6);
            Block* b_block = new Block(np1, np2, points[2], points[3], np5, np6, points[6], points[7]);
            answer.first = u_block;
            answer.second = b_block;
            break;
        }
        case 2: {
            vec4 np1(points[0][0], points[0][1], pos, 1.0f);
            vec4 np3(points[2][0], points[2][1], pos, 1.0f);
            vec4 np5(points[4][0], points[4][1], pos, 1.0f);
            vec4 np7(points[6][0], points[6][1], pos, 1.0f);
            Block* f_block = new Block(points[0], np1, points[2], np3, points[4], np5, points[6], np7);
            Block* b_block = new Block(np1, points[1], np3, points[3], np5, points[5], np7, points[7]);
            answer.first = f_block;
            answer.second = b_block;
            break;
        }
        default: {
            answer.first = nullptr;
            answer.second = nullptr;
            break;
        }
    }
    return answer;
}

Block::Block(BoundingBox bx) {
    mbx = bx;
}

void Block::extend(BoundingBox bx) {
    if(mbx.m_x < bx.m_x) mbx.m_x = bx.m_x;
    if(mbx.s_x > bx.s_x) mbx.s_x = bx.s_x;
    if(mbx.m_y < bx.m_y) mbx.m_y = bx.m_y;
    if(mbx.s_y > bx.s_y) mbx.s_y = bx.s_y;
    if(mbx.m_z < bx.m_z) mbx.m_z = bx.m_z;
    if(mbx.s_z > bx.s_z) mbx.s_z = bx.s_z;
}

void Block::init_box() {
    vec4 p1(mbx.m_x, mbx.m_y, mbx.s_z, 1.0f);
    vec4 p2(mbx.m_x, mbx.m_y, mbx.m_z, 1.0f);
    vec4 p3(mbx.m_x, mbx.s_y, mbx.s_z, 1.0f);
    vec4 p4(mbx.m_x, mbx.s_y, mbx.m_z, 1.0f);
    vec4 p5(mbx.s_x, mbx.m_y, mbx.s_z, 1.0f);
    vec4 p6(mbx.s_x, mbx.m_y, mbx.m_z, 1.0f);
    vec4 p7(mbx.s_x, mbx.s_y, mbx.s_z, 1.0f);
    vec4 p8(mbx.s_x, mbx.s_y, mbx.m_z, 1.0f);
    init(p1, p2, p3, p4, p5, p6, p7, p8);
}

bool Block::isInside(Ray& nray) {
    vec4 o = nray.getOrigin();
    return o[0] < points[0][0] && o[0] > points[4][0] && o[1] < points[0][1] && o[1] > points[2][1] && o[2] < points[1][2] && o[2] > points[0][2];
}

