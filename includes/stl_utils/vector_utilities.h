#ifndef VECTOR_UTILITIES_H
#define VECTOR_UTILITIES_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
vector<T> get_union(vector<T> v1, vector<T> v2);

template <typename T>
vector<T> get_intersection(vector<T> v1, vector<T> v2);

template <typename T>
T pop_front(vector<T> &v);

///////////////////////////////////////////////////////////////////

template <typename T>
vector<T> get_union(vector<T> v1, vector<T> v2)
{
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    vector<T> v3;

    set_union(v1.begin(), v1.end(),
              v2.begin(), v2.end(),
              back_inserter(v3));

    return v3;
}

template <typename T>
vector<T> get_intersection(vector<T> v1, vector<T> v2)
{
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    vector<T> v3;

    set_intersection(v1.begin(), v1.end(),
                     v2.begin(), v2.end(),
                     back_inserter(v3));

    return v3;
}

template <typename T>
T pop_front(vector<T> &v)
{
    T temp = v.front();
    v.erase(v.begin());
    return temp;
}
#endif