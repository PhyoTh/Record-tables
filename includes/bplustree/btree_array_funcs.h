#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class T>
void shift_left(T *a, int &size, int shift_here); // shift left @ pos:
                                                  //     erases @ pos

template <class T>
void shift_left(T *a, int &size, T *shift_here); // shift left @ pos:
                                                 //     erases @ pos

template <class T>
void shift_right(T *a, int &size, int shift_here); // shift right:
                                                   //       make a hole

template <class T>
void shift_right(T *a, int &size, T *shift_here); // shift right:
                                                  //    make a hole

template <class T>
T maximal(const T &a, const T &b); // return the larger of the two items

template <class T>
void swap(T &a, T &b); // swap the two items

template <class T>
int index_of_maximal(T data[], int n); // return index of the largest item in data

template <class T>
void ordered_insert(T data[], int &n, T entry); // insert entry into the sorted array
                                                // data with length n

template <class T>
int first_ge(const T data[], int n, const T &entry); // return the first element in data that is
                                                     // not less than entry

template <class T>
void attach_item(T data[], int &n, const T &entry); // append entry to the right of data

template <class T>
void insert_item(T data[], int i, int &n, T entry); // insert entry at index i in data

template <class T>
void detach_item(T data[], int &n, T &entry); // remove the last element in data and place
                                              // it in entry

template <class T>
void delete_item(T data[], int i, int &n, T &entry); // delete item at index i and place it in entry

template <class T>
void merge(T data1[], int &n1, T data2[], int &n2); // append data2 to the right of data1

template <class T>
void split(T data1[], int &n1, T data2[], int &n2); // move n/2 elements from the right of data1
                                                    // and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int &dest_size, int src_size); // copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1); // print array data

template <class T>
bool is_gt(const T data[], int n, const T &item); // item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T &item); // item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream &operator<<(ostream &outs, const vector<T> &list); // print vector list

template <typename T>
vector<T> &operator+=(vector<T> &list, const T &addme); // list.push_back addme

template <class T>
void shift_left(T *a, int &size, int shift_here)
{
    T *to_here = shift_here + a;
    T *from_here = to_here + 1;
    for (int i = shift_here; i < size - 1; i++)
    {
        *to_here = *from_here;
        to_here++;
        from_here++;
    }
    size--;
}

template <class T>
void shift_left(T *a, int &size, T *shift_here)
{
    int i = shift_here - a;
    shift_left(a, size, i);
}

template <class T>
void shift_right(T *a, int &size, int shift_here)
{
    T *to_here = a + size;
    T *from_here = to_here - 1;
    for (int i = size; i > shift_here; i--)
    {
        *to_here = *from_here;
        to_here--;
        from_here--;
    }
    size++;
}

template <class T>
void shift_right(T *a, int &size, T *shift_here)
{
    int i = shift_here - a;
    shift_right(a, size, i);
}

template <class T>
T maximal(const T &a, const T &b) // return the larger of the two items
{
    if (a > b)
        return a;
    return b;
}

template <class T>
void swap(T &a, T &b) // swap the two items
{
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
int index_of_maximal(T data[], int n) // return index of the largest item in data
{
    // assert(n >= 1);
    int max = 0;
    for (int i = 1; i < n && data[i] > data[max]; i++)
    {
        max = i;
    }
    return max;
}

template <class T>
void ordered_insert(T data[], int &n, T entry) // insert entry into the sorted array
// data with length n
{
    bool flag = false;
    for (int i = 0; i < n; i++)
    {
        if (data[i] > entry)
        {
            shift_right(data, n, i);
            data[i] = entry;
            flag = true;
            break;
        }
    }
    if (!flag)
        attach_item(data, n, entry);
}

template <class T>
int first_ge(const T data[], int n, const T &entry) // return the first element in data that is
// not less than entry
{
    for (int i = 0; i < n; i++)
    {
        if (!(data[i] < entry))
            return i;
    }
    return n;
}

template <class T>
void attach_item(T data[], int &n, const T &entry) // append entry to the right of data
{
    data[n++] = entry;
}

template <class T>
void insert_item(T data[], int i, int &n, T entry) // insert entry at index i in data
{
    shift_right(data, n, i);
    data[i] = entry;
}

template <class T>
void detach_item(T data[], int &n, T &entry) // remove the last element in data and place
// it in entry
{
    entry = data[n - 1];
    n--;
}

template <class T>
void delete_item(T data[], int i, int &n, T &entry) // delete item at index i and place it in entry
{
    entry = data[i];
    shift_left(data, n, i);
}

template <class T>
void merge(T data1[], int &n1, T data2[], int &n2) // append data2 to the right of data1
{
    for (int i = 0; i < n2; i++)
    {
        data1[n1++] = data2[i];
    }
    n2 = 0;
}

template <class T>
void split(T data1[], int &n1, T data2[], int &n2) // move n/2 elements from the right of data1
// and move to data2
{
    int count = 0, j = 0;
    if (n1 % 2 != 0)
        j = 1;
    for (int i = (n1 / 2) + j; i < n1; i++)
    {
        count++;
        data2[n2++] = data1[i];
    }
    n1 -= count;
}

template <class T>
void copy_array(T dest[], const T src[],
                int &dest_size, int src_size) // copy src[] into dest[]
{
    for (int i = 0; i < src_size; i++)
    {
        dest[i] = src[i];
    }
    dest_size = src_size;
}

template <class T>
void print_array(const T data[], int n, int pos) // print array data
{
    for (int i = 0; i < n; i++)
    {
        cout << "[ " << i << " : " << data[i] << " ]";
    }
    cout << endl;
}

template <class T>
bool is_gt(const T data[], int n, const T &item) // item > all data[i]
{
    for (int i = 0; i < n; i++)
    {
        if (data[i] >= item)
            return false;
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T &item) // item <= all data[i]
{
    for (int i = 0; i < n; i++)
    {
        if (data[i] < item)
            return false;
    }
    return true;
}

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream &operator<<(ostream &outs, const vector<T> &list) // print vector list
{
    for (int i = 0; i < list.size(); i++)
    {
        outs << list.at(i) << " ";
    }
    return outs;
}

template <typename T>
vector<T> &operator+=(vector<T> &list, const T &addme) // list.push_back addme
{
    list.push_back(addme);
    return list;
}

template <typename T>
vector<T> &operator+=(vector<T> &list1, const vector<T> &list2)
{
    for (int i = 0; i < list2.size(); ++i)
        list1.push_back(list2[i]);
    return list1;
}
#endif