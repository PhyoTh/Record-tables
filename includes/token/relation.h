#ifndef RELATIONAL_H
#define RELATIONAL_H

#include "token.h"
using namespace std;

class Relational : public Token
{
public:
    Relational(){};
    Relational(string relation) { _relation = relation; }
    void Print(ostream &outs = cout) const
    {
        outs << "Relational:" << _relation;
    }
    string get_string()
    {
        return _relation;
    }
    TOKEN_TYPES TypeOf() { return RELATION; }

private:
    string _relation;
};
#endif