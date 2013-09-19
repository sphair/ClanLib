
/*! \page DatabaseServices Database Services


Contents
•1 General use
•2 Connecting to a database
•3 Creating SQL statements
•4 Executing a command and reading back results ?4.1 execute_non_query
?4.2 execute_scalar_string
?4.3 execute_scalar_int
?4.4 execute_reader

•5 Transactions
•6 Schemas
•7 Complete example
•8 Providers
 

General use

Database access in ClanLib is basically handled through 4 classes: 
• CL_DBConnection 
• CL_DBCommand 
• CL_DBReader 
• CL_DBTransaction 

To use ClanLib Database API with the ClanSqlite provider, you need to include these in your application: 


 
#include <ClanLib/core.h>
#include <ClanLib/database.h>
#include <ClanLib/sqlite.h>

Please note this overview will not learn you how SQL databases work in general, or how to write SQL statements. The purpose of this document is to describe how the ClanLib database API works. We assume you have general database knowledge before reading on. 

Connecting to a database

To connect to a database, you use the CL_DBConnection class. You need to use a provider specific implementation of it, since it depends on the target database system how to connect to it. ClanLib provides one provider currently, the Sqlite provider. Others might be added later, for instance MySQL or MS SQL Server. 

Since Sqlite is a file based database system, the connection only takes one parameter for connecting; a filename pointing to the database file. 


 
CL_String database = "mydatabase.db";
CL_SqliteConnection connection(database);

If something went wrong during this connection, it will throw an exception explaining why. 

Creating SQL statements

To issue a SQL query to a connection, you use the CL_DBCommand class. It is created using create_command on the connection object, and it takes a query string and a type. Type can be a sql statement (default) or a stored procedure. Sqlite only supports sql statements, but future providers might support both. 


 
CL_DBCommand command = connection.create_command("SELECT UserId, UserName FROM Users");

A command can also set input parameters. If the command query string contains a ?x, it requires a corresponding parameter to be set. 


 
CL_DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
command.set_input_parameter_string(1, "joe");
command.set_input_parameter_string(2, "secret");
command.set_input_parameter_string(3, "Joe McCoe");
 
CL_DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1");
command.set_input_parameter_int(1, user_id);
command.set_input_parameter_string(2, "Joe");

A templated function is also provided to reduce verbosity: 


 
CL_DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
command.set_input_parameter(1, "joe");
command.set_input_parameter(2, "secret");
command.set_input_parameter(3, "Joe McCoe");
 
CL_DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1");
command.set_input_parameter(1, user_id);
command.set_input_parameter(2, "Joe");

Alternatively you may set the input parameters like so: 


 
CL_DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)", "joe", "secret", "Joe McCoe");
 
CL_DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1", user_id, "Joe");


 The CL_DBCommand class support strings, bools, int, doubles, DateTime and DataBuffer (binary) as parameters: 


 
set_input_parameter(int index, Type value); // where Type = CL_String or bool or int or double or CL_DateTime or CL_DataBuffer
set_input_parameter_string(int index, const CL_StringRef &value);
set_input_parameter_bool(int index, bool value);
set_input_parameter_int(int index, int value);
set_input_parameter_double(int index, double value);
set_input_parameter_datetime(int index, const CL_DateTime &value);
set_input_parameter_binary(int index, const CL_DataBuffer &value);

Executing a command and reading back results

To execute a command, you call one of 4 execute methods on the connection object. 

execute_non_query

execute_non_query is used if you don't care about any returning values from the command. 


 
CL_DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
command.set_input_parameter_string(1, user_name);
command.set_input_parameter_string(2, password);
command.set_input_parameter_string(3, real_name);
connection.execute_non_query(command);

execute_scalar_string

execute_scalar_string is used if you have only one returning value. It returns the first column of the first row as a string. 


 
CL_DBCommand command = connection.create_command("SELECT UserName FROM Users WHERE UserId=?1");
command.set_input_parameter_int(1, user_id);
CL_String user_name = connection.execute_scalar_string(command);

execute_scalar_int

execute_scalar_int is used if you have only one returning value. It returns the first column of the first row as a integer. 


 
CL_DBCommand command = connection.create_command("SELECT UserId FROM Players WHERE PlayerId=?1");
command.set_input_parameter_int(1, player_id);
int user_id = connection.execute_scalar_int(command);

execute_reader

execute_reader is used if you have more than one returning value. 


 Preferred syntax: 


 
CL_DBCommand command = connection.create_command("SELECT UserId, UserName, CreateDate FROM Users WHERE UserId=?1", user_id);
CL_DBReader reader = connection.execute_reader(command);
while (reader.retrieve_row())
{
	int userId = reader.get_column_value(0);
	CL_String user_name = reader.get_column_value(1);
	CL_DateTime create_date = reader.get_column_value(2);
}
reader.close();

Retrieve a value by column name: 


 
CL_DBCommand command = connection.create_command("SELECT UserId, UserName FROM Users WHERE UserId=?1", user_id);
CL_DBReader reader = connection.execute_reader(command);
while (reader.retrieve_row())
{
	CL_String user_name = reader.get_column_value("UserName");
	int userId = reader.get_column_value("UserId");
}
reader.close();

You can retrieve values as strings, bools, integers, doubles, datetimes or binaries: 


 
Type get_column_value(int index) const; // where Type = CL_String or bool or int or double or CL_DateTime or CL_DataBuffer
Type get_column_value(const CL_StringRef &column_name) const; // where Type = CL_String or bool or int or double or CL_DateTime or CL_DataBuffer


 Old syntax: 


 
CL_DBCommand command = connection.create_command("SELECT UserId, UserName, CreateDate FROM Users WHERE UserId=?1");
command.set_input_parameter_int(1, user_id);
CL_DBReader reader = connection.execute_reader(command);
while (reader.retrieve_row())
{
	int userId = reader.get_column_int(0);
	CL_String user_name = reader.get_column_string(1);
	CL_DateTime create_date = reader.get_column_datetime(2);
}
reader.close();

You can retrieve values as strings, bools, integers, doubles, datetimes or binaries: 


 
CL_String get_column_string(int index) const;
bool get_column_bool(int index) const;
int get_column_int(int index) const;
double get_column_double(int index) const;
CL_DateTime get_column_datetime(int index) const;
CL_DataBuffer get_column_binary(int index) const;

You can retrieve values as strings, bools, integers, doubles, datetimes or binaries using a column name: 


 
CL_String get_column_string(const CL_StringRef &column_name) const;
bool get_column_bool(const CL_StringRef &column_name) const;
int get_column_int(const CL_StringRef &column_name) const;
double get_column_double(const CL_StringRef &column_name) const;
CL_DateTime get_column_datetime(const CL_StringRef &column_name) const;
CL_DataBuffer get_column_binary(const CL_StringRef &column_name) const;


 You can also query a reader about column count, column names, etc. Remember to close the reader when you are done with it. 

Transactions

A transaction is a "sequence of operations performed as a single logical unit of work". It is a tool to make sure the entire operation you want to perform on the database is either completely executed, or not executed at all. 


 
CL_DBTransaction transaction = connection.begin_transaction();
CL_DBCommand command = connection.create_command("INSERT INTO Users (UserName) VALUES (?1)");
command.set_input_parameter_string(1, user_name);
connection.execute_non_query(command);
transaction.commit();

You can either commit or rollback a transaction, depending on how the query statements were executed. 

If the transaction object gets destroyed without committing, the default behavior is to rollback the transaction. 

Schemas

A database require a database schema which describes tables, relationships, keys etc. This can be created using a set of CL_DBCommand functions, usage of a gui toolkit that supports Sqlite, or use the sqlite command line tool sqlite3.exe. 

An example for a schema that sqlite3.exe can execute: 


 
BEGIN TRANSACTION;
 
CREATE TABLE Users (
	UserId INTEGER PRIMARY KEY,
	UserName TEXT NOT NULL,
	Password TEXT NOT NULL,
	RealName TEXT NOT NULL,
	CreateDate DATETIME DEFAULT CURRENT_TIMESTAMP,
	LastLoginDate DATETIME DEFAULT NULL
);
 
CREATE TABLE Characters (
	CharacterId INTEGER PRIMARY KEY,
	UserId INTEGER NOT NULL,
	Name TEXT NOT NULL
);
 
COMMIT;

Complete example

See the SQL example in the ClanLib distribution for a complete example showing Create, Retrieve, Update and Delete operations. 

Providers

The ClanLib database API is modelled after a provider pattern, making it is possible to add support for new database systems. If you want to look into this, you basically have to make provider-specific implementations of CL_DBConnection, CL_DBCommandProvider, CL_DBConnectionProvider, CL_DBReaderProvider and CL_DBTransactionProvider. See the ClanSqlite implementation for an example of this. 


*/
