#ifndef MAP_H
#define MAP_H

#include "bplustree.h"
#include "set_class.h"

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V>> map_base;
    class Iterator
    {
    public:
        friend class Map;
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
        Pair<K, V> operator*()
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

    Map();
    //  Iterators
    Iterator begin();
    Iterator end();
    Iterator lower_bound(const K &key);
    Iterator upper_bound(const K &key);

    //  Capacity
    int size() const;
    bool empty() const;

    //  Element Access
    V &operator[](const K &key);
    V &at(const K &key);
    const V &at(const K &key) const;

    //  Modifiers
    void insert(const K &k, const V &v);
    void erase(const K &key);
    void clear();
    V get(const K &key);

    //  Operations:
    Iterator find(const K &key);
    bool contains(const Pair<K, V> &target) const;

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid() { return map.is_valid(); }

    friend ostream &operator<<(ostream &outs, const Map<K, V> &print_me)
    {
        outs << print_me.map << endl;
        return outs;
    }

private:
    int key_count;
    BPlusTree<Pair<K, V>> map;
};

template <class K, class V>
Map<K, V>::Map()
{
    key_count = 0;
    map = BPlusTree<Pair<K, V>>(true);
}

template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::begin()
{
    return Iterator(map.begin());
}

template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
    return Iterator(map.end());
}

template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::lower_bound(const K &key)
{
    return Iterator(map.lower_bound(key));
}

template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::upper_bound(const K &key)
{
    return Iterator(map.upper_bound(key));
}

template <class K, class V>
int Map<K, V>::size() const
{
    return map.size();
}

template <class K, class V>
bool Map<K, V>::empty() const
{
    return map.empty();
}

template <class K, class V>
V &Map<K, V>::operator[](const K &key)
{
    return at(key);
}

template <class K, class V>
V &Map<K, V>::at(const K &key)
{
    return map.get(key).value;
}

template <class K, class V>
const V &Map<K, V>::at(const K &key) const
{
    return map.get(key).value;
}

template <class K, class V>
void Map<K, V>::insert(const K &k, const V &v)
{
    map.insert(Pair<K, V>(k, v));
}

template <class K, class V>
void Map<K, V>::erase(const K &key)
{
    map.remove(key);
}

template <class K, class V>
void Map<K, V>::clear()
{
    map.clear_tree();
    key_count = 0;
}

template <class K, class V>
V Map<K, V>::get(const K &key)
{
    return map.get(key).value;
}

template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::find(const K &key)
{
    return Iterator(map.find(key));
}

template <class K, class V>
bool Map<K, V>::contains(const Pair<K, V> &target) const
{
    return map.contains(target);
}
#endif