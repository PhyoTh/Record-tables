#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "bplustree.h"
#include "set_class.h"

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V>> map_base;
    class Iterator
    {
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = typename map_base::Iterator())
        {
            _it = it;
        }
        Iterator operator++(int unused)
        {
            Iterator hold(*this);
            this->operator++();
            return hold;
        }
        Iterator operator++()
        {
            ++_it;
            return *this;
        }
        MPair<K, V> operator*()
        {
            return *_it;
        }
        friend bool operator==(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs._it == rhs._it;
        }
        friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs._it != rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap();

    //  Iterators
    Iterator begin();
    Iterator end();
    Iterator lower_bound(const K &key);
    Iterator upper_bound(const K &key);

    //  Capacity
    int size() const;
    bool empty() const;

    //  Element Access
    const vector<V> &operator[](const K &key) const;
    vector<V> &operator[](const K &key);
    vector<V> &at(const K &key);
    const vector<V> &at(const K &key) const;

    //  Modifiers
    void insert(const K &k, const V &v);
    void erase(const K &key);
    void clear();

    //  Operations:
    bool contains(const K &key) const;
    vector<V> &get(const K &key);

    Iterator find(const K &key);
    int count(const K &key);
    // I have not written these yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid();

    friend ostream &operator<<(ostream &outs, const MMap<K, V> &print_me)
    {
        outs << print_me.mmap << endl;
        return outs;
    }

    void print_lookup()
    {
        for (Iterator it = begin(); it != end(); ++it)
        {
            cout << *it << endl;
        }
    }

private:
    BPlusTree<MPair<K, V>> mmap;
};

template <class K, class V>
MMap<K, V>::MMap()
{
    mmap = BPlusTree<MPair<K, V>>(true);
}

template <class K, class V>
typename MMap<K, V>::Iterator MMap<K, V>::begin()
{
    return Iterator(mmap.begin());
}

template <class K, class V>
typename MMap<K, V>::Iterator MMap<K, V>::end()
{
    return Iterator(mmap.end());
}

template <class K, class V>
typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K &key)
{
    return Iterator(mmap.lower_bound(key));
}

template <class K, class V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K &key)
{
    return Iterator(mmap.upper_bound(key));
}

template <class K, class V>
int MMap<K, V>::size() const
{
    return mmap.size();
}

template <class K, class V>
bool MMap<K, V>::empty() const
{
    return mmap.empty();
}

template <class K, class V>
const std::vector<V> &MMap<K, V>::operator[](const K &key) const
{
    return mmap.get(key).value_list;
}

template <class K, class V>
std::vector<V> &MMap<K, V>::operator[](const K &key)
{
    return mmap.get(key).value_list;
}

template <class K, class V>
std::vector<V> &MMap<K, V>::at(const K &key)
{
    return mmap.get(key).value_list;
}

template <class K, class V>
const std::vector<V> &MMap<K, V>::at(const K &key) const
{
    return mmap.get(key).value_list;
}

template <class K, class V>
void MMap<K, V>::insert(const K &k, const V &v)
{
    mmap.insert(MPair<K, V>(k, v));
}

template <class K, class V>
void MMap<K, V>::erase(const K &key)
{
    mmap.remove(key);
}

template <class K, class V>
void MMap<K, V>::clear()
{
    mmap.clear_tree();
}

template <class K, class V>
bool MMap<K, V>::contains(const K &key) const
{
    return mmap.contains(key);
}

template <class K, class V>
std::vector<V> &MMap<K, V>::get(const K &key)
{
    return mmap.get(key).value_list;
}

template <class K, class V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K &key)
{
    return Iterator(mmap.find(key));
}

template <class K, class V>
int MMap<K, V>::count(const K &key)
{
    return mmap.get_existing(key).value_list.size();
}

template <class K, class V>
bool MMap<K, V>::is_valid()
{
    return mmap.is_valid();
}
#endif