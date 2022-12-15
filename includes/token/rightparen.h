#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H

#include "token.h"

class RightParen : public Token
{
public:
    RightParen() {}
    void Print(ostream &outs = cout) const
    {
        outs << "RightParen:)";
    }
    TOKEN_TYPES TypeOf() { return RIGHTPAREN; }
};
#endif