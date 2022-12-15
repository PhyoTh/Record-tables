#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "btree_array_funcs.h"
#include <cassert>

template <class T>
class BPlusTree
{
public:
    /////////////////////////////////////////
    /////////////   ITERATOR   //////////////
    /////////////////////////////////////////
    class Iterator
    {
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T> *_it = NULL, int _key_ptr = 0) : it(_it), key_ptr(_key_ptr) {}

        T operator*()
        {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used) // it++
        {
            Iterator hold(*this);
            this->operator++();
            return hold;
        }
        Iterator operator++() //++it
        {
            assert(it);
            if (key_ptr < it->data_count - 1)
                key_ptr++;
            else
            {
                key_ptr = 0;
                it = it->next;
            }
            return *this;
        }

        friend bool
        operator==(const Iterator &lhs, const Iterator &rhs)
        {
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }
        friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
        {
            return !(lhs == rhs);
        }
        void print_Iterator()
        {
            if (it)
            {
                cout << "iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else
            {
                cout << "iterator: NULL, key_ptr: " << key_ptr << endl;
            }
        }
        bool is_null() { return !it; }
        void info()
        {
            cout << endl
                 << "Iterator info:" << endl;
            cout << "key_ptr: " << key_ptr << endl;
            cout << "it: " << *it << endl;
        }

    private:
        BPlusTree<T> *it;
        int key_ptr;
    };
    /////////////////////////////////////////
    /////////////  BPLUS TREE   /////////////
    /////////////////////////////////////////

    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);

    // big three:
    BPlusTree(const BPlusTree<T> &other);
    ~BPlusTree();
    BPlusTree<T> &operator=(const BPlusTree<T> &RHS);

    void copy_tree(const BPlusTree<T> &other); // copy other into this object
    void copy_tree(const BPlusTree<T> &other, BPlusTree<T> *&last_node);

    void insert(const T &entry); // insert entry into the tree
    void remove(const T &entry); // remove entry from the tree
    void clear_tree();           // clear this object (delete all nodes etc.)

    bool contains(const T &entry) const; // true if entry can be found
    T &get(const T &entry);              // return a reference to entry
    // if not there insert
    const T &get(const T &entry) const; // return a reference to entry
    T &get_existing(const T &entry);    // return a reference to entry
    Iterator find(const T &key)
    // return an iterator to this key.
    //      NULL if not there.
    {
        int i = first_ge(data, data_count, key);
        if (i < data_count && data[i] == key)
        {
            if (is_leaf())
                return Iterator(this, i);
            else
                return subset[i + 1]->find(key);
        }
        else if (is_leaf())
            return Iterator(nullptr);
        else
            return subset[i]->find(key);
    }

    Iterator lower_bound(const T &key)
    // return first that goes NOT BEFORE
    //  key entry or next if does not
    //  exist: >= entry
    {
        int i = first_ge(data, data_count, key);
        if (is_leaf())
        {
            if (i < data_count && !(key < data[i]))
                return Iterator(this, i);
            else
            {
                for (; i < data_count; i++)
                {
                    if (!(data[i] < key))
                        return Iterator(this, i);
                }
            }
            if (next)
                return Iterator(next, 0);
            return Iterator();
        }
        else
        {
            if (i < data_count && !(key < data[i]))
                return subset[i + 1]->lower_bound(key);
            return subset[i]->lower_bound(key);
        }
    }
    Iterator upper_bound(const T &key)
    // return first that goes AFTER key
    // exist or not, the next entry  >entry
    {
        int i = first_ge(data, data_count, key);
        if (is_leaf())
        {
            if (i < data_count && key < data[i])
                return Iterator(this, i);
            else
            {
                for (; i < data_count; i++)
                {
                    if (data[i] > key)
                        return Iterator(this, i);
                }
            }
            if (next)
                return Iterator(next, 0);
            return Iterator();
        }
        else
        {
            if (i < data_count && !(key < data[i]))
                return subset[i + 1]->upper_bound(key);
            return subset[i]->upper_bound(key);
        }
    }

    int size() const; // count the number of elements

    bool empty() const { return data_count == 0 && child_count == 0; } // true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs = cout) const
    {
        for (int i = data_count; i >= 0; --i)
        {
            if (i == data_count - 1)
            {
                if (next)
                    cout << "| " << string(8 * level, ' ') << "^" << endl;
                outs << "| " << string(8 * level, ' ') << "⎴" << endl;
            }
            if (i < data_count)
                outs << "| " << string(8 * level, ' ') << data[i] << endl;
            if (i == 0)
                outs << "| " << string(8 * level, ' ') << "⎵" << endl;
            if (!is_leaf())
                subset[i]->print_tree(level + 1);
        }
    }
    friend ostream &operator<<(ostream &outs,
                               const BPlusTree<T> &print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();
    string in_order();
    string pre_order();
    string post_order();

    Iterator begin()
    {
        return Iterator(get_smallest_node());
    }
    Iterator end()
    {
        return Iterator(NULL);
    }
    ostream &list_keys(Iterator from = NULL, Iterator to = NULL)
    {
        if (from == NULL)
            from = begin();
        if (to == NULL)
            to = end();
        for (Iterator it = from; it != to; it++)
            cout << *it << " ";
        return cout;
    }

    /////////////////////////////////////////
    ///////////////  PRIVATE  ///////////////
    /////////////////////////////////////////
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                   // true if duplicate keys are allowed
    int data_count;                 // number of data elements
    T data[MAXIMUM + 1];            // holds the keys
    int child_count;                // number of children
    BPlusTree *subset[MAXIMUM + 2]; // subtrees
    BPlusTree *next;

    bool is_leaf() const { return child_count == 0; } // true if this is a leaf node

    T *find_ptr(const T &entry);
    // return a pointer to this key.
    //  NULL if not there.

    // insert element functions
    void loose_insert(const T &entry);
    // allows MAXIMUM+1 data elements in
    //    the root

    void fix_excess(int i); // fix excess in child i

    // remove element functions:
    void loose_remove(const T &entry);
    // allows MINIMUM-1 data elements
    //   in the root

    BPlusTree<T> *fix_shortage(int i);
    // fix shortage in child i
    // and return the smallest key in this subtree

    BPlusTree<T> *get_smallest_node()
    {
        if (!is_leaf())
            return subset[0]->get_smallest_node();
        else
            return this;
    }

    void get_smallest(T &entry) // entry := leftmost leaf
    {
        if (!is_leaf())
            subset[0]->get_smallest(entry);
        else
            entry = data[0];
    }

    void get_biggest(T &entry) // entry := rightmost leaf
    {
        if (!is_leaf())
            subset[child_count - 1]->get_biggest(entry);
        else
            entry = data[data_count - 1];
    }

    void transfer_left(int i);
    // transfer one element LEFT from child i

    void transfer_right(int i);
    // transfer one element RIGHT from child i

    BPlusTree<T> *merge_with_next_subset(int i);
    // merge subset i with  i+1
};

///////////////////////////////////////////////
//////////Big Three and constructors///////////
///////////////////////////////////////////////
template <class T>
BPlusTree<T>::BPlusTree(bool dups)
{
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    next = nullptr;
}

template <class T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups)
{
    dups_ok = dups;
    child_count = 0;
    data_count = 0;
    next = nullptr;
    for (int i = 0; i < size; i++)
    {
        insert(a[i]);
        // print_tree();
    }
}

template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T> &other)
{
    copy_tree(other);
}

template <class T>
BPlusTree<T>::~BPlusTree()
{
    clear_tree();
    // cout << "destructor get called" << endl;
}

template <class T>
BPlusTree<T> &BPlusTree<T>::operator=(const BPlusTree<T> &RHS)
{
    if (this == &RHS)
        return *this;
    clear_tree();
    copy_tree(RHS);
    return *this;
}

template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other)
{
    BPlusTree<T> *last_leaf = nullptr;
    copy_tree(other, last_leaf);
}

template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other, BPlusTree<T> *&last_node)
{
    dups_ok = other.dups_ok;
    copy_array(data, other.data, data_count, other.data_count);
    child_count = other.child_count;
    next = nullptr;

    if (!other.is_leaf())
    {
        for (int i = 0; i < child_count; ++i)
        {
            subset[i] = new BPlusTree<T>();
            subset[i]->copy_tree(*other.subset[i], last_node);
        }
    }
    else
    {
        if (last_node)
            last_node->next = this;
        last_node = this;
    }
}

template <class T>
void BPlusTree<T>::clear_tree()
{
    data_count = 0;
    next = nullptr;
    for (int i = 0; i < child_count; ++i)
    {
        subset[i]->clear_tree();
        delete subset[i];
    }
    child_count = 0;
}
///////////////////////////////////////////
//////////////get find contains////////////
///////////////////////////////////////////
template <class T>
T *BPlusTree<T>::find_ptr(const T &entry)
{
    int i = first_ge(data, data_count, entry);
    if (i < data_count && data[i] == entry)
    {
        if (is_leaf())
            return &data[i];
        else
            return subset[i + 1]->find_ptr(entry);
    }
    else if (is_leaf())
        return nullptr;
    else
        return subset[i]->find_ptr(entry);
}

template <class T>
bool BPlusTree<T>::contains(const T &entry) const
{
    int i = first_ge(data, data_count, entry);
    if (i < data_count && data[i] == entry)
    {
        if (is_leaf())
            return true;
        else
            return subset[i + 1]->contains(entry);
    }
    else if (is_leaf())
        return false;
    else
        return subset[i]->contains(entry);
}

template <class T>
T &BPlusTree<T>::get(const T &entry) // return a reference to entry
{
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);
}

template <class T>
const T &BPlusTree<T>::get(const T &entry) const // return a reference to entry
{
    return get_existing();
}

template <class T>
T &BPlusTree<T>::get_existing(const T &entry) // return a reference to entry
{
    assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);
    if (is_leaf())
        if (found)
        {
            return data[i];
        }
        else
        {
            if (debug)
                cout << "get_existing was called with nonexistent entry" << endl;
            assert(found);
        }
    if (found) // inner node
        return subset[i + 1]->get_existing(entry);
    // or just return true?
    else // not found yet...
        return subset[i]->get_existing(entry);
}

////////////////////////////////////////////////////
///////////////is valid size orders/////////////////
////////////////////////////////////////////////////
template <class T>
int BPlusTree<T>::size() const
{
    int count = data_count;
    for (int i = 0; i < child_count; ++i)
        count += subset[i]->size();
    return count;
}

template <class T>
bool BPlusTree<T>::is_valid() //
{
    if (!is_leaf() && (data_count < MINIMUM || child_count != data_count + 1))
    {
        cout << "data_count is less than minimum or child_count is not equal to data_count + 1" << endl;
        return false;
    }
    if (data_count > MAXIMUM || child_count > MAXIMUM + 1)
    {
        cout << "data_count or child count is not valid" << endl;
        return false;
    }
    for (int i = 0; i < data_count; ++i)
    {
        if (i + 1 < data_count && data[i] > data[i + 1])
        {
            cout << "data is not valid" << endl;
            return false;
        }
    }
    for (int i = 0; i < child_count; ++i)
    {
        if (!subset[i]->is_valid())
            return false;
    }
    return true;
}

template <class T>
std::string BPlusTree<T>::in_order()
{
    string result;
    for (int i = 0; i <= data_count; ++i)
    {
        if (!is_leaf())
            result += subset[i]->in_order();

        if (i < data_count)
            result += to_string(data[i]) + "|";
    }
    return result;
}

template <class T>
std::string BPlusTree<T>::pre_order()
{
    string result;
    for (int i = 0; i <= data_count; ++i)
    {
        if (i < data_count)
            result += to_string(data[i]) + "|";

        if (!is_leaf())
            result += subset[i]->pre_order();
    }
    return result;
}

template <class T>
std::string BPlusTree<T>::post_order()
{
    string result;
    for (int i = 0; i <= data_count; ++i)
    {
        if (!is_leaf())
            result += subset[i]->post_order();
        if (i - 1 >= 0)
            result += to_string(data[i - 1]) + "|";
    }
    return result;
}

///////////////////////////////////////////////////
///////////////////insert//////////////////////////
///////////////////////////////////////////////////
template <class T>
void BPlusTree<T>::insert(const T &entry)
{
    loose_insert(entry);
    if (data_count == MAXIMUM + 1)
    {
        BPlusTree<T> *temp = new BPlusTree<T>(dups_ok);
        copy_array(temp->data, data, temp->data_count, data_count);
        copy_array(temp->subset, subset, temp->child_count, child_count);
        data_count = 0;
        child_count = 1;
        subset[0] = temp;
        temp = nullptr;
        fix_excess(0);
    }
}

template <class T>
void BPlusTree<T>::loose_insert(const T &entry)
{
    int i = first_ge(data, data_count, entry);
    if (i < data_count && data[i] == entry)
    {
        if (!is_leaf())
        {
            subset[i + 1]->loose_insert(entry);
            if (subset[i + 1]->data_count > MAXIMUM)
                fix_excess(i + 1);
        }
        else
        {
            if (dups_ok)
                data[i] = data[i] + entry;
        }
    }
    else if (is_leaf())
    {
        insert_item(data, i, data_count, entry);
    }
    else
    {
        subset[i]->loose_insert(entry);
        if (subset[i]->data_count > MAXIMUM)
            fix_excess(i);
    }
}

template <class T>
void BPlusTree<T>::fix_excess(int i)
{
    BPlusTree<T> *temp = new BPlusTree<T>(dups_ok);

    split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);
    if (!subset[i]->is_leaf())
        split(subset[i]->subset, subset[i]->child_count, temp->subset, temp->child_count);

    T item = T();
    detach_item(subset[i]->data, subset[i]->data_count, item);

    insert_item(data, i, data_count, item);
    insert_item(subset, i + 1, child_count, temp);
    temp = nullptr;

    if (subset[i]->is_leaf())
    {
        insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, item);
        subset[i + 1]->next = subset[i]->next;
        subset[i]->next = subset[i + 1];
    }
}
//////////////////////////////////////////////////
//////////////////remove//////////////////////////
//////////////////////////////////////////////////
template <class T>
void BPlusTree<T>::remove(const T &entry)
{
    // cout << "remove gets called" << endl;
    loose_remove(entry);
    if (data_count == 0 && child_count == 1)
    {
        cout << "shrinking occurs" << endl;
        BPlusTree<T> *temp = subset[0];
        copy_array(data, temp->data, data_count, temp->data_count);
        copy_array(subset, temp->subset, child_count, temp->child_count);
        temp->child_count = 0;
        temp->data_count = 0;
        delete temp;
    }
}

template <class T>
inline void BPlusTree<T>::loose_remove(const T &entry)
{
    // cout << "loose_remove" << endl;
    int i = first_ge(data, data_count, entry);
    // cout << "i = " << i << endl;
    if (is_leaf())
    {
        // cout << "leaf" << endl;
        // print_array(data, data_count);
        if (i < data_count && data[i] == entry)
        {
            // cout << "found and is leaf()" << endl;
            T temp = T();
            delete_item(data, i, data_count, temp);
        }
    }
    else
    {
        // cout << "not leaf" << endl;
        if (i < data_count && data[i] == entry)
        {
            // cout << "found but in the middle goes to subset i+1" << endl;
            subset[i + 1]->loose_remove(entry);
            if (subset[i + 1]->data_count < MINIMUM)
            {
                // cout << "loose remove calls fix_shortage i + 1" << endl;
                fix_shortage(i + 1);
                // cout << "after fix_shortage i + 1" << endl;
                // print_tree();
            }
            if (data_count > 0 && data[i] == entry)
            {
                cout << "lee" << endl;
                subset[i + 1]->get_smallest(data[i]);
            }
            else if (i + 1 < child_count && subset[i + 1]->data[0] == entry)
            {
                cout << "lee1" << endl;
                subset[i + 1]->subset[1]->get_smallest(subset[i + 1]->data[0]);
            }
            else if (subset[i]->data[subset[i]->data_count - 1] == entry)
            {
                cout << "lee2" << endl;
                subset[i]->subset[subset[i]->child_count - 1]->get_smallest(subset[i]->data[subset[i]->data_count - 1]);
            }
        }
        else
        {
            subset[i]->loose_remove(entry);
            if (subset[i]->data_count < MINIMUM)
            {
                // cout << "loose remove calls fix_shortage i" << endl;
                fix_shortage(i);
                // cout << "after fix_shortage i" << endl;
                // print_tree();
            }
        }
    }
}

template <class T>
inline BPlusTree<T> *BPlusTree<T>::fix_shortage(int i)
{
    // cout << "fix shortage gets called" << endl;
    // print_tree();
    // cout << "child number = " << i << endl;
    if (i > 0 && i < child_count - 1)
    {
        // cout << "a lel mhr" << endl;
        if (subset[i + 1]->data_count > MINIMUM)
        {
            transfer_left(i);
        }
        else if (subset[i - 1]->data_count > MINIMUM)
        {
            transfer_right(i);
        }
        else
        {
            merge_with_next_subset(i - 1);
        }
    }
    else if (i == 0)
    {
        // cout << "shae sone mhar" << endl;
        if (subset[i + 1]->data_count > MINIMUM)
        {
            transfer_left(i);
        }
        else
        {
            merge_with_next_subset(i);
        }
    }
    else
    {
        // cout << "nout sone mhar" << endl;
        if (subset[i - 1]->data_count > MINIMUM)
        {
            transfer_right(i);
        }
        else
        {
            merge_with_next_subset(i - 1);
        }
    }
    // cout << "fix_shortage done" << endl;
    return subset[i];
}

template <class T>
void BPlusTree<T>::transfer_left(int i)
{
    // cout << "transfer left gets called" << endl;
    T temp = T();
    if (!subset[i]->is_leaf())
    {
        delete_item(data, i, data_count, temp);
        attach_item(subset[i]->data, subset[i]->data_count, temp);

        delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
        insert_item(data, i, data_count, temp);

        BPlusTree<T> *temp1 = new BPlusTree<T>(dups_ok);
        if (!subset[i + 1]->is_leaf())
        {
            delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp1);
            attach_item(subset[i]->subset, subset[i]->child_count, temp1);
        }
    }
    else
    {
        delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
        attach_item(subset[i]->data, subset[i]->data_count, temp);
        data[i] = subset[i + 1]->data[0];
    }
}

template <class T>
inline void BPlusTree<T>::transfer_right(int i)
{
    // cout << "transfer right gets called" << endl;
    T temp = T();
    if (!subset[i]->is_leaf())
    {
        delete_item(data, i - 1, data_count, temp);
        insert_item(subset[i]->data, 0, subset[i]->data_count, temp);

        detach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
        insert_item(data, i - 1, data_count, temp);

        BPlusTree<T> *temp1 = new BPlusTree<T>(dups_ok);
        if (!subset[i - 1]->is_leaf())
        {
            detach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp1);
            insert_item(subset[i]->subset, 0, subset[i]->child_count, temp1);
        }
    }
    else
    {
        detach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
        insert_item(subset[i]->data, 0, subset[i]->data_count, temp);
        data[i - 1] = subset[i]->data[0];
    }
}

template <class T>
BPlusTree<T> *BPlusTree<T>::merge_with_next_subset(int i)
{
    // cout << "merge gets called" << endl;
    T temp = T();
    delete_item(data, i, data_count, temp);
    if (!subset[i]->is_leaf())
        attach_item(subset[i]->data, subset[i]->data_count, temp);

    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);

    BPlusTree<T> *temp1;
    delete_item(subset, i + 1, child_count, temp1);
    if (subset[i]->is_leaf())
        subset[i]->next = temp1->next;
    delete temp1;
    return subset[i];
}
#endif