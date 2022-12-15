#include "file_record.h"

long FileRecord::write(fstream &outs)
{
    // r.write(f); //take the Record r and write it into file f
    //   and return the record number of this Record

    // write to the end of the file.
    outs.seekg(0, outs.end);

    long pos = outs.tellp(); // retrieves the current position of the
                             //       file pointer

    // pay attention to this:
    // outs.write(&record[0], sizeof(record));
    outs.write(&_record[0][0], sizeof(_record));
    _recno = pos / sizeof(_record);

    return _recno; // record number
}

long FileRecord::read(fstream &ins, long recno)
{
    // returns the number of bytes read.
    //     r.read(f, 6);
    //     cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    // ins.read(&_record[0], sizeof(_record))
    ins.read(&_record[0][0], sizeof(_record));

    // don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    // don't need the null character, but for those of us with OCD and PTSD:
    while (_record[_size][0] != '\0')
    {
        ++_size;
    }

    return ins.gcount();
}

ostream &operator<<(ostream &outs, const FileRecord &r)
{
    for (int i = 0; i < r._size; ++i)
    {
        outs << right << setw(25) << r._record[i];
    }
    return outs;
}
