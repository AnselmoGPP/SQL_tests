/*
 *  https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
 *  You can install SQLite library with "sudo apt-get install libsqlite3-dev"
 */

#include "auxiliar.hpp"

int main(int argc, char** argv)
{
    open_DB();
    create_DB();
    //show_table(DB);
    insert_and_delete();
    select();
}
