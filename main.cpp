#include <iostream>
#include <iomanip>
#include <windows.h>

#include "includes/table/table.h"

using namespace std;

Table make_table()
{
    vectorstr fields = {"fname", "lname", "age"};

    Table t("student", fields);
    vectorstr row;
    row = {"Joe", "Gomez", "20"};
    t.insert_into(row);
    row = {"Karen", "Orozco", "21"};
    t.insert_into(row);
    row = {"Flo", "Yao", "29"};
    t.insert_into(row);
    row = {"Jack", "Yao", "19"};
    t.insert_into(row);
    row = {"Flo", "Jackson", "20"};
    t.insert_into(row);
    row = {"Flo", "Gomez", "20"};
    t.insert_into(row);
    row = {"Karen", "Jackson", "15"};
    t.insert_into(row);
    return t;
}

int main(int argv, char **argc)
{
    cout << "\n\n"
         << endl;
    SetConsoleOutputCP(CP_UTF8);

    Table t = make_table();
    cout << "Here is table t: " << endl
         << t << endl;

    Table tbl_student("student");
    cout << "\n\nhere is the student table: " << tbl_student << endl;

    cout << "\n\n\n=====================" << endl;
    return 0;
}
