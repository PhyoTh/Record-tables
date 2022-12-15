#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utilities.h"

using namespace std;

class FileRecord
{
public:
    // when you construct a FileRecord, it's either empty or it
    //   contains a word
    FileRecord()
    {
        // for (int i = 0; i <= MAX; ++i)
        //     _record[i][0] = '\0';

        _recno = -1;
        _size = 0;
        //['\0'|  | G | a | r | b | a   |  g  |  e |    ]
    }

    FileRecord(vector<string> v)
    {
        _size = v.size();
        for (int i = 0; i < _size; ++i)
        {
            strncpy(_record[i], v.at(i).c_str(), MAX);
        }
        _record[v.size()][0] = '\0';
    }

    long get_recno() const
    {
        return _recno;
    }

    int get_size() const
    {
        return _size;
    }

    vector<string> get_vector() const
    {
        vector<string> result;
        for (int i = 0; i < _size; ++i)
        {
            result.push_back(_record[i]);
        }
        return result;
    }

    void print()
    {
        for (int i = 0; i < _size; ++i)
        {
            cout << setw(10) << _record[i] << endl;
        }
    }

    long write(fstream &outs);           // returns the record number
    long read(fstream &ins, long recno); // returns the number of bytes
                                         //       read = MAX, or zero if
                                         //       read passed the end of file

    friend ostream &operator<<(ostream &outs, const FileRecord &r);
    char _record[101][101]; // NULL char

private:
    int MAX = 100;
    long _recno;
    int _size;
};

#endif