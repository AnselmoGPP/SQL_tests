/*
 *  https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
 *  You can install SQLite library with "sudo apt-get install libsqlite3-dev"
 */

#include "sqlite3.h"

#include "auxiliar.hpp"

void generateWorldDB();
void queryManager();

int main(int argc, char** argv)
{
    //generateWorldDB();
    queryManager();

    return 0;
}

// ---------------------------------------

static int cb_description(void* data, int argc, char** argv, char** azColName)
{

    // "city","city_ascii","lat","lng","country","iso2","iso3","admin_name","capital","population","id"
    // "Las Palmas","Las Palmas","28.1272","-15.4314","Spain","ES","ESP","Canary Islands","admin","379925","1724501748"
    // "Madrid","Madrid","40.4189","-3.6919","Spain","ES","ESP","Madrid","primary","3266126","1724616994"
    // "Hassan","Hassan","13.0050","76.1028","India","IN","IND","Karnātaka","","133436","1356007455"

    // ASCII name
    std::cout << argv[1];

    // Actual city name
    if(strcmp(argv[0], argv[1]))
        std::cout << " (" << argv[0] << ")";

    // Location
    if(!strcmp(argv[8], "primary"))
        std::cout << " is the capital of " << argv[4] << ", it is located in " << argv[7];
    else
        std::cout << " is in " << argv[7] << " (" << argv[4] << ")";

    // Population
    std::cout << " and has " << (strcmp(argv[9], "-1")? argv[9] : "[unknown]") << " population" << std::endl;

    // Coordinates
    std::cout << "Lat: " << argv[2] << " / " << "Lon: " << argv[3] << std::endl;

    std::cout << std::endl;
    return 0;
}

void queryManager()
{
    std::string userInput(""), query("");
    char* messaggeError;

    std::cout << "Enter a city name (0 to exit)" << std::endl;

    for(;;)
    {
        // Get user input
        std::cout << "Search: ";
        std::getline(std::cin, userInput);

        if(userInput == "0") break;

        // Open DB
        sqlite3* DB;
        if (sqlite3_open("worldData.db", &DB))
        {
            std::cerr << "Cannot open DB: " << sqlite3_errmsg(DB) << std::endl;
            break;
        }

        // Query DB
        query = "SELECT * FROM Cities WHERE City_ASCII LIKE \'" + userInput + "\'";

        if (sqlite3_exec(DB, query.c_str(), cb_description, 0, &messaggeError) != SQLITE_OK)
        {
            std::cerr << "Error: " << messaggeError << std::endl;
            sqlite3_free(messaggeError);
            continue;
        }
    }
}

void generateWorldDB()
{
    // Extract data from a CSV file (just the data elements between quotation marks)
    std::vector<std::vector<std::string>> data;
    parserInputCSV2("worldcities.csv", &data);      // Extracts the sets of characters that are between quotation marks

    std::cout << "Data extracted from CSV" << std::endl;

    // Add another apostrophe wherever you find one to avoid sql syntax error (SQL scape sequence for ' is '')
    // Note: In SQL, string are denoted as a set of characters surrounded by apostrophes
    for(size_t i = 1; i < data.size(); ++i)
        for(size_t j = 0; j < data[i].size(); ++j)
            add_extra_apostrophes(data[i][j]);

    std::cout << "Extra apostrophes added" << std::endl;

    // If the "population" field (INT) is empty, SQL complains. So, if it's empty, it's assigned -1
    for(size_t i = 1; i < data.size(); ++i)
        if(data[i][9] == "") data[i][9] = "-1";

    std::cout << "-1 assigned to empty population fields" << std::endl;

    // Create a new DB (database)
    sqlite3* DB;

    if (sqlite3_open("worldData.db", &DB))
        std::cerr << "Cannot open DB: " << sqlite3_errmsg(DB) << std::endl;
    else
        std::cout << "Database opened" << std::endl;  // If database doesn't exists, it's created

    // Create table CITIES in our database
    char* messaggeError;

    std::string sqlCommand = "CREATE TABLE Cities(                   "  // Types: INT, REAL, TEXT, CHAR(50)
                                "City        TEXT,                   "  // Constraints: PRIMARY KEY, NOT NULL, UNIQUE, DEFAULT"abc"
                                "City_ASCII  TEXT,                   "
                                "Lat         REAL,                   "
                                "Lng         REAL,                   "
                                "Country     TEXT,                   "
                                "ISO2        TEXT,                   "
                                "ISO3        TEXT,                   "
                                "Admin_name  TEXT,                   "
                                "Capital     TEXT,                   "
                                "Population  INT,                    "
                                "Id          INT PRIMARY KEY NOT NULL"
                                ");                                  ";

    if (sqlite3_exec(DB, sqlCommand.c_str(), NULL, 0, &messaggeError) == SQLITE_OK)
        std::cout << "Table created" << std::endl;
    else
    {
        std::cerr << "Cannot create table: " << messaggeError << std::endl;
        sqlite3_free(messaggeError);
    }

    // Insert all records
    for(size_t i = 1; i < data.size(); ++i)
    {
        sqlCommand = "INSERT INTO Cities VALUES( "
                        "'" + data[i][0] + "', "
                        "'" + data[i][1] + "', "
                            + data[i][2] + " , "
                            + data[i][3] + " , "
                        "'" + data[i][4] + "', "
                        "'" + data[i][5] + "', "
                        "'" + data[i][6] + "', "
                        "'" + data[i][7] + "', "
                        "'" + data[i][8] + "', "
                            + data[i][9] + " , "
                            + data[i][10] + " ); ";

        if (sqlite3_exec(DB, sqlCommand.c_str(), NULL, 0, &messaggeError) == SQLITE_OK);
            //std::cout << "INSERT operation was successful!" << std::endl;
        else
        {
            std::cerr << i << ". INSERT error: " << messaggeError << std::endl;
            sqlite3_free(messaggeError);
        }

        std::cout << "Records: " << i << '/' << data.size()-1 << '\r';
    }

    std::cout << "Table filled" << std::endl;
    sqlite3_close(DB);
}
