#include "utilities.h"

bool file_exists(const char filename[])
{
    const bool debug = false;
    fstream ff;
    ff.open(filename,
            std::fstream::in | std::fstream::binary);
    if (ff.fail())
    {
        if (debug)
            cout << "file_exists(): File does NOT exist: " << filename << endl;
        return false;
    }
    if (debug)
        cout << "file_exists(): File DOES exist: " << filename << endl;
    ff.close();
    return true;
}

void open_fileRW(fstream &f, const char filename[])
{
    const bool debug = false;
    // opening a nonexistent file for in|out|app causes a fail()
    //   so, if the file does not exist, create it by opening it for
    //   output:
    if (!file_exists(filename))
    {
        // create the file
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            cout << "file open (RW) failed: with out|" << filename << "]" << endl;
            throw("file RW failed  ");
        }
        else
        {
            if (debug)
                cout << "open_fileRW: file created successfully: " << filename << endl;
        }
    }
    else
    {
        f.open(filename,
               std::fstream::in | std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            cout << "file open (RW) failed. [" << filename << "]" << endl;
            throw("file failed to open.");
        }
    }
}

void open_fileW(fstream &f, const char filename[])
{
    f.open(filename,
           std::fstream::out | std::fstream::binary);
    if (f.fail())
    {
        cout << "file open failed: " << filename << endl;
        throw("file failed to open.");
    }
}
