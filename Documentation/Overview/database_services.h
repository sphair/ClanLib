
/*! \page DatabaseServices Database Services

<h2>General use</h2>

Database access in ClanLib is basically handled through 4 classes: 
\ref clan::DBConnection 
\ref clan::DBCommand 
\ref clan::DBReader 
\ref clan::DBTransaction 

To use ClanLib Database API with the ClanSqlite provider, you need to include these in your application: 

\code 
#include <ClanLib/core.h>
#include <ClanLib/database.h>
#include <ClanLib/sqlite.h>
\endcode

Please note this overview will not learn you how SQL databases work in general, or how to write SQL statements. The purpose of this
document is to describe how the ClanLib database API works. We assume you have general database knowledge before reading on. 

<h2>Connecting to a database</h2>

To connect to a database, you use the \ref clan::DBConnection class. You need to use a provider specific implementation of it, since
it depends on the target database system how to connect to it. ClanLib provides one provider currently, the Sqlite provider. Others
might be added later, for instance MySQL or MS SQL Server. 

Since Sqlite is a file based database system, the connection only takes one parameter for connecting; a filename pointing to the database file. 

\code 
clan::String database = "mydatabase.db";
clan::SqliteConnection connection(database);
\endcode

If something went wrong during this connection, it will throw an exception explaining why. 

<h2>Creating SQL statements</h2>

To issue a SQL query to a connection, you use the \ref clan::DBCommand class. It is created using create_command on the connection object, and
it takes a query string and a type. Type can be a sql statement (default) or a stored procedure. Sqlite only supports sql statements, but
future providers might support both. 

\code
clan::DBCommand command = connection.create_command("SELECT UserId, UserName FROM Users");
\endcode

A command can also set input parameters. If the command query string contains a ?x, it requires a corresponding parameter to be set. 

\code
clan::DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
command.set_input_parameter_string(1, "joe");
command.set_input_parameter_string(2, "secret");
command.set_input_parameter_string(3, "Joe McCoe");
 
clan::DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1");
command.set_input_parameter_int(1, user_id);
command.set_input_parameter_string(2, "Joe");
\endcode

A templated function is also provided to reduce verbosity: 

\code
clan::DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
command.set_input_parameter(1, "joe");
command.set_input_parameter(2, "secret");
command.set_input_parameter(3, "Joe McCoe");
 
clan::DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1");
command.set_input_parameter(1, user_id);
command.set_input_parameter(2, "Joe");
\endcode

Alternatively you may set the input parameters like so: 

\code
clan::DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)", "joe", "secret", "Joe McCoe");
 
clan::DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1", user_id, "Joe");
\endcode

The \ref clan::DBCommand class support strings, bools, int, doubles, DateTime and DataBuffer (binary) as parameters: 

\code
set_input_parameter(int index, Type value); // where Type = clan::String or bool or int or double or clan::DateTime or clan::DataBuffer
set_input_parameter_string(int index, const clan::StringRef &value);
set_input_parameter_bool(int index, bool value);
set_input_parameter_int(int index, int value);
set_input_parameter_double(int index, double value);
set_input_parameter_datetime(int index, const clan::DateTime &value);
set_input_parameter_binary(int index, const clan::DataBuffer &value);
\endcode

<h2>Executing a command and reading back results</h2>

To execute a command, you call one of 4 execute methods on the connection object. 

<h2>execute_non_query</h2>

execute_non_query is used if you don't care about any returning values from the command. 

\code 
clan::DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
command.set_input_parameter_string(1, user_name);
command.set_input_parameter_string(2, password);
command.set_input_parameter_string(3, real_name);
connection.execute_non_query(command);
\endcode

<h2>execute_scalar_string</h2>

execute_scalar_string is used if you have only one returning value. It returns the first column of the first row as a string. 

\code
clan::DBCommand command = connection.create_command("SELECT UserName FROM Users WHERE UserId=?1");
command.set_input_parameter_int(1, user_id);
clan::String user_name = connection.execute_scalar_string(command);
\endcode

<h2>execute_scalar_int</h2>

execute_scalar_int is used if you have only one returning value. It returns the first column of the first row as a integer. 

\code
clan::DBCommand command = connection.create_command("SELECT UserId FROM Players WHERE PlayerId=?1");
command.set_input_parameter_int(1, player_id);
int user_id = connection.execute_scalar_int(command);
\endcode

<h2>execute_reader</h2>

execute_reader is used if you have more than one returning value. 

Preferred syntax: 

\code
clan::DBCommand command = connection.create_command("SELECT UserId, UserName, CreateDate FROM Users WHERE UserId=?1", user_id);
clan::DBReader reader = connection.execute_reader(command);
while (reader.retrieve_row())
{
	int userId = reader.get_column_value(0);
	clan::String user_name = reader.get_column_value(1);
	clan::DateTime create_date = reader.get_column_value(2);
}
reader.close();
\endcode

<h2>Retrieve a value by column name</h2>

\code
clan::DBCommand command = connection.create_command("SELECT UserId, UserName FROM Users WHERE UserId=?1", user_id);
clan::DBReader reader = connection.execute_reader(command);
while (reader.retrieve_row())
{
	clan::String user_name = reader.get_column_value("UserName");
	int userId = reader.get_column_value("UserId");
}
reader.close();
\endcode

You can retrieve values as strings, bools, integers, doubles, datetimes or binaries: 

\code
Type get_column_value(int index) const; // where Type = clan::String or bool or int or double or clan::DateTime or clan::DataBuffer
Type get_column_value(const clan::StringRef &column_name) const; // where Type = clan::String or bool or int or double or clan::DateTime or clan::DataBuffer
\endcode

You can retrieve values as strings, bools, integers, doubles, datetimes or binaries: 

\code
 
clan::String get_column_string(int index) const;
bool get_column_bool(int index) const;
int get_column_int(int index) const;
double get_column_double(int index) const;
clan::DateTime get_column_datetime(int index) const;
clan::DataBuffer get_column_binary(int index) const;
\endcode

You can retrieve values as strings, bools, integers, doubles, datetimes or binaries using a column name: 

\code
 
clan::String get_column_string(const clan::StringRef &column_name) const;
bool get_column_bool(const clan::StringRef &column_name) const;
int get_column_int(const clan::StringRef &column_name) const;
double get_column_double(const clan::StringRef &column_name) const;
clan::DateTime get_column_datetime(const clan::StringRef &column_name) const;
clan::DataBuffer get_column_binary(const clan::StringRef &column_name) const;
\endcode

 You can also query a reader about column count, column names, etc. Remember to close the reader when you are done with it. 

<h2>Transactions</h2>

A transaction is a "sequence of operations performed as a single logical unit of work". It is a tool to make sure the entire operation you want to perform on the database is either completely executed, or not executed at all. 

\code
 
clan::DBTransaction transaction = connection.begin_transaction();
clan::DBCommand command = connection.create_command("INSERT INTO Users (UserName) VALUES (?1)");
command.set_input_parameter_string(1, user_name);
connection.execute_non_query(command);
transaction.commit();
\endcode

You can either commit or rollback a transaction, depending on how the query statements were executed. 

If the transaction object gets destroyed without committing, the default behavior is to rollback the transaction. 

<h2>Schemas</h2>

A database require a database schema which describes tables, relationships, keys etc. This can be created using a set of clan::DBCommand functions, usage of a gui toolkit that supports Sqlite, or use the sqlite command line tool sqlite3.exe. 

An example for a schema that sqlite3.exe can execute: 

\code 
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
\endcode

See the SQL example in the ClanLib distribution for a complete example showing Create, Retrieve, Update and Delete operations. 

<h2>Providers</h2>

The ClanLib database API is modelled after a provider pattern, making it is possible to add support for new database systems. If you want to look
into this, you basically have to make provider-specific implementations of \ref clan::DBConnection, \ref clan::DBCommandProvider, \ref clan::DBConnectionProvider,
\ref clan::DBReaderProvider and \ref clan::DBTransactionProvider.

See the ClanSqlite implementation for an example of this.

*/
