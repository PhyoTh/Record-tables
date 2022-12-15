#ifndef LEFTPAREN_H
#define LEFTPAREN_H

#include "token.h"

class LeftParen : public Token
{
public:
    LeftParen() {}
    void Print(ostream &outs = cout) const
    {
        outs << "LeftParen:(";
    }
    TOKEN_TYPES TypeOf() { return LEFTPAREN; }
};
#endif