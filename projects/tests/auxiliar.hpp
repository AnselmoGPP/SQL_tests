#ifndef AUXILIAR_HPP
#define AUXILIAR_HPP

/*
 *  Functions used:
 *
 *      sqlite3_open():     Open DB
 *      sqlite3_close():    Close DB
 *      sqlite3_errmsg():   Get error message from a (sqlite3*)DB
 *      sqlite3_free():     Free memory allocated with sqlite3_malloc() or sqlite3_realloc()
 *
 *      sqlite3_exec():     Runs an SQL statement (create DB,
 *                          sqlite3_exec( sqlite3* openDB,
 *                                        const char* SQLtoEvaluate,
 *                                        int (*callbackFunction)(void*, int, char**, char**),
 *                                        void* firstArgumentToCallback,
 *                                        char** errorMessagge );
 *
 *      static int (*sqlite3_callback)( void*  data_provided_by_fourth_argument_of_sqlite3_exec,
 *                                      int    num_columns_in_a_row,
 *                                      char** array_of_strings_representing_fields_in_a_row,
 *                                      char** array_of_strings_representing_column_names );
 */

#include <iostream>
#include <string>

#include "sqlite3.h"
//#include "sqlite3ext.h"

/// Opens an existing database (or creates it if it doesn't exists)
void open_DB()
{
    sqlite3* DB;

    if (sqlite3_open("database_1.db", &DB))
        std::cerr << "Cannot open DB: " << sqlite3_errmsg(DB) << std::endl;
    else
        std::cout << "Database open successfully!" << std::endl;  // If database doesn't exists, it's created

    sqlite3_close(DB);
}

/// Create a new database
void create_DB()
{
    sqlite3* DB;
    sqlite3_open("database_1.db", &DB);

    char* messaggeError;
    std::string sqlCommands = "CREATE TABLE PERSON(                 "
                              "ID        INT PRIMARY KEY  NOT NULL, "
                              "NAME      TEXT             NOT NULL, "
                              "SURNAME   TEXT             NOT NULL, "
                              "AGE       INT              NOT NULL, "
                              "ADDRESS   CHAR(50),                  "
                              "SALARY    REAL                       "
                              ");                                   ";

    if (sqlite3_exec(DB, sqlCommands.c_str(), NULL, 0, &messaggeError) == SQLITE_OK)
        std::cout << "Table created Successfully" << std::endl;
    else
    {
        std::cerr << "Cannot create table: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
    }

    sqlite3_close(DB);
}

/// Callback function. It's run for every record, and knows the columns' names and record's values
static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;

    fprintf(stderr, "%s: \n", (const char*)data);                        //std::cerr << (const char*)data << std::endl;

    for (i = 0; i < argc; i++)
        printf("%s = %s \n", azColName[i], argv[i] ? argv[i] : "NULL");  //std::cout << azColName[i] << " = " << argv[i] ? argv[i] : "NULL" << std::endl;

    printf("\n");
    return 0;
}

/// Show current state of a table
void show_table(sqlite3* DB, std::string tableName)
{
    std::string query = "SELECT * FROM " + tableName + ";";
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

/// Insert new records & Delete existing records
void insert_and_delete()
{
    sqlite3* DB;
    sqlite3_open("database_1.db", &DB);

    char* messaggeError;

    std::cout << ">State of table before INSERT:" << std::endl;
    show_table(DB, "PERSON");

    // INSERT new records into table PERSON
    std::string sqlCommands("INSERT INTO PERSON VALUES( 1, 'STEVE', 'GATES', 30, 'PALO ALTO', 1000.0 ); "
                            "INSERT INTO PERSON VALUES( 2, 'BILL',  'ALLEN', 20, 'SEATTLE',   300.22 ); "
                            "INSERT INTO PERSON VALUES( 3, 'PAUL',  'JOBS',  24, 'SEATTLE',   9900.0 ); ");

    if (sqlite3_exec(DB, sqlCommands.c_str(), NULL, 0, &messaggeError) == SQLITE_OK)
        std::cout << "Records created successfully!" << std::endl;
    else
    {
        std::cerr << "INSERT error: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
    }

    std::cout << ">State of table after INSERT" << std::endl;
    show_table(DB, "PERSON");

    // DELETE some record from table PERSON
    sqlCommands = "DELETE FROM PERSON WHERE ID = 2;";
    if (sqlite3_exec(DB, sqlCommands.c_str(), NULL, 0, &messaggeError) == SQLITE_OK)
        std::cout << "Record deleted successfully!" << std::endl;
    else
    {
        std::cerr << "DELETE error: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
    }

    std::cout << ">State of table after DELETE:" << std::endl;
    show_table(DB, "PERSON");

    sqlite3_close(DB);
}

/// Select all elements in the table
void select()
{
    std::cout << ">Select all elements from the DB:" << std::endl;

    sqlite3* DB;
    sqlite3_open("database_1.db", &DB);

    char* messaggeError;

    std::string data("CALLBACK FUNCTION");
    std::string sqlCommand("SELECT * FROM PERSON;");

    if (sqlite3_exec(DB, sqlCommand.c_str(), callback, (void*)data.c_str(), &messaggeError) == SQLITE_OK)
        std::cout << "SELECT operation was successful!" << std::endl;
    else
    {
        std::cerr << "SELECT error: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
    }

    sqlite3_close(DB);
}

#endif
