#ifndef LOGIC_H
#define LOGIC_H

#include "token.h"
using namespace std;

class Logical : public Token
{
public:
    Logical(){};
    Logical(string logic) { _logic = logic; }
    void Print(ostream &outs = cout) const
    {
        outs << "Logical:" << _logic;
    }
    string get_string()
    {
        return _logic;
    }
    TOKEN_TYPES TypeOf() { return LOGIC; }

private:
    string _logic;
};
#endif