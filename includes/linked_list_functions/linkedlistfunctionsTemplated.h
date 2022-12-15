#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H

#include <iostream>
#include <iomanip>
#include <assert.h>
#include "../node/node.h"

using namespace std;

// Linked List General Functions:
template <typename T>
void _print_list(node<T> *head)
{
    cout << "Head->";
    node<T> *ptr = head;
    while (ptr != nullptr)
    {
        cout << "<-[" << ptr->_item << "]-> ";
        ptr = ptr->_next;
    }
    if (ptr == nullptr)
    {
        cout << "|||" << endl;
    }
}

// recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head)
{
    node<T> *ptr = head;
    if (ptr != nullptr)
    {
        _print_list_backwards(ptr->_next);
        cout << "<-[" << ptr->_item << "]-> ";
        if (ptr->_prev == nullptr)
        {
            cout << "Head" << endl;
        }
    }
    else
    {
        cout << "|||";
    }
}

// return ptr to key or NULL
template <typename T>
node<T> *_search_list(node<T> *head, T key)
{
    node<T> *ptr = head;
    while (ptr != nullptr)
    {
        if (ptr->_item == key)
        {
            return ptr;
        }
        ptr = ptr->_next;
    }
    return nullptr;
}

template <typename T>
node<T> *_insert_head(node<T> *&head, T insert_this)
{
    node<T> *ptr = new node<T>(insert_this);
    ptr->_next = head;
    if (head == nullptr)
    {
        head = ptr;
        return head;
    }
    head->_prev = ptr;
    head = ptr;
    return head;
}

// insert after ptr: insert head if marker null
template <typename T>
node<T> *_insert_after(node<T> *&head, node<T> *after_this, T insert_this)
{
    if (after_this == nullptr || head == nullptr)
    {
        _insert_head(head, insert_this);
        return head;
    }
    node<T> *ptr = new node<T>(insert_this);
    ptr->_next = after_this->_next;
    ptr->_prev = after_this;
    if (after_this->_next != nullptr)
    {
        after_this->_next->_prev = ptr;
    }
    after_this->_next = ptr;
    return ptr;
}

// insert before ptr: insert head if marker null
template <typename T>
node<T> *_insert_before(node<T> *&head, node<T> *before_this, T insert_this)
{
    if (before_this == nullptr || head == nullptr)
    {
        _insert_head(head, insert_this);
        return head;
    }
    return _insert_after(head, before_this->_prev, insert_this);
}

// ptr to previous node
template <typename T>
node<T> *_previous_node(node<T> *prev_to_this)
{
    assert(prev_to_this != nullptr);
    return prev_to_this->_prev;
}

// delete, return item
template <typename T>
T _delete_node(node<T> *&head, node<T> *delete_this)
{
    if (head == nullptr)
    {
        return T();
    }
    T x = delete_this->_item;
    if (delete_this == head)
    {
        head = head->_next;
        if (head != nullptr)
            head->_prev = nullptr;
    }
    else if (delete_this->_next == nullptr)
    {
        delete_this->_prev->_next = nullptr;
    }
    else
    {
        delete_this->_prev->_next = delete_this->_next;
        delete_this->_next->_prev = _previous_node(delete_this);
    }
    delete delete_this;
    return x;
}

// duplicate the list...
template <typename T>
node<T> *_copy_list(node<T> *head)
{
    if (head == nullptr)
    {
        return nullptr;
    }
    node<T> *ptr = nullptr;
    _insert_head(ptr, head->_item);
    node<T> *walker1 = head->_next;
    node<T> *walker2 = ptr;
    while (walker1 != nullptr)
    {
        _insert_after(ptr, walker2, walker1->_item);
        walker1 = walker1->_next;
        walker2 = walker2->_next;
    }
    return ptr;
}

// duplicate the list, return pointer to last node in dest...
//      use this function in your queue big three
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src)
{
    _clear_list(dest);
    dest = _copy_list(src);
    if (dest == nullptr)
    {
        return nullptr;
    }
    node<T> *walker = dest;
    while (walker->_next != nullptr)
    {
        walker = walker->_next;
    }
    return walker;
}

// delete all the nodes
template <typename T>
void _clear_list(node<T> *&head)
{
    while (head != nullptr)
    {
        node<T> *ptr = head;
        head = head->_next;
        delete ptr;
    }
}

//_item at this position
template <typename T>
T &_at(node<T> *head, int pos)
{
    assert(pos >= 0);
    node<T> *walker = head;
    for (int i = 0; i < pos; i++)
    {
        walker = walker->_next;
    }
    assert(walker != nullptr);
    return walker->_item;
}

#endif
