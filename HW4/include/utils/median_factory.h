#ifndef MEDIAN_FACTORY_H
#define MEDIAN_FACTORY_H

#include <vector>
#include <algorithm>

using namespace std;

template <class T>
using Compare = bool (*)(T* , T*);

template <class T>
T* median_of_median(vector<T*>& list, Compare<T> compare_function = nullptr) {
    if(list.size() <= 5) {
        return get_median_from_sublist(list, compare_function);
    }
    else {
        vector<T*> median_list;
        size_t now_count = 0;
        while(now_count < list.size()) {
            vector<T*> temp_list;
            size_t i;
            for(i = 0; i < 5 && i + now_count < list.size(); i++) {
                temp_list.push_back(list[i + now_count]);
            }
            now_count += i;
            if(temp_list.size() <= 5) {
                T* median_s = get_median_from_sublist(temp_list, compare_function);
                median_list.push_back(median_s);
            }
            else {
                T* median_f = get_median_from_five(temp_list, compare_function);
                median_list.push_back(median_f);
            }
        }
        T* median_m = median_of_median(median_list, compare_function);
        vector<T*> left_list, right_list;
        for(size_t i = 0; i < list.size(); i++) {
            if(list[i] == median_m) {
                continue;
            }
            else if(compare_function(list[i], median_m)) {
                left_list.push_back(list[i]);
            }
            else {
                right_list.push_back(list[i]);
            }
        }
        size_t k;
        k = list.size() / 2;
        if(left_list.size() == k) {
            return median_m;
        }
        else if(left_list.size() > k) {
            return median_of_median(left_list, compare_function);
        }
        else {
            return median_of_median(right_list, compare_function);
        }
    }
}

template <class T> 
T* get_median_from_five(vector<T*>& list, Compare<T> compare_function = nullptr) {
    vector<T*> temp_list = list;
    sort(temp_list.begin(), temp_list.end(), compare_function);
    return temp_list[2];
}

template <class T>
T* get_median_from_sublist(vector<T*>& list, Compare<T> compare_function = nullptr) {
    vector<T*> temp_list = list;
    sort(temp_list.begin(), temp_list.end(), compare_function);
    switch(temp_list.size()) {
        case 1: return temp_list[0];
        case 2: return temp_list[1];
        case 3: return temp_list[1];
        case 4: return temp_list[2];
        case 5: return temp_list[2];
    }
    return nullptr;
}

template<class T>
class XMedianFactory
{
public:
    static T* getMedian(vector<T*>& list) {
        return median_of_median(list, compare_by);
    }
    static bool compare_by(T* l, T* r) {
        T& lr = *l;
        T& rr = *r;
        return lr[0] < rr[0];
    }
private:
    XMedianFactory();
};

template<class T>
class YMedianFactory
{
public:
    static T* getMedian(vector<T*>& list) {
        return median_of_median(list, compare_by);
    }
    static bool compare_by(T* l, T* r) {
        T& lr = *l;
        T& rr = *r;
        return lr[1] < rr[1];
    }
private:
    YMedianFactory();
};

template<class T>
class ZMedianFactory 
{
public:
    static T* getMedian(vector<T*>& list) {
        return median_of_median(list, compare_by);
    }
    static bool compare_by(T* l, T* r) {
        T& lr = *l;
        T& rr = *r;
        return lr[2] < rr[2];
    }
private:
    ZMedianFactory();
};

#endif