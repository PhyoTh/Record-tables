#ifndef MYSTACK_H
#define MYSTACK_H

#include <iostream>
#include <iomanip>
#include <assert.h>
#include "../node/node.h"
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

using namespace std;

template <typename T>
class Stack
{
public:
    class Iterator
    {
    public:
        friend class Stack;                // give access to list to access _ptr
        Iterator() { _ptr = NULL; }        // default ctor
        Iterator(node<T> *p) { _ptr = p; } // Point Iterator to where
                                           //   p is pointing to

        T &operator*() // dereference operator
        {
            assert(_ptr);
            return _ptr->_item;
        }

        T *operator->() // member access operator
        {
            return &(operator*());
        }

        bool is_null() // true if _ptr is NULL
        {
            return _ptr == nullptr;
        }

        friend bool operator!=(const Iterator &left,
                               const Iterator &right) // true if left != right
        {
            return left._ptr != right._ptr;
        }

        friend bool operator==(const Iterator &left,
                               const Iterator &right) // true if left == right
        {
            return left._ptr == right._ptr;
        }

        Iterator &operator++() // member operator: ++it; or ++it = new_value
        {
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator &it, int unused) // friend operator: it++
        {
            Iterator hold = it;
            it._ptr = it._ptr->_next;
            return hold;
        }

    private:
        node<T> *_ptr; // pointer being encapsulated
    };

    Stack();
    Stack(const Stack<T> &copyMe);
    ~Stack();
    Stack<T> &operator=(const Stack<T> &RHS);
    T top();
    bool empty();
    void push(T item);
    T pop();
    void print_pointers();
    template <typename TT>
    friend ostream &operator<<(ostream &outs,
                               const Stack<TT> &printMe)
    {
        outs << "Stack:Head->";
        node<TT> *ptr = printMe._top;
        while (ptr != nullptr)
        {
            outs << "<-[" << ptr->_item << "]-> ";
            ptr = ptr->_next;
        }
        if (ptr == nullptr)
        {
            outs << "|||" << endl;
        }
        return outs;
    }

    Iterator begin() const // Iterator to the head node
    {
        return Iterator(_top);
    }
    Iterator end() const // Iterator to NULL
    {
        return Iterator(nullptr);
    }
    int size() const { return _size; }

private:
    node<T> *_top;
    int _size;
};

template <typename T>
Stack<T>::Stack()
{
    _top = nullptr;
    _size = 0;
}

template <typename T>
Stack<T>::Stack(const Stack<T> &copyMe)
{
    _top = nullptr;
    _top = _copy_list(copyMe._top);
    _size = copyMe._size;
}

template <typename T>
Stack<T>::~Stack()
{
    _clear_list(_top);
    _size = 0;
}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &RHS)
{
    if (this == &RHS)
        return *this;
    _clear_list(_top);
    _top = _copy_list(RHS._top);
    _size = RHS._size;
    return *this;
}

template <typename T>
T Stack<T>::top()
{
    assert(_top != nullptr);
    return _top->_item;
}

template <typename T>
bool Stack<T>::empty()
{
    return _top == nullptr && _size == 0;
}

template <typename T>
void Stack<T>::push(T item)
{
    _insert_head(_top, item);
    _size++;
}

template <typename T>
T Stack<T>::pop()
{
    assert(_top != nullptr);
    _size--;
    return _delete_node(_top, _top);
}

template <typename T>
void Stack<T>::print_pointers()
{
    node<T> *walker = _top;
    cout << "H->";
    while (walker != nullptr)
    {
        cout << *(walker->_item);
        walker = walker->_next;
    }
    cout << "|||";
}

#endif