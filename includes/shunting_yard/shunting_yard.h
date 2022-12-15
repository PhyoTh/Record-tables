#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <cstring>
#include "../token/token.h"
#include "../token/constants.h"
#include "../token/logic.h"
#include "../token/relation.h"
#include "../token/string.h"
#include "../token/leftparen.h"
#include "../token/rightparen.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"

class ShuntingYard
{
public:
    ShuntingYard() {}
    ShuntingYard(const vector<string> &infix) // infix input(can be done with = operator for both)
    {
        que = vecToQueue(infix);
    }
    void infix(const vector<string> &infix) // infix input
    {
        que = vecToQueue(infix);
    }

    Queue<Token *> vecToQueue(const vector<string> &infix)
    {
        Queue<Token *> q;
        for (int i = 0; i < infix.size(); ++i)
        {
            if (infix[i] == "(")
                q.push(new LeftParen());
            else if (infix[i] == ")")
                q.push(new RightParen());
            else if (infix[i] == "=" || infix[i] == ">" || infix[i] == "<" || infix[i] == ">=" || infix[i] == "<=")
                q.push(new Relational(infix[i]));
            else if (infix[i] == "and" || infix[i] == "or")
                q.push(new Logical(infix[i]));
            else
                q.push(new TokenStr(infix[i]));
        }
        return q;
    }

    Queue<Token *> postfix() // return postfix que
    {
        cout << "this is your infix que" << endl
             << que << endl;
        Stack<Token *> stack;
        Queue<Token *> res;

        while (que.size() != 0)
        {
            switch (que.front()->TypeOf())
            {
            case STRING:
            {
                res.push(que.pop());
                break;
            }
            case RELATION:
            {
                while (!stack.empty() && stack.top()->TypeOf() == RELATION)
                    res.push(stack.pop());
                stack.push(que.pop());
                break;
            }
            case LOGIC:
            {
                while (!stack.empty() && (stack.top()->TypeOf() == RELATION || stack.top()->TypeOf() == LOGIC))
                    res.push(stack.pop());
                stack.push(que.pop());
                break;
            }
            case LEFTPAREN:
            {
                que.pop();
                stack.push(new LeftParen());
                break;
            }
            case RIGHTPAREN:
            {
                que.pop();
                while (stack.top()->TypeOf() != LEFTPAREN)
                {
                    if (stack.top()->TypeOf() == RELATION || stack.top()->TypeOf() == LOGIC)
                        res.push(stack.pop());
                }
                stack.pop();
                break;
            }
            }
        }
        while (!stack.empty())
        {
            if (stack.top()->TypeOf() == RELATION || stack.top()->TypeOf() == LOGIC)
                res.push(stack.pop());
        }
        cout << "this is your post fix que" << endl
             << res << endl;
        return res;
    }

    Queue<Token *>
    postfix(const vector<string> &infix) // return postfix que
    {
        ShuntingYard sy(infix);
        return sy.postfix();
    }

private:
    Queue<Token *> que;
};
#endif