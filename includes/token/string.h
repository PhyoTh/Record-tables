#ifndef STRING_H
#define STRING_H

#include "token.h"
using namespace std;

class TokenStr : public Token
{
public:
    TokenStr(){};
    TokenStr(string tokenstr) { _tokenstr = tokenstr; }
    void Print(ostream &outs = cout) const
    {
        outs << "TokenStr:" << _tokenstr;
    }
    string get_string()
    {
        return _tokenstr;
    }
    TOKEN_TYPES TypeOf() { return STRING; }

private:
    string _tokenstr;
};
#endif