#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

void createTable(Connection* conn) {
    Statement* stmt = conn->createStatement();
    stmt->execute("CREATE TABLE IF NOT EXISTS students (id INT PRIMARY KEY AUTO_INCREMENT, name VARCHAR(100), age INT)");
    delete stmt;
}

void insertData(PreparedStatement* prepStmt, Connection* conn) {
    int numberOfStudents;
    cout << "Enter the number of students: ";
    cin >> numberOfStudents;

    cin.ignore(); // Ignore newline character left by cin

    prepStmt = conn->prepareStatement("INSERT INTO students (name, age) VALUES (?, ?)");

    for (int i = 0; i < numberOfStudents; ++i) {
        string name;
        int age;

        cout << "Enter the name of student " << i + 1 << ": ";
        getline(cin, name);

        cout << "Enter the age of student " << i + 1 << ": ";
        cin >> age;

        cin.ignore(); // Ignore newline character left by cin

        prepStmt->setString(1, name);
        prepStmt->setInt(2, age);
        prepStmt->executeUpdate();
    }

    delete prepStmt;
}

ResultSet* fetchData(Statement* stmt) {
    ResultSet* res = stmt->executeQuery("SELECT * FROM students");
    return res;
}

void displayData(ResultSet* res) {
    while (res->next()) {
        int id = res->getInt("id");
        string name = res->getString("name");
        int age = res->getInt("age");

        cout << "Id = " << id << ", name = " << name << ", age = " << age << endl;
    }
}

int main() {
    try {
        mysql::MySQL_Driver* driver;
        /*
        Manages the MySQL Connector/C++ driver instance.
        Provides methods for obtaining connections to MySQL databases.
        Typically, only one Driver instance is used per application.
        */
        Connection* conn;
        /*
        Represents a specific connection to a MySQL database.
        Handles interaction with the database server (e.g., executing SQL queries, managing transactions).
        Multiple Connection instances can exist within an application, each representing a separate connection to a different database.
        */
        Statement* stmt = nullptr;
        /* sql::Statement *stmt;
        Executes SQL statements (queries and updates).
        Does not store query results directly; instead, it returns a ResultSet for queries that return data.
        Used for all types of SQL operations, including SELECT, INSERT, UPDATE, DELETE, and DDL (Data Definition Language) statements.
        */
        PreparedStatement* prepStmt = nullptr;
        
        ResultSet* res = nullptr;
        /* sql::ResultSet *res;
        Represents the result of a SELECT query.
        Provides methods to iterate over rows and access column data.
        Specifically designed for handling query results and extracting data from them.
        */

        driver = mysql::get_mysql_driver_instance();
        /*
        driver is already defined above taht singletone instance of object 
        */
        conn = driver->connect("tcp://127.0.0.1:3306", "root", "1234"); //* it establlish connection
        conn->setSchema("Hostel");  //* setSchema is pure virtual function it is for set up which database u r using

        createTable(conn); //* creating tables
        insertData(prepStmt, conn); //* inserting data

        stmt = conn->createStatement(); //* Create a statement object associated with the connection
        res = fetchData(stmt);

        displayData(res);

        // Clean up resources
        delete res;     // Free ResultSet
        delete stmt;    // Free Statement
        delete prepStmt;// Free PreparedStatement
        delete conn;    // Free Connection
        delete driver;  // Free Driver

    } catch (SQLException &e) {
        cerr << "Error: " << e.what();
        cerr << " (MySQL error code: " << e.getErrorCode();
        cerr << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    return 0;
}
