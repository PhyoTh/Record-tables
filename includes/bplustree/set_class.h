#ifndef SET_CLASS_H
#define SET_CLASS_H

#include <vector>

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K &k = K(), const V &v = V())
    {
        key = k;
        value = v;
    }
    friend ostream &operator<<(ostream &outs, const Pair<K, V> &print_me)
    {
        outs << "key(" << print_me.key << "): value(" << print_me.value << ")";
        return outs;
    }
    friend bool operator==(const Pair<K, V> &lhs, const Pair<K, V> &rhs)
    {
        return (lhs.key == rhs.key);
    }
    friend bool operator<(const Pair<K, V> &lhs, const Pair<K, V> &rhs)
    {
        return (lhs.key < rhs.key);
    }
    friend bool operator>(const Pair<K, V> &lhs, const Pair<K, V> &rhs)
    {
        return (lhs.key > rhs.key);
    }
    friend bool operator<=(const Pair<K, V> &lhs, const Pair<K, V> &rhs)
    {
        return (lhs.key <= rhs.key);
    }
    friend Pair<K, V> operator+(const Pair<K, V> &lhs, const Pair<K, V> &rhs)
    {
        // lhs.key += rhs.key;
        return lhs;
    }
};

template <typename K, typename V>
struct MPair
{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K &k = K())
    {
        key = k;
    }
    MPair(const K &k, const V &v)
    {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K &k, const vector<V> &vlist)
    {
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    // You'll need to overload << for your vector:
    friend ostream &operator<<(ostream &outs, const MPair<K, V> &print_me)
    {
        outs << setw(10) << right << print_me.key << " :    ";
        outs << print_me.value_list;
        return outs;
    }
    friend bool operator==(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return (lhs.key == rhs.key);
    }
    friend bool operator<(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return (lhs.key < rhs.key);
    }
    friend bool operator<=(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return (lhs.key <= rhs.key);
    }
    friend bool operator>(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        return (lhs.key > rhs.key);
    }
    friend MPair<K, V> operator+(const MPair<K, V> &lhs, const MPair<K, V> &rhs)
    {
        MPair<K, V> temp(lhs.key, lhs.value_list);
        for (int i = 0; i < rhs.value_list.size(); i++)
        {
            temp.value_list.push_back(rhs.value_list.at(i));
        }
        return temp;
    }
};

#endif