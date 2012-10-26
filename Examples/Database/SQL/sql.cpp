/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    
*/

#include <ClanLib/core.h>
#include <ClanLib/database.h>
#include <ClanLib/sqlite.h>
using namespace clan;

void show_all_users(SqliteConnection &connection);
void show_specific_user(SqliteConnection &connection, int user_id);
void show_specific_user_characters(SqliteConnection &connection, int user_id);
int create_new_user(SqliteConnection &connection, std::string user_name, std::string password, std::string real_name);
void update_user_realname(SqliteConnection &connection, int user_id, std::string real_name);
void delete_specific_user(SqliteConnection &connection, int user_id);

int main(int argc, char **argv)
{
	try
	{
		SetupCore setup_core;

		ConsoleWindow console("Console");

		std::string database = PathHelp::normalize("Database/clanlib.db");

		SqliteConnection connection(database);

		show_all_users(connection);
		show_specific_user(connection, 1);
		show_specific_user_characters(connection, 1);

		int id = create_new_user(connection, "testuser", "test", "Test User");
		show_specific_user(connection, id);

		update_user_realname(connection, id, "Real Name");
		show_specific_user(connection, id);

		show_all_users(connection);
		delete_specific_user(connection, id);
		show_all_users(connection);

		console.display_close_message();

	}
	catch (Exception &e)
	{
		ConsoleWindow console("Console");
		Console::write_line("Exception caught: " + e.get_message_and_stack_trace());
		console.display_close_message();
		return 0;
	}
	return 1;
}

void show_all_users(SqliteConnection &connection)
{
	Console::write_line("All available users:");

	DBCommand command = connection.create_command("SELECT UserId, UserName FROM Users");
	DBReader reader = connection.execute_reader(command);
	while (reader.retrieve_row())
	{
		int user_id = reader.get_column_int(0);
		std::string user_name = reader.get_column_string(1);

		Console::write_line("- Id: %1 Username: %2", user_id, user_name);
	}
	reader.close();

	Console::write_line(std::string());
}

void show_specific_user(SqliteConnection &connection, int user_id)
{
	Console::write_line("User info for user %1: ", user_id);

	DBCommand command = connection.create_command("SELECT UserId, UserName, Password, RealName, CreateDate, LastLoginDate FROM Users WHERE UserId=?1");
	command.set_input_parameter_int(1, user_id);
	DBReader reader = connection.execute_reader(command);
	while (reader.retrieve_row())
	{
		int user_id = reader.get_column_int(0);
		std::string user_name = reader.get_column_string(1);
		std::string password = reader.get_column_string(2);
		std::string real_name = reader.get_column_string(3);
		DateTime create_date = reader.get_column_datetime(4);
		DateTime last_login_date = reader.get_column_datetime(5);

		Console::write_line("- User id: %1", user_id);
		Console::write_line("- Username: %1", user_name);
		Console::write_line("- Password: %1", password);
		Console::write_line("- Real name: %1", real_name);
		Console::write_line("- Create date: %1", create_date.to_short_datetime_string());
		if(!last_login_date.is_null())
			Console::write_line("- Last login date: %1", last_login_date.to_short_datetime_string());
		else
			Console::write_line("- Last login date: NULL");
	}
	reader.close();

	Console::write_line(std::string());
}

void show_specific_user_characters(SqliteConnection &connection, int user_id)
{
	Console::write_line("Characters for user %1: ", user_id);

	DBCommand command = connection.create_command("SELECT Characters.CharacterId, Characters.Name, Users.UserName FROM Characters INNER JOIN Users ON Characters.UserId = Users.UserId WHERE Characters.UserId=?1");
	command.set_input_parameter_int(1, user_id);
	DBReader reader = connection.execute_reader(command);
	while (reader.retrieve_row())
	{
		int character_id = reader.get_column_int(0);
		std::string character_name = reader.get_column_string(1);
		std::string user_name = reader.get_column_string(2);

		Console::write_line("- (Username %3) - Character id: %1 name: %2 ", character_id, character_name, user_name);
	}
	reader.close();

	Console::write_line(std::string());
}

int create_new_user(SqliteConnection &connection, std::string user_name, std::string password, std::string real_name)
{
	Console::write_line("Creating new user: %1", user_name);

	DBTransaction transaction = connection.begin_transaction();

	DBCommand command = connection.create_command("INSERT INTO Users (UserName, Password, RealName) VALUES (?1,?2,?3)");
	command.set_input_parameter_string(1, user_name);
	command.set_input_parameter_string(2, password);
	command.set_input_parameter_string(3, real_name);
	connection.execute_non_query(command);

	transaction.commit();

	int id = command.get_output_last_insert_rowid();

	Console::write_line("New user created with id %1", id);
	Console::write_line(std::string());

	return id;
}

void update_user_realname(SqliteConnection &connection, int user_id, std::string real_name)
{
	Console::write_line("Updating realname of user %1 with realname %2", user_id, real_name);

	DBTransaction transaction = connection.begin_transaction();

	DBCommand command = connection.create_command("UPDATE Users SET RealName=?2 WHERE UserId=?1");
	command.set_input_parameter_int(1, user_id);
	command.set_input_parameter_string(2, real_name);
	connection.execute_non_query(command);

	transaction.commit();

	Console::write_line(std::string());
}

void delete_specific_user(SqliteConnection &connection, int user_id)
{
	Console::write_line("Deleting user %1", user_id);

	DBTransaction transaction = connection.begin_transaction();

	DBCommand command = connection.create_command("DELETE FROM Users Where UserId=?1");
	command.set_input_parameter_int(1, user_id);
	connection.execute_non_query(command);

	transaction.commit();

	Console::write_line(std::string());
}
