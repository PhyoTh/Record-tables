#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"
#include "../bplustree/btree_array_funcs.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"
#include "../stl_utils/vector_utilities.h"
#include "../shunting_yard/shunting_yard.h"
#include "typedefs.h"

class Table
{
public:
    /// @brief tester will call this
    /// @return the inserted txt file and the table infos
    Table make_table();

    Table()
    {
        _serial = 0;
        _tableName = "";
        _empty = true;
        _last_record = -1;
    }

    Table(const string &filename, const vectorstr &field_names)
    // create a table
    // filename is the filename to open file.txt
    // field_name is for the number of columns in the table or the rows in the record
    {
        _serial = 0;
        _tableName = filename;
        set_fields(field_names);
        _empty = true;
        _last_record = -1;
        fstream f;

        // creating field txt file
        open_fileW(f, (_tableName + ".fields").c_str());
        FileRecord r;
        r = FileRecord(_fieldNames);
        r.write(f);
        f.close();

        // creating table file
        open_fileW(f, (_tableName + ".tbl").c_str());
        f.close();
    }

    Table(const string filename)
    // this will find the filename and open it for reading and writing
    {
        _serial = 0;
        _tableName = filename;
        fstream f;

        open_fileRW(f, (_tableName + ".fields").c_str());

        FileRecord r;
        r.read(f, 0);
        set_fields(r.get_vector());
        f.close();

        open_fileRW(f, (_tableName + ".tbl").c_str());
        int record = -1;
        long pos = r.read(f, record + 1);
        while (pos > 0)
        {
            reindex(r.get_vector(), record + 1);
            ++record;
            pos = r.read(f, record + 1);
        }
        _last_record = record;
        if (_last_record >= 0)
            _empty = false;
        f.close();
    }

    ~Table()
    {
        for (long i = _serial; i > 0; --i)
        {
            string str = _tableName + to_string(i);
            std::remove((str + ".fields").c_str());
            std::remove((str + ".tbl").c_str());
        }
    }

    void insert_into(vectorstr record)
    // this will insert a record
    {
        _empty = false;
        fstream f;

        open_fileRW(f, (_tableName + ".tbl").c_str());
        FileRecord r;
        r = FileRecord(record);
        _last_record = r.write(f);

        f.close();

        reindex(record, _last_record);
    }

    Table read_recnos(vectorstr field_names)
    {
        ++_serial;
        Table t(_tableName + to_string(_serial), field_names);
        vector<long> col;
        for (int i = 0; i < field_names.size(); ++i)
        {
            col.push_back(_map.get(field_names.at(i)));
        }

        fstream f;
        open_fileRW(f, (_tableName + ".tbl").c_str());

        FileRecord r;
        for (int i = 0; i < _recnos.size(); ++i)
        {
            r.read(f, _recnos[i]);
            vectorstr temp = r.get_vector();
            vectorstr result;
            for (int i = 0; i < col.size(); ++i)
            {
                result.push_back(temp[col[i]]);
            }
            t.insert_into(result);
        }

        return t;
    }

    Table select(vectorstr field_names, string typeofField, string relation, string info)
    // this will take a vector of field_names with a field and a operator and an info to compare with
    {
        ++_serial;
        Table t(_tableName + to_string(_serial), field_names);
        vector<long> col;
        for (int i = 0; i < field_names.size(); ++i)
        {
            col.push_back(_map.get(field_names.at(i)));
        }

        _recnos.clear();
        t._recnos = get_recnos(typeofField, relation, info);
        _recnos = get_recnos(typeofField, relation, info);

        fstream f;
        open_fileRW(f, (_tableName + ".tbl").c_str());

        FileRecord r;
        for (int i = 0; i < _recnos.size(); ++i)
        {
            r.read(f, _recnos[i]);
            vectorstr temp = r.get_vector();
            vectorstr result;
            for (int i = 0; i < col.size(); ++i)
            {
                result.push_back(temp[col[i]]);
            }
            t.insert_into(result);
        }
        f.close();
        return t;
    }

    Table select(vectorstr field_names, const vectorstr &condition)
    // the type of fields will be given in the argument with the vectorstr conditions of info
    {
        cout << "condition: " << condition << endl;
        ShuntingYard sy(condition);
        return select(field_names, sy.postfix());
    }

    Table select(const vectorstr &field_names, Queue<Token *> post)
    // queue token is the same as condition
    {
        _recnos = rpn(field_names, post);
        return read_recnos(field_names);
    }

    vector<long> select_recnos()
    // this will return a vector of record numbers
    {
        return _recnos;
    }

    friend ostream &operator<<(ostream &outs, const Table &t)
    {
        // cout << "here is indices\n"
        //      << t._indices << endl;

        outs << "\nTable name: " << t._tableName << ", records: " << t._last_record + 1 << endl;

        fstream f;
        open_fileRW(f, (t._tableName + ".fields").c_str());

        FileRecord r;
        r.read(f, 0);
        outs << right << setw(25) << "record" << r << endl
             << endl;

        f.close();
        open_fileRW(f, (t._tableName + ".tbl").c_str());

        int record = 0;
        long pos = r.read(f, record);
        while (pos > 0)
        {
            outs << right << setw(25) << record << r << endl;
            record++;
            pos = r.read(f, record);
        }
        f.close();
        outs << endl;
        return outs;
    }

    void set_fields(vectorstr fields)
    {
        for (int i = 0; i < fields.size(); ++i)
        {
            _fieldNames.push_back(fields.at(i));
            _map.insert(fields.at(i), i);
            _indices.push_back(MMap<string, long>());
        }
    }

    vector<long> get_recnos(string typeofField, string relation, string info)
    {
        int i = _map.get(typeofField);
        vector<long> result;
        if (relation == "=")
        {
            result = _indices[i][info];
            return result;
        }

        MMap<string, long>::Iterator up(_indices[i].upper_bound(info));  // >
        MMap<string, long>::Iterator low(_indices[i].lower_bound(info)); // >=
        MMap<string, long>::Iterator begin(_indices[i].begin());
        MMap<string, long>::Iterator end(_indices[i].end());
        if (relation == ">")
        {
            result = it_loop(up, end);
        }
        else if (relation == "<")
        {
            result = it_loop(begin, low);
        }
        else if (relation == "<=")
        {
            result = it_loop(begin, up);
        }
        else if (relation == ">=")
        {
            result = it_loop(low, end);
        }
        else
        {
            cout << "what the hell is this: " << relation << endl;
        }
        return result;
    }

    vector<long> rpn(const vectorstr &field_names, Queue<Token *> que)
    {
        // cout << "rpn gets called" << endl;
        // cout << "this is your que" << endl
        //      << que << endl;
        vector<vector<long>> ql;
        Stack<Token *> s;

        while (que.size() != 0)
        {
            switch (que.front()->TypeOf())
            {
            case STRING:
            {
                // cout << "string" << endl;
                s.push(que.pop());
                // cout << s << endl;
                break;
            }
            case RELATION:
            {
                // cout << "relation" << endl;
                string info = static_cast<TokenStr *>(s.pop())->get_string();
                string field = static_cast<TokenStr *>(s.pop())->get_string();
                string relation = static_cast<Relational *>(que.pop())->get_string();

                ql.push_back(select(field_names, field, relation, info)._recnos);

                // cout << s << endl;
                // cout << "records: " << endl;
                // for (int i = 0; i < ql.size(); ++i)
                //     cout << ql[i] << endl;

                break;
            }
            case LOGIC:
            {
                // cout << "logic" << endl;
                vector<long> v1 = pop_front(ql);
                vector<long> v2 = pop_front(ql);
                string str = static_cast<Logical *>(que.pop())->get_string();
                if (str == "and")
                {
                    ql.push_back(get_intersection(v1, v2));
                }
                else if (str == "or")
                {
                    ql.push_back(get_union(v1, v2));
                }

                // cout << s << endl;
                // cout << "records: " << endl;
                // for (int i = 0; i < ql.size(); ++i)
                //     cout << ql[i] << endl;
                break;
            }
            }
        }
        return pop_front(ql);
    }

    vector<long>
    it_loop(MMap<string, long>::Iterator begin, MMap<string, long>::Iterator end)
    {
        vector<long> result;
        for (; begin != end; ++begin)
        {
            result += (*begin).value_list;
        }
        return result;
    }

    vectorstr get_fields()
    {
        return _fieldNames;
    }

    void reindex(vectorstr fields, long recno)
    {
        for (int i = 0; i < fields.size(); ++i)
        {
            _indices.at(i).insert(fields.at(i), recno);
        }
    }

private:
    string _tableName;
    vectorstr _fieldNames;
    vector<long> _recnos;
    Map<string, long> _map;
    bool _empty;
    long _last_record;
    vector<MMap<string, long>> _indices;
    long _serial;
};

#endif